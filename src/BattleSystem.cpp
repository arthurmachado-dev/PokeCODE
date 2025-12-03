#include "../include/BattleSystem.h"
#include "../include/Utils.h"
#include "../include/Trainer.h"
#include "../include/Pokemon.h"
#include "../include/Move.h"
#include "../include/Pokedex.h"
#include <iostream>
#include <cmath>

BattleSystem::BattleSystem(std::shared_ptr<Trainer> player, 
                           std::shared_ptr<Trainer> opponent,
                           std::shared_ptr<Pokedex> pokedex)
    : player(player), opponent(opponent), pokedex(pokedex),
      battleOver(false), winner(nullptr) {}

void BattleSystem::startBattle() {
    battleOver = false;
    winner = nullptr;
    
    if (!player->getActivePokemon() || player->getActivePokemon()->getIsFainted()) {
        player->chooseNewActivePokemon();
    }
    
    if (!opponent->getActivePokemon() || opponent->getActivePokemon()->getIsFainted()) {
        opponent->chooseNewActivePokemon();
    }
    
    std::cout << "\n=== BATALHA POKEMON ===\n";
    std::cout << player->getName() << " vs " << opponent->getName() << "\n\n";
    
    std::cout << player->getName() << " enviou " << 
        player->getActivePokemon()->getName() << "!\n";
    std::cout << opponent->getName() << " enviou " << 
        opponent->getActivePokemon()->getName() << "!\n";
    
    printBattleStatus();
}

int BattleSystem::calculateDamage(std::shared_ptr<Pokemon> attacker, 
                                 std::shared_ptr<Pokemon> defender,
                                 std::shared_ptr<Move> move) {
    if (attacker->getIsFainted()) {
        std::cout << attacker->getName() << " esta desmaiado!\n";
        return 0;
    }
    
    if (move->getCategory() == "Status") return 0;
    
    // Verificar precisão do movimento
    if (!Utils::chance(move->getAccuracy() / 100.0)) {
        std::cout << move->getName() << " errou!\n";
        return 0;
    }
    
    int attackStat, defenseStat;
    if (move->getCategory() == "Physical") {
        attackStat = attacker->getAttack();
        defenseStat = defender->getDefense();
    } else {
        attackStat = attacker->getSpAttack();
        defenseStat = defender->getSpDefense();
    }
    
    // STAB (Same Type Attack Bonus)
    double stab = 1.0;
    if (move->getType() == attacker->getType1() || 
        move->getType() == attacker->getType2()) {
        stab = 1.5;
    }
    
    // Multiplicador de tipo
    double typeMultiplier = getAttackMultiplier(move->getType(), defender);
    
    if (typeMultiplier > 1.0) {
        std::cout << "E super efetivo!\n";
    } else if (typeMultiplier < 1.0 && typeMultiplier > 0) {
        std::cout << "Nao e muito efetivo...\n";
    } else if (typeMultiplier == 0) {
        std::cout << "Nao afetou " << defender->getName() << "...\n";
        return 0;
    }
    
    // Fórmula de dano Pokémon simplificada
    int damage = static_cast<int>((((2 * attacker->getLevel() / 5.0 + 2) * 
                                   move->getPower() * attackStat / defenseStat) / 
                                  50.0 + 2) * stab * typeMultiplier);
    
    // Adicionar variação aleatória (85%-100%)
    damage = static_cast<int>(damage * Utils::randomDouble(0.85, 1.0));
    
    // Garantir dano mínimo de 1 se o ataque for bem-sucedido
    if (damage < 1 && typeMultiplier > 0) {
        damage = 1;
    }
    
    return std::max(1, damage);
}

double BattleSystem::getAttackMultiplier(const std::string& attackType,
                                        std::shared_ptr<Pokemon> defender) {
    return pokedex->getTypeEffectiveness(attackType, 
                                        defender->getType1(), 
                                        defender->getType2());
}

std::shared_ptr<Move> BattleSystem::chooseCPUMove() {
    auto pokemon = opponent->getActivePokemon();
    if (pokemon->getIsFainted()) return nullptr;
    
    auto moves = pokemon->getMoves();
    if (moves.empty()) return nullptr;
    
    std::vector<std::shared_ptr<Move>> availableMoves;
    for (const auto& move : moves) {
        if (move->getCurrentPP() > 0) availableMoves.push_back(move);
    }
    
    if (availableMoves.empty()) return nullptr;
    
    int index = Utils::randomInt(0, availableMoves.size() - 1);
    return availableMoves[index];
}

int BattleSystem::chooseCPUSwitch() {
    auto team = opponent->getTeam();
    std::vector<int> available;
    
    for (size_t i = 0; i < team.size(); i++) {
        if (!team[i]->getIsFainted() && team[i] != opponent->getActivePokemon()) {
            available.push_back(i);
        }
    }
    
    if (available.empty()) return -1;
    
    return available[Utils::randomInt(0, available.size() - 1)];
}

void BattleSystem::executePlayerTurn(int moveChoice) {
    auto playerPokemon = player->getActivePokemon();
    auto opponentPokemon = opponent->getActivePokemon();
    
    if (playerPokemon->getIsFainted()) {
        std::cout << playerPokemon->getName() << " esta desmaiado!\n";
        return;
    }
    
    auto move = playerPokemon->getMove(moveChoice);
    if (!move) {
        std::cout << "Movimento invalido!\n";
        return;
    }
    
    if (move->getCurrentPP() <= 0) {
        std::cout << "Nao ha PP para " << move->getName() << "!\n";
        return;
    }
    
    std::cout << "\n" << playerPokemon->getName() << " usou " << 
        move->getName() << "!\n";
    
    move->use();
    
    if (move->getCategory() != "Status") {
        int damage = calculateDamage(playerPokemon, opponentPokemon, move);
        if (damage > 0) {
            bool fainted = opponentPokemon->takeDamage(damage);
            std::cout << opponentPokemon->getName() << " sofreu " << 
                damage << " de dano!\n";
            
            if (fainted) {
                // Pokémon adversário desmaiou
                std::cout << opponentPokemon->getName() << " desmaiou!\n";
                
                // Verificar se oponente tem mais Pokémon
                if (opponent->hasUsablePokemon()) {
                    // Tentar trocar para outro Pokémon
                    int switchIndex = chooseCPUSwitch();
                    if (switchIndex != -1) {
                        opponent->switchPokemon(switchIndex);
                        std::cout << opponent->getName() << " enviou " << 
                            opponent->getActivePokemon()->getName() << "!\n";
                    }
                }
            }
        }
    }
}

void BattleSystem::executeCPUTurn() {
    auto playerPokemon = player->getActivePokemon();
    auto opponentPokemon = opponent->getActivePokemon();
    
    if (opponentPokemon->getIsFainted()) {
        // CPU tenta trocar de Pokémon
        int switchIndex = chooseCPUSwitch();
        if (switchIndex != -1) {
            opponent->switchPokemon(switchIndex);
            opponentPokemon = opponent->getActivePokemon();
            std::cout << opponent->getName() << " enviou " << 
                opponentPokemon->getName() << "!\n";
        } else {
            // Não tem mais Pokémon para trocar
            return;
        }
    }
    
    auto move = chooseCPUMove();
    if (!move) {
        std::cout << opponentPokemon->getName() << " nao tem movimentos disponiveis!\n";
        return;
    }
    
    std::cout << "\n" << opponent->getName() << ": " << 
        opponentPokemon->getName() << " usou " << move->getName() << "!\n";
    
    move->use();
    
    if (move->getCategory() != "Status") {
        int damage = calculateDamage(opponentPokemon, playerPokemon, move);
        if (damage > 0) {
            bool fainted = playerPokemon->takeDamage(damage);
            std::cout << playerPokemon->getName() << " sofreu " << 
                damage << " de dano!\n";
            
            if (fainted) {
                std::cout << playerPokemon->getName() << " desmaiou!\n";
            }
        }
    }
}

void BattleSystem::checkBattleEnd() {
    if (!player->hasUsablePokemon()) {
        battleOver = true;
        winner = opponent.get();
        std::cout << "\n" << Utils::Colors::RED;
        std::cout << player->getName() << " perdeu a batalha!\n";
        std::cout << Utils::Colors::RESET;
    } else if (!opponent->hasUsablePokemon()) {
        battleOver = true;
        winner = player.get();
        std::cout << "\n" << Utils::Colors::GREEN;
        std::cout << player->getName() << " venceu a batalha!\n";
        std::cout << Utils::Colors::RESET;
        awardExperience();
    }
}

void BattleSystem::awardExperience() {
    auto opponentTeam = opponent->getTeam();
    int totalExp = 0;
    
    for (const auto& pokemon : opponentTeam) {
        if (pokemon->getIsFainted()) {
            totalExp += pokemon->getLevel() * 100;
        }
    }
    
    auto playerTeam = player->getTeam();
    int usableCount = player->getUsablePokemonCount();
    
    if (usableCount > 0 && totalExp > 0) {
        int expPerPokemon = totalExp / usableCount;
        
        std::cout << "\nSeus Pokemon ganharam experiencia!\n";
        for (auto& pokemon : playerTeam) {
            if (!pokemon->getIsFainted()) {
                int oldLevel = pokemon->getLevel();
                pokemon->gainExperience(expPerPokemon);
                if (pokemon->getLevel() > oldLevel) {
                    std::cout << pokemon->getName() << " subiu para o nivel " << 
                        pokemon->getLevel() << "!\n";
                }
            }
        }
    }
}

void BattleSystem::playerAttack(int moveIndex) {
    if (battleOver) return;
    
    executePlayerTurn(moveIndex);
    
    // Verificar se o oponente ainda tem Pokémon vivo
    if (!opponent->getActivePokemon()->getIsFainted()) {
        executeCPUTurn();
    }
    
    // Verificar fim da batalha APÓS ambos os turnos
    checkBattleEnd();
    
    if (!battleOver) {
        printBattleStatus();
    } else {
        // Batalha terminou, mostrar resultado final
        std::cout << "\n=== BATALHA ENCERRADA ===\n";
        if (winner == player.get()) {
            std::cout << Utils::Colors::GREEN << "VITORIA! " << player->getName() << " venceu!\n" << Utils::Colors::RESET;
        } else {
            std::cout << Utils::Colors::RED << "DERROTA! " << player->getName() << " perdeu!\n" << Utils::Colors::RESET;
        }
    }
}

void BattleSystem::playerSwitchPokemon(int pokemonIndex) {
    if (battleOver) return;
    
    if (player->switchPokemon(pokemonIndex)) {
        executeCPUTurn();
        checkBattleEnd();
        if (!battleOver) printBattleStatus();
    }
}

void BattleSystem::playerUseItem() {
    if (battleOver) return;
    
    auto activePokemon = player->getActivePokemon();
    if (activePokemon->getIsFainted()) {
        std::cout << activePokemon->getName() << " esta desmaiado!\n";
        return;
    }
    
    std::cout << "Usou Pocao! +20 HP\n";
    activePokemon->heal(20);
    
    executeCPUTurn();
    checkBattleEnd();
    if (!battleOver) printBattleStatus();
}

bool BattleSystem::playerAttemptRun() {
    if (battleOver) return true;
    
    auto playerPokemon = player->getActivePokemon();
    auto opponentPokemon = opponent->getActivePokemon();
    
    if (playerPokemon->getIsFainted()) {
        std::cout << "Nao pode fugir com Pokemon desmaiado!\n";
        return false;
    }
    
    double escapeChance = playerPokemon->getSpeed() / 
                         (double)(playerPokemon->getSpeed() + opponentPokemon->getSpeed());
    
    if (Utils::chance(escapeChance)) {
        std::cout << "Fuga bem sucedida!\n";
        battleOver = true;
        return true;
    } else {
        std::cout << "Nao foi possivel fugir!\n";
        executeCPUTurn();
        checkBattleEnd();
        if (!battleOver) printBattleStatus();
        return false;
    }
}

void BattleSystem::printBattleStatus() const {
    std::cout << "\n=== STATUS DA BATALHA ===\n";
    
    auto opp = opponent->getActivePokemon();
    std::cout << "\n" << opponent->getName() << ":\n";
    std::cout << opp->getName() << " Nv." << opp->getLevel();
    if (opp->getIsFainted()) std::cout << " [DESMAIADO]";
    std::cout << "\nHP: " << opp->getHPBar() << "\n";
    std::cout << opp->getCurrentHP() << "/" << opp->getMaxHP() << "\n";
    
    auto playerPoke = player->getActivePokemon();
    std::cout << "\n" << player->getName() << ":\n";
    std::cout << playerPoke->getName() << " Nv." << playerPoke->getLevel();
    if (playerPoke->getIsFainted()) std::cout << " [DESMAIADO]";
    std::cout << "\nHP: " << playerPoke->getHPBar() << "\n";
    std::cout << playerPoke->getCurrentHP() << "/" << playerPoke->getMaxHP() << "\n";
    std::cout << "=======================\n";
}

void BattleSystem::printAvailableMoves() const {
    auto pokemon = player->getActivePokemon();
    
    if (pokemon->getIsFainted()) {
        std::cout << pokemon->getName() << " esta desmaiado!\n";
        return;
    }
    
    auto moves = pokemon->getMoves();
    std::cout << "\nMovimentos disponiveis:\n";
    for (size_t i = 0; i < moves.size(); i++) {
        std::cout << (i + 1) << ". " << moves[i]->getName();
        std::cout << " (" << moves[i]->getCurrentPP() << "/" << 
            moves[i]->getMaxPP() << " PP)";
        std::cout << " [" << moves[i]->getType() << ", " << 
            moves[i]->getCategory() << "]";
        if (moves[i]->getPower() > 0) {
            std::cout << " Poder: " << moves[i]->getPower();
        }
        std::cout << "\n";
    }
}

std::shared_ptr<Pokemon> BattleSystem::getPlayerActive() const {
    return player->getActivePokemon();
}

std::shared_ptr<Pokemon> BattleSystem::getOpponentActive() const {
    return opponent->getActivePokemon();
}

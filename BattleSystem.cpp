#include "../include/BattleSystem.h"
#include "../include/Utils.h"
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
    
    Utils::printColored("\n=== BATALHA POKÉMON ===", Utils::Colors::YELLOW);
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
    if (move->getCategory() == "Status") {
        return 0;
    }
    
    // Verificar se acerta
    if (!Utils::chance(move->getAccuracy() / 100.0)) {
        std::cout << move->getName() << " errou!\n";
        return 0;
    }
    
    // Dano base
    int attackStat, defenseStat;
    if (move->getCategory() == "Physical") {
        attackStat = attacker->getAttack();
        defenseStat = defender->getDefense();
    } else { // Special
        attackStat = attacker->getSpAttack();
        defenseStat = defender->getSpDefense();
    }
    
    // STAB (Same Type Attack Bonus)
    double stab = 1.0;
    if (move->getType() == attacker->getType1() || 
        move->getType() == attacker->getType2()) {
        stab = 1.5;
    }
    
    // Efetividade de tipo
    double typeMultiplier = getAttackMultiplier(move->getType(), defender);
    
    if (typeMultiplier > 1.0) {
        std::cout << "É super efetivo!\n";
    } else if (typeMultiplier < 1.0 && typeMultiplier > 0) {
        std::cout << "Não é muito efetivo...\n";
    } else if (typeMultiplier == 0) {
        std::cout << "Não afetou " << defender->getName() << "...\n";
        return 0;
    }
    
    // Fórmula de dano simplificada
    int damage = static_cast<int>((((2 * attacker->getLevel() / 5.0 + 2) * 
                                   move->getPower() * attackStat / defenseStat) / 
                                  50.0 + 2) * stab * typeMultiplier);
    
    // Variação aleatória (85%-100%)
    damage = static_cast<int>(damage * Utils::randomDouble(0.85, 1.0));
    
    return std::max(1, damage); // Mínimo 1 de dano
}

double BattleSystem::getAttackMultiplier(const std::string& attackType,
                                        std::shared_ptr<Pokemon> defender) {
    return pokedex->getTypeEffectiveness(attackType, 
                                        defender->getType1(), 
                                        defender->getType2());
}

std::shared_ptr<Move> BattleSystem::chooseCPUMove() {
    auto pokemon = opponent->getActivePokemon();
    auto moves = pokemon->getMoves();
    
    if (moves.empty()) {
        return nullptr;
    }
    
    // IA simples: escolher movimento aleatório
    int index = Utils::randomInt(0, moves.size() - 1);
    return moves[index];
}

int BattleSystem::chooseCPUSwitch() {
    auto team = opponent->getTeam();
    
    // Procurar Pokémon não desmaiado
    std::vector<int> available;
    for (size_t i = 0; i < team.size(); i++) {
        if (!team[i]->getIsFainted() && team[i] != opponent->getActivePokemon()) {
            available.push_back(i);
        }
    }
    
    if (available.empty()) {
        return -1;
    }
    
    // Escolher aleatoriamente
    int randomIndex = Utils::randomInt(0, available.size() - 1);
    return available[randomIndex];
}

void BattleSystem::executePlayerTurn(int moveChoice) {
    auto playerPokemon = player->getActivePokemon();
    auto opponentPokemon = opponent->getActivePokemon();
    
    auto move = playerPokemon->getMove(moveChoice);
    if (!move) {
        std::cout << "Movimento inválido!\n";
        return;
    }
    
    if (move->getCurrentPP() <= 0) {
        std::cout << "Não há mais PP para " << move->getName() << "!\n";
        return;
    }
    
    std::cout << "\n" << playerPokemon->getName() << " usou " << 
        move->getName() << "!\n";
    
    move->use();
    
    if (move->getCategory() != "Status") {
        int damage = calculateDamage(playerPokemon, opponentPokemon, move);
        if (damage > 0) {
            opponentPokemon->takeDamage(damage);
            std::cout << opponentPokemon->getName() << " sofreu " << 
                damage << " de dano!\n";
        }
    }
}

void BattleSystem::executeCPUTurn() {
    auto playerPokemon = player->getActivePokemon();
    auto opponentPokemon = opponent->getActivePokemon();
    
    auto move = chooseCPUMove();
    if (!move) {
        std::cout << opponentPokemon->getName() << " não tem movimentos!\n";
        return;
    }
    
    if (move->getCurrentPP() <= 0) {
        // Tentar trocar de Pokémon
        int switchIndex = chooseCPUSwitch();
        if (switchIndex != -1) {
            opponent->switchPokemon(switchIndex);
        }
        return;
    }
    
    std::cout << "\n" << opponent->getName() << ": " << 
        opponentPokemon->getName() << " usou " << move->getName() << "!\n";
    
    move->use();
    
    if (move->getCategory() != "Status") {
        int damage = calculateDamage(opponentPokemon, playerPokemon, move);
        if (damage > 0) {
            playerPokemon->takeDamage(damage);
            std::cout << playerPokemon->getName() << " sofreu " << 
                damage << " de dano!\n";
        }
    }
}

void BattleSystem::checkBattleEnd() {
    if (!player->hasUsablePokemon()) {
        battleOver = true;
        winner = opponent.get();
        Utils::printColored("\n" + player->getName() + " perdeu a batalha!", 
                          Utils::Colors::RED);
    } else if (!opponent->hasUsablePokemon()) {
        battleOver = true;
        winner = player.get();
        Utils::printColored("\n" + player->getName() + " venceu a batalha!", 
                          Utils::Colors::GREEN);
        awardExperience();
    }
}

void BattleSystem::awardExperience() {
    auto opponentTeam = opponent->getTeam();
    int totalExp = 0;
    
    for (const auto& pokemon : opponentTeam) {
        if (pokemon->getIsFainted()) {
            // Experiência baseada no nível do oponente
            totalExp += pokemon->getLevel() * 100;
        }
    }
    
    // Dividir experiência entre Pokémon não desmaiados do jogador
    auto playerTeam = player->getTeam();
    int usableCount = 0;
    
    for (const auto& pokemon : playerTeam) {
        if (!pokemon->getIsFainted()) {
            usableCount++;
        }
    }
    
    if (usableCount > 0) {
        int expPerPokemon = totalExp / usableCount;
        
        for (auto& pokemon : playerTeam) {
            if (!pokemon->getIsFainted()) {
                pokemon->gainExperience(expPerPokemon);
                while (pokemon->checkLevelUp()) {
                    pokemon->levelUp();
                }
            }
        }
    }
}

void BattleSystem::playerAttack(int moveIndex) {
    if (battleOver) return;
    
    executePlayerTurn(moveIndex);
    
    if (!opponent->getActivePokemon()->getIsFainted()) {
        executeCPUTurn();
    }
    
    checkBattleEnd();
    if (!battleOver) {
        printBattleStatus();
    }
}

void BattleSystem::playerSwitchPokemon(int pokemonIndex) {
    if (battleOver) return;
    
    if (player->switchPokemon(pokemonIndex)) {
        // CPU ataca depois da troca
        executeCPUTurn();
        checkBattleEnd();
        if (!battleOver) {
            printBattleStatus();
        }
    }
}

void BattleSystem::playerUseItem() {
    // Implementação simplificada
    std::cout << "Usou poção! +20 HP\n";
    player->getActivePokemon()->heal(20);
    
    // CPU ataca
    executeCPUTurn();
    checkBattleEnd();
    if (!battleOver) {
        printBattleStatus();
    }
}

bool BattleSystem::playerAttemptRun() {
    // Chance de fuga baseada na velocidade
    auto playerPokemon = player->getActivePokemon();
    auto opponentPokemon = opponent->getActivePokemon();
    
    double escapeChance = playerPokemon->getSpeed() / 
                         (double)(playerPokemon->getSpeed() + opponentPokemon->getSpeed());
    
    if (Utils::chance(escapeChance)) {
        std::cout << "Fuga bem sucedida!\n";
        battleOver = true;
        return true;
    } else {
        std::cout << "Não foi possível fugir!\n";
        
        // CPU ataca
        executeCPUTurn();
        checkBattleEnd();
        if (!battleOver) {
            printBattleStatus();
        }
        return false;
    }
}

void BattleSystem::printBattleStatus() const {
    Utils::printSeparator('=');
    
    // Pokémon do oponente
    auto opponentPokemon = opponent->getActivePokemon();
    std::cout << opponent->getName() << "\n";
    std::cout << opponentPokemon->getName() << " Nv." << 
        opponentPokemon->getLevel() << "\n";
    std::cout << "HP: " << opponentPokemon->getHPBar() << "\n";
    std::cout << opponentPokemon->getCurrentHP() << "/" << 
        opponentPokemon->getMaxHP() << "\n\n";
    
    // Pokémon do jogador
    auto playerPokemon = player->getActivePokemon();
    std::cout << playerPokemon->getName() << " Nv." << 
        playerPokemon->getLevel() << "\n";
    std::cout << "HP: " << playerPokemon->getHPBar() << "\n";
    std::cout << playerPokemon->getCurrentHP() << "/" << 
        playerPokemon->getMaxHP() << "\n";
    
    Utils::printSeparator('-');
}

void BattleSystem::printAvailableMoves() const {
    auto pokemon = player->getActivePokemon();
    auto moves = pokemon->getMoves();
    
    std::cout << "\nMovimentos disponíveis:\n";
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
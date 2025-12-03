#ifndef BATTLESYSTEM_H
#define BATTLESYSTEM_H

#include <memory>

class Trainer;
class Pokemon;
class Move;
class Pokedex;

class BattleSystem {
private:
    std::shared_ptr<Trainer> player;
    std::shared_ptr<Trainer> opponent;
    std::shared_ptr<Pokedex> pokedex;
    
    bool battleOver;
    Trainer* winner;
    
    int calculateDamage(std::shared_ptr<Pokemon> attacker, 
                       std::shared_ptr<Pokemon> defender,
                       std::shared_ptr<Move> move);
    
    double getAttackMultiplier(const std::string& attackType,
                              std::shared_ptr<Pokemon> defender);
    
    std::shared_ptr<Move> chooseCPUMove();
    int chooseCPUSwitch();
    
    void executePlayerTurn(int moveChoice);
    void executeCPUTurn();
    
    void checkBattleEnd();
    void awardExperience();
    
public:
    BattleSystem(std::shared_ptr<Trainer> player, 
                 std::shared_ptr<Trainer> opponent,
                 std::shared_ptr<Pokedex> pokedex);
    
    void startBattle();
    bool isBattleOver() const { return battleOver; }
    Trainer* getWinner() const { return winner; }
    
    void playerAttack(int moveIndex);
    void playerSwitchPokemon(int pokemonIndex);
    void playerUseItem();
    bool playerAttemptRun();
    
    void printBattleStatus() const;
    void printAvailableMoves() const;
    
    std::shared_ptr<Pokemon> getPlayerActive() const;
    std::shared_ptr<Pokemon> getOpponentActive() const;
};

#endif

#ifndef POKEMON_H
#define POKEMON_H

#include <string>
#include <vector>
#include <memory>

class Move;
class Pokedex;

class Pokemon {
private:
    int id;
    std::string name;
    std::string type1;
    std::string type2;
    int baseHP;
    int baseAttack;
    int baseDefense;
    int baseSpAttack;
    int baseSpDefense;
    int baseSpeed;
    
    int level;
    int currentHP;
    int maxHP;
    int attack;
    int defense;
    int spAttack;
    int spDefense;
    int speed;
    
    std::vector<std::shared_ptr<Move>> moves;
    
    bool isFainted;
    int experience;
    int expToNextLevel;
    
    int evolvesIntoId;
    int evolutionLevel;
    
    void calculateStats();
    
public:
    Pokemon(int id, const std::string& name, const std::string& type1, const std::string& type2,
            int baseHP, int baseAttack, int baseDefense, int baseSpAttack, 
            int baseSpDefense, int baseSpeed, int level = 5,
            int evolvesIntoId = 0, int evolutionLevel = 0);
    
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getType1() const { return type1; }
    std::string getType2() const { return type2; }
    int getLevel() const { return level; }
    int getCurrentHP() const { return currentHP; }
    int getMaxHP() const { return maxHP; }
    int getAttack() const { return attack; }
    int getDefense() const { return defense; }
    int getSpAttack() const { return spAttack; }
    int getSpDefense() const { return spDefense; }
    int getSpeed() const { return speed; }
    bool getIsFainted() const { return isFainted; }
    int getExperience() const { return experience; }
    int getExpToNextLevel() const { return expToNextLevel; }
    std::vector<std::shared_ptr<Move>> getMoves() const { return moves; }
    int getEvolvesIntoId() const { return evolvesIntoId; }
    int getEvolutionLevel() const { return evolutionLevel; }
    int getBaseHP() const { return baseHP; }
    int getBaseAttack() const { return baseAttack; }
    int getBaseDefense() const { return baseDefense; }
    int getBaseSpAttack() const { return baseSpAttack; }
    int getBaseSpDefense() const { return baseSpDefense; }
    int getBaseSpeed() const { return baseSpeed; }
    
    void setCurrentHP(int hp);
    void setLevel(int newLevel);
    void setExperience(int exp) { experience = exp; }
    
    bool takeDamage(int damage);
    void heal(int amount);
    void fullHeal();
    void faint();
    void revive();
    
    void gainExperience(int exp);
    bool checkLevelUp();
    void levelUp();
    
    bool canEvolve() const;
    std::shared_ptr<Pokemon> evolve(std::shared_ptr<Pokedex> pokedex);
    
    bool addMove(std::shared_ptr<Move> move);
    bool hasMove(const std::string& moveName) const;
    std::shared_ptr<Move> getMove(int index) const;
    void setMoves(const std::vector<std::shared_ptr<Move>>& newMoves) { moves = newMoves; }
    
    void printStats() const;
    std::string getHPBar() const;
    
    std::shared_ptr<Pokemon> clone() const;
    
    std::string serialize() const;
    static std::shared_ptr<Pokemon> deserialize(const std::string& data, std::shared_ptr<Pokedex> pokedex);
};

#endif

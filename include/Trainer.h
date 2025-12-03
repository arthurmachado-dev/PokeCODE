#ifndef TRAINER_H
#define TRAINER_H

#include <string>
#include <vector>
#include <memory>

class Pokemon;
class Pokedex;

class Trainer {
private:
    std::string name;
    std::vector<std::shared_ptr<Pokemon>> team;
    std::shared_ptr<Pokemon> activePokemon;
    int money;
    bool isPlayer;
    
public:
    Trainer(const std::string& name, bool isPlayer = true);
    
    std::string getName() const { return name; }
    std::vector<std::shared_ptr<Pokemon>> getTeam() const { return team; }
    std::shared_ptr<Pokemon> getActivePokemon() const { return activePokemon; }
    int getMoney() const { return money; }
    bool getIsPlayer() const { return isPlayer; }
    
    void setName(const std::string& newName);
    void setMoney(int amount);
    
    bool addPokemon(std::shared_ptr<Pokemon> pokemon);
    bool removePokemon(int index);
    bool switchPokemon(int index);
    bool hasUsablePokemon() const;
    int getUsablePokemonCount() const;
    bool canAddMorePokemon() const { return team.size() < 6; }
    
    void setActivePokemon(std::shared_ptr<Pokemon> pokemon);
    bool chooseNewActivePokemon();
    
    void printTeam() const;
    void healTeam();
    int getTeamSize() const { return team.size(); }
    
    std::shared_ptr<Trainer> clone() const;
    
    std::string serialize() const;
    static std::shared_ptr<Trainer> deserialize(const std::string& data, std::shared_ptr<Pokedex> pokedex);
};

#endif

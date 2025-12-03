#ifndef POKEDEX_H
#define POKEDEX_H

#include <string>
#include <vector>
#include <map>
#include <memory>

class Pokemon;
class Move;

class Pokedex {
private:
    std::map<int, std::shared_ptr<Pokemon>> pokemonDatabase;
    std::map<int, std::shared_ptr<Move>> moveDatabase;
    std::map<std::string, double> typeEffectiveness;
    
    void initializeTypeChart();
    void loadSampleMoves(std::shared_ptr<Pokemon> pokemon) const;
    
public:
    Pokedex();
    
    bool loadPokemonFromFile(const std::string& filename);
    bool loadMovesFromFile(const std::string& filename);
    
    std::shared_ptr<Pokemon> getPokemonById(int id) const;
    std::shared_ptr<Pokemon> getPokemonByName(const std::string& name) const;
    std::shared_ptr<Move> getMoveById(int id) const;
    std::shared_ptr<Move> getMoveByName(const std::string& name) const;
    
    std::shared_ptr<Pokemon> createPokemon(int id, int level = 5) const;
    std::shared_ptr<Pokemon> createRandomPokemon(int level = 5) const;
    
    double getTypeEffectiveness(const std::string& attackType, 
                                const std::string& defenseType1,
                                const std::string& defenseType2 = "") const;
    
    void printAllPokemon() const;
    void printAllMoves() const;
    int getPokemonCount() const { return pokemonDatabase.size(); }
    int getMoveCount() const { return moveDatabase.size(); }
    
    std::vector<std::shared_ptr<Pokemon>> getStarterTeam() const;
    std::vector<std::shared_ptr<Pokemon>> getCPUTeam() const;
};

#endif


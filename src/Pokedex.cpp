#include "../include/Pokedex.h"
#include "../include/Utils.h"
#include "../include/Pokemon.h"
#include "../include/Move.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>

Pokedex::Pokedex() {
    initializeTypeChart();
}

void Pokedex::initializeTypeChart() {
    typeEffectiveness["Fire-Grass"] = 2.0;
    typeEffectiveness["Water-Fire"] = 2.0;
    typeEffectiveness["Grass-Water"] = 2.0;
    typeEffectiveness["Electric-Water"] = 2.0;
    typeEffectiveness["Electric-Flying"] = 2.0;
    typeEffectiveness["Ice-Grass"] = 2.0;
    typeEffectiveness["Ice-Flying"] = 2.0;
    typeEffectiveness["Ice-Dragon"] = 2.0;
    typeEffectiveness["Fighting-Normal"] = 2.0;
    typeEffectiveness["Fighting-Ice"] = 2.0;
    typeEffectiveness["Fighting-Rock"] = 2.0;
    typeEffectiveness["Poison-Grass"] = 2.0;
    typeEffectiveness["Poison-Fairy"] = 2.0;
    typeEffectiveness["Ground-Fire"] = 2.0;
    typeEffectiveness["Ground-Electric"] = 2.0;
    typeEffectiveness["Ground-Poison"] = 2.0;
    typeEffectiveness["Ground-Rock"] = 2.0;
    typeEffectiveness["Flying-Grass"] = 2.0;
    typeEffectiveness["Flying-Fighting"] = 2.0;
    typeEffectiveness["Flying-Bug"] = 2.0;
    typeEffectiveness["Psychic-Fighting"] = 2.0;
    typeEffectiveness["Psychic-Poison"] = 2.0;
    typeEffectiveness["Bug-Grass"] = 2.0;
    typeEffectiveness["Bug-Psychic"] = 2.0;
    typeEffectiveness["Rock-Fire"] = 2.0;
    typeEffectiveness["Rock-Ice"] = 2.0;
    typeEffectiveness["Rock-Flying"] = 2.0;
    typeEffectiveness["Rock-Bug"] = 2.0;
    typeEffectiveness["Ghost-Psychic"] = 2.0;
    typeEffectiveness["Ghost-Ghost"] = 2.0;
    typeEffectiveness["Dragon-Dragon"] = 2.0;
    
    typeEffectiveness["Fire-Fire"] = 0.5;
    typeEffectiveness["Fire-Water"] = 0.5;
    typeEffectiveness["Fire-Rock"] = 0.5;
    typeEffectiveness["Fire-Dragon"] = 0.5;
    typeEffectiveness["Water-Water"] = 0.5;
    typeEffectiveness["Water-Grass"] = 0.5;
    typeEffectiveness["Water-Dragon"] = 0.5;
    typeEffectiveness["Grass-Grass"] = 0.5;
    typeEffectiveness["Grass-Fire"] = 0.5;
    typeEffectiveness["Grass-Poison"] = 0.5;
    typeEffectiveness["Grass-Flying"] = 0.5;
    typeEffectiveness["Grass-Bug"] = 0.5;
    typeEffectiveness["Grass-Dragon"] = 0.5;
    typeEffectiveness["Electric-Electric"] = 0.5;
    typeEffectiveness["Electric-Grass"] = 0.5;
    typeEffectiveness["Electric-Dragon"] = 0.5;
    typeEffectiveness["Ice-Ice"] = 0.5;
    typeEffectiveness["Ice-Fire"] = 0.5;
    typeEffectiveness["Ice-Water"] = 0.5;
    typeEffectiveness["Ice-Steel"] = 0.5;
    typeEffectiveness["Fighting-Flying"] = 0.5;
    typeEffectiveness["Fighting-Poison"] = 0.5;
    typeEffectiveness["Fighting-Psychic"] = 0.5;
    typeEffectiveness["Fighting-Bug"] = 0.5;
    typeEffectiveness["Fighting-Fairy"] = 0.5;
    typeEffectiveness["Poison-Poison"] = 0.5;
    typeEffectiveness["Poison-Ground"] = 0.5;
    typeEffectiveness["Poison-Rock"] = 0.5;
    typeEffectiveness["Poison-Ghost"] = 0.5;
    
    typeEffectiveness["Normal-Ghost"] = 0.0;
    typeEffectiveness["Ghost-Normal"] = 0.0;
    typeEffectiveness["Electric-Ground"] = 0.0;
    typeEffectiveness["Ground-Flying"] = 0.0;
}

bool Pokedex::loadPokemonFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int count = 0;
    
    while (std::getline(file, line)) {
        Utils::trim(line);
        if (line.empty() || line[0] == '#') continue;
        
        std::vector<std::string> parts = Utils::split(line, ',');
        if (parts.size() < 13) {
            std::cerr << "Linha invalida: " << line << std::endl;
            continue;
        }
        
        try {
            int id = std::stoi(parts[0]);
            std::string name = parts[1];
            std::string type1 = parts[2];
            std::string type2 = parts[3];
            int hp = std::stoi(parts[4]);
            int attack = std::stoi(parts[5]);
            int defense = std::stoi(parts[6]);
            int spAttack = std::stoi(parts[7]);
            int spDefense = std::stoi(parts[8]);
            int speed = std::stoi(parts[9]);
            int level = std::stoi(parts[10]);
            int evolvesIntoId = std::stoi(parts[11]);
            int evolutionLevel = std::stoi(parts[12]);
            
            auto pokemon = std::make_shared<Pokemon>(id, name, type1, type2, 
                                                    hp, attack, defense, spAttack,
                                                    spDefense, speed, level,
                                                    evolvesIntoId, evolutionLevel);
            
            pokemonDatabase[id] = pokemon;
            count++;
            
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar linha: " << line << std::endl;
        }
    }
    
    file.close();
    std::cout << "Carregados " << count << " Pokemon do arquivo." << std::endl;
    return count > 0;
}

bool Pokedex::loadMovesFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Erro ao abrir arquivo: " << filename << std::endl;
        return false;
    }
    
    std::string line;
    int count = 0;
    
    while (std::getline(file, line)) {
        Utils::trim(line);
        if (line.empty() || line[0] == '#') continue;
        
        std::vector<std::string> parts = Utils::split(line, ',');
        if (parts.size() < 7) {
            std::cerr << "Linha invalida: " << line << std::endl;
            continue;
        }
        
        try {
            int id = std::stoi(parts[0]);
            std::string name = parts[1];
            std::string type = parts[2];
            std::string category = parts[3];
            
            int power = 0;
            if (!parts[4].empty()) power = std::stoi(parts[4]);
            
            int accuracy = std::stoi(parts[5]);
            int pp = std::stoi(parts[6]);
            
            auto move = std::make_shared<Move>(id, name, type, category, power, accuracy, pp);
            moveDatabase[id] = move;
            count++;
            
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar linha: " << line << std::endl;
        }
    }
    
    file.close();
    std::cout << "Carregados " << count << " movimentos do arquivo." << std::endl;
    return count > 0;
}

std::shared_ptr<Pokemon> Pokedex::getPokemonById(int id) const {
    auto it = pokemonDatabase.find(id);
    if (it != pokemonDatabase.end()) return it->second->clone();
    return nullptr;
}

std::shared_ptr<Pokemon> Pokedex::getPokemonByName(const std::string& name) const {
    std::string lowerName = Utils::toLower(name);
    for (const auto& pair : pokemonDatabase) {
        std::string pokemonName = Utils::toLower(pair.second->getName());
        if (pokemonName == lowerName) return pair.second->clone();
    }
    return nullptr;
}

std::shared_ptr<Move> Pokedex::getMoveById(int id) const {
    auto it = moveDatabase.find(id);
    if (it != moveDatabase.end()) return it->second->clone();
    return nullptr;
}

std::shared_ptr<Move> Pokedex::getMoveByName(const std::string& name) const {
    std::string lowerName = Utils::toLower(name);
    for (const auto& pair : moveDatabase) {
        std::string moveName = Utils::toLower(pair.second->getName());
        if (moveName == lowerName) return pair.second->clone();
    }
    return nullptr;
}

void Pokedex::loadSampleMoves(std::shared_ptr<Pokemon> pokemon) const {
    std::string type1 = pokemon->getType1();
    int movesAdded = 0;
    
    for (const auto& pair : moveDatabase) {
        if (movesAdded >= 4) break;
        auto move = pair.second;
        if (move->getType() == type1 && move->getPower() > 0) {
            if (pokemon->addMove(move->clone())) movesAdded++;
        }
    }
    
    if (movesAdded < 4) {
        auto tackle = getMoveByName("Tackle");
        if (tackle && !pokemon->hasMove("Tackle")) {
            pokemon->addMove(tackle->clone());
            movesAdded++;
        }
    }
}

std::shared_ptr<Pokemon> Pokedex::createPokemon(int id, int level) const {
    auto basePokemon = getPokemonById(id);
    if (!basePokemon) return nullptr;
    
    auto pokemon = basePokemon->clone();
    pokemon->setLevel(level);
    loadSampleMoves(pokemon);
    return pokemon;
}

std::shared_ptr<Pokemon> Pokedex::createRandomPokemon(int level) const {
    if (pokemonDatabase.empty()) return nullptr;
    
    auto it = pokemonDatabase.begin();
    std::advance(it, Utils::randomInt(0, pokemonDatabase.size() - 1));
    return createPokemon(it->first, level);
}

double Pokedex::getTypeEffectiveness(const std::string& attackType, 
                                    const std::string& defenseType1,
                                    const std::string& defenseType2) const {
    double multiplier = 1.0;
    
    std::string key1 = attackType + "-" + defenseType1;
    auto it1 = typeEffectiveness.find(key1);
    if (it1 != typeEffectiveness.end()) multiplier *= it1->second;
    
    if (!defenseType2.empty()) {
        std::string key2 = attackType + "-" + defenseType2;
        auto it2 = typeEffectiveness.find(key2);
        if (it2 != typeEffectiveness.end()) multiplier *= it2->second;
    }
    
    return multiplier;
}

void Pokedex::printAllPokemon() const {
    std::cout << "\n=== POKEDEX ===\n";
    std::cout << "Total: " << pokemonDatabase.size() << " Pokemon\n\n";
    
    for (const auto& pair : pokemonDatabase) {
        auto pokemon = pair.second;
        std::cout << "#" << pokemon->getId() << " " << pokemon->getName();
        std::cout << " [" << pokemon->getType1();
        if (!pokemon->getType2().empty()) std::cout << "/" << pokemon->getType2();
        std::cout << "] Nv." << pokemon->getLevel();
        if (pokemon->getEvolvesIntoId() > 0) {
            std::cout << " (Evolui no nivel " << pokemon->getEvolutionLevel() << ")";
        }
        std::cout << std::endl;
    }
}

void Pokedex::printAllMoves() const {
    std::cout << "\n=== MOVIMENTOS ===\n";
    std::cout << "Total: " << moveDatabase.size() << " movimentos\n\n";
    
    for (const auto& pair : moveDatabase) {
        auto move = pair.second;
        std::cout << "#" << move->getId() << " " << move->getName();
        std::cout << " [" << move->getType() << ", " << move->getCategory() << "]";
        std::cout << " Poder: " << move->getPower();
        std::cout << " Precisao: " << move->getAccuracy() << "%";
        std::cout << " PP: " << move->getMaxPP() << std::endl;
    }
}

std::vector<std::shared_ptr<Pokemon>> Pokedex::getStarterTeam() const {
    std::vector<std::shared_ptr<Pokemon>> team;
    
    auto bulbasaur = createPokemon(1, 5);
    auto charmander = createPokemon(4, 5);
    auto squirtle = createPokemon(7, 5);
    
    if (bulbasaur) team.push_back(bulbasaur);
    if (charmander) team.push_back(charmander);
    if (squirtle) team.push_back(squirtle);
    
    return team;
}

std::vector<std::shared_ptr<Pokemon>> Pokedex::getCPUTeam() const {
    std::vector<std::shared_ptr<Pokemon>> team;
    
    for (int i = 0; i < 3; i++) {
        auto pokemon = createRandomPokemon(Utils::randomInt(10, 15));
        if (pokemon) team.push_back(pokemon);
    }
    
    return team;
}

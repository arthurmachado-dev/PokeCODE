#include "../include/Pokedex.h"
#include "../include/Utils.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <random>

Pokedex::Pokedex() {
    initializeTypeChart();
}

void Pokedex::initializeTypeChart() {
    // Mapa de efetividade de tipos
    // Chave: "attack-defense", Valor: multiplicador
    
    // Efetivo (2x)
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
    typeEffectiveness["Dark-Psychic"] = 2.0;
    typeEffectiveness["Dark-Ghost"] = 2.0;
    typeEffectiveness["Steel-Ice"] = 2.0;
    typeEffectiveness["Steel-Rock"] = 2.0;
    typeEffectiveness["Steel-Fairy"] = 2.0;
    typeEffectiveness["Fairy-Fighting"] = 2.0;
    typeEffectiveness["Fairy-Dragon"] = 2.0;
    typeEffectiveness["Fairy-Dark"] = 2.0;
    
    // Não efetivo (0.5x)
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
    typeEffectiveness["Ground-Grass"] = 0.5;
    typeEffectiveness["Ground-Bug"] = 0.5;
    typeEffectiveness["Flying-Electric"] = 0.5;
    typeEffectiveness["Flying-Rock"] = 0.5;
    typeEffectiveness["Flying-Steel"] = 0.5;
    typeEffectiveness["Psychic-Psychic"] = 0.5;
    typeEffectiveness["Psychic-Steel"] = 0.5;
    typeEffectiveness["Bug-Fire"] = 0.5;
    typeEffectiveness["Bug-Fighting"] = 0.5;
    typeEffectiveness["Bug-Poison"] = 0.5;
    typeEffectiveness["Bug-Flying"] = 0.5;
    typeEffectiveness["Bug-Ghost"] = 0.5;
    typeEffectiveness["Bug-Steel"] = 0.5;
    typeEffectiveness["Bug-Fairy"] = 0.5;
    typeEffectiveness["Rock-Fighting"] = 0.5;
    typeEffectiveness["Rock-Ground"] = 0.5;
    typeEffectiveness["Rock-Steel"] = 0.5;
    typeEffectiveness["Ghost-Dark"] = 0.5;
    typeEffectiveness["Dragon-Steel"] = 0.5;
    typeEffectiveness["Dark-Fighting"] = 0.5;
    typeEffectiveness["Dark-Dark"] = 0.5;
    typeEffectiveness["Dark-Fairy"] = 0.5;
    typeEffectiveness["Steel-Steel"] = 0.5;
    typeEffectiveness["Steel-Fire"] = 0.5;
    typeEffectiveness["Steel-Water"] = 0.5;
    typeEffectiveness["Steel-Electric"] = 0.5;
    typeEffectiveness["Fairy-Fire"] = 0.5;
    typeEffectiveness["Fairy-Poison"] = 0.5;
    typeEffectiveness["Fairy-Steel"] = 0.5;
    
    // Imune (0x)
    typeEffectiveness["Normal-Ghost"] = 0.0;
    typeEffectiveness["Ghost-Normal"] = 0.0;
    typeEffectiveness["Electric-Ground"] = 0.0;
    typeEffectiveness["Ground-Flying"] = 0.0;
    typeEffectiveness["Fighting-Ghost"] = 0.0;
    typeEffectiveness["Poison-Steel"] = 0.0;
    typeEffectiveness["Psychic-Dark"] = 0.0;
    typeEffectiveness["Ghost-Normal"] = 0.0;
    typeEffectiveness["Dragon-Fairy"] = 0.0;
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
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::vector<std::string> parts = Utils::split(line, ',');
        if (parts.size() < 11) {
            std::cerr << "Linha inválida: " << line << std::endl;
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
            
            auto pokemon = std::make_shared<Pokemon>(id, name, type1, type2, 
                                                    hp, attack, defense, spAttack,
                                                    spDefense, speed, level);
            
            pokemonDatabase[id] = pokemon;
            count++;
            
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar linha: " << line << " - " << e.what() << std::endl;
        }
    }
    
    file.close();
    std::cout << "Carregados " << count << " Pokémon do arquivo." << std::endl;
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
        if (line.empty() || line[0] == '#') {
            continue;
        }
        
        std::vector<std::string> parts = Utils::split(line, ',');
        if (parts.size() < 7) {
            std::cerr << "Linha inválida: " << line << std::endl;
            continue;
        }
        
        try {
            int id = std::stoi(parts[0]);
            std::string name = parts[1];
            std::string type = parts[2];
            std::string category = parts[3];
            int power = std::stoi(parts[4]);
            int accuracy = std::stoi(parts[5]);
            int pp = std::stoi(parts[6]);
            
            auto move = std::make_shared<Move>(id, name, type, category, 
                                              power, accuracy, pp);
            
            moveDatabase[id] = move;
            count++;
            
        } catch (const std::exception& e) {
            std::cerr << "Erro ao processar linha: " << line << " - " << e.what() << std::endl;
        }
    }
    
    file.close();
    std::cout << "Carregados " << count << " movimentos do arquivo." << std::endl;
    return count > 0;
}

std::shared_ptr<Pokemon> Pokedex::getPokemonById(int id) const {
    auto it = pokemonDatabase.find(id);
    if (it != pokemonDatabase.end()) {
        return it->second->clone();
    }
    return nullptr;
}

std::shared_ptr<Pokemon> Pokedex::getPokemonByName(const std::string& name) const {
    std::string lowerName = Utils::toLower(name);
    
    for (const auto& pair : pokemonDatabase) {
        std::string pokemonName = Utils::toLower(pair.second->getName());
        if (pokemonName == lowerName) {
            return pair.second->clone();
        }
    }
    return nullptr;
}

std::shared_ptr<Move> Pokedex::getMoveById(int id) const {
    auto it = moveDatabase.find(id);
    if (it != moveDatabase.end()) {
        return it->second->clone();
    }
    return nullptr;
}

std::shared_ptr<Move> Pokedex::getMoveByName(const std::string& name) const {
    std::string lowerName = Utils::toLower(name);
    
    for (const auto& pair : moveDatabase) {
        std::string moveName = Utils::toLower(pair.second->getName());
        if (moveName == lowerName) {
            return pair.second->clone();
        }
    }
    return nullptr;
}

// CORREÇÃO: Método agora é const
void Pokedex::loadSampleMoves(std::shared_ptr<Pokemon> pokemon) const {
    // Adicionar movimentos baseados no tipo do Pokémon
    std::string type1 = pokemon->getType1();
    
    // Limitar a 4 movimentos
    int movesAdded = 0;
    
    // Adicionar movimentos do tipo do Pokémon
    for (const auto& pair : moveDatabase) {
        if (movesAdded >= 4) break;
        
        auto move = pair.second;
        if (move->getType() == type1 && move->getPower() > 0) {
            if (pokemon->addMove(move->clone())) {
                movesAdded++;
            }
        }
    }
    
    // Se não tiver movimentos suficientes, adicionar Tackle
    if (movesAdded < 4) {
        auto tackle = getMoveByName("Tackle");
        if (tackle && !pokemon->hasMove("Tackle")) {
            pokemon->addMove(tackle->clone());
            movesAdded++;
        }
    }
    
    // Se ainda não tiver movimentos suficientes, adicionar movimentos aleatórios
    if (movesAdded < 4) {
        std::vector<std::shared_ptr<Move>> availableMoves;
        for (const auto& pair : moveDatabase) {
            if (pair.second->getPower() > 0) {
                availableMoves.push_back(pair.second);
            }
        }
        
        std::random_shuffle(availableMoves.begin(), availableMoves.end());
        
        for (const auto& move : availableMoves) {
            if (movesAdded >= 4) break;
            if (!pokemon->hasMove(move->getName())) {
                if (pokemon->addMove(move->clone())) {
                    movesAdded++;
                }
            }
        }
    }
}

std::shared_ptr<Pokemon> Pokedex::createPokemon(int id, int level) const {
    auto basePokemon = getPokemonById(id);
    if (!basePokemon) {
        return nullptr;
    }
    
    // Criar clone e ajustar nível
    auto pokemon = basePokemon->clone();
    pokemon->setLevel(level);
    
    // Adicionar movimentos
    loadSampleMoves(pokemon);
    
    return pokemon;
}

std::shared_ptr<Pokemon> Pokedex::createRandomPokemon(int level) const {
    if (pokemonDatabase.empty()) {
        return nullptr;
    }
    
    // Escolher Pokémon aleatório
    auto it = pokemonDatabase.begin();
    std::advance(it, Utils::randomInt(0, pokemonDatabase.size() - 1));
    
    return createPokemon(it->first, level);
}

double Pokedex::getTypeEffectiveness(const std::string& attackType, 
                                    const std::string& defenseType1,
                                    const std::string& defenseType2) const {
    double multiplier = 1.0;
    
    // Verificar contra primeiro tipo
    std::string key1 = attackType + "-" + defenseType1;
    auto it1 = typeEffectiveness.find(key1);
    if (it1 != typeEffectiveness.end()) {
        multiplier *= it1->second;
    }
    
    // Verificar contra segundo tipo se existir
    if (!defenseType2.empty()) {
        std::string key2 = attackType + "-" + defenseType2;
        auto it2 = typeEffectiveness.find(key2);
        if (it2 != typeEffectiveness.end()) {
            multiplier *= it2->second;
        }
    }
    
    return multiplier;
}

void Pokedex::printAllPokemon() const {
    Utils::printColored("\n=== POKÉDEX ===", Utils::Colors::CYAN);
    std::cout << "Total: " << pokemonDatabase.size() << " Pokémon\n\n";
    
    for (const auto& pair : pokemonDatabase) {
        auto pokemon = pair.second;
        std::cout << "#" << pokemon->getId() << " " << pokemon->getName();
        std::cout << " [" << pokemon->getType1();
        if (!pokemon->getType2().empty()) {
            std::cout << "/" << pokemon->getType2();
        }
        std::cout << "] Nv." << pokemon->getLevel() << std::endl;
    }
}

void Pokedex::printAllMoves() const {
    Utils::printColored("\n=== MOVIMENTOS ===", Utils::Colors::CYAN);
    std::cout << "Total: " << moveDatabase.size() << " movimentos\n\n";
    
    for (const auto& pair : moveDatabase) {
        auto move = pair.second;
        std::cout << "#" << move->getId() << " " << move->getName();
        std::cout << " [" << move->getType() << ", " << move->getCategory() << "]";
        std::cout << " Poder: " << move->getPower();
        std::cout << " Precisão: " << move->getAccuracy() << "%";
        std::cout << " PP: " << move->getMaxPP() << std::endl;
    }
}

std::vector<std::shared_ptr<Pokemon>> Pokedex::getStarterTeam() const {
    std::vector<std::shared_ptr<Pokemon>> team;
    
    // Time inicial: Bulbasaur, Charmander, Squirtle
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
    
    // Time da CPU: Pokémon aleatórios nível 10-15
    for (int i = 0; i < 3; i++) {
        auto pokemon = createRandomPokemon(Utils::randomInt(10, 15));
        if (pokemon) {
            team.push_back(pokemon);
        }
    }
    
    return team;
}
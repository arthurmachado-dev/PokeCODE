#include "../include/Pokemon.h"
#include "../include/Utils.h"
#include "../include/Pokedex.h"
#include "../include/Move.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>

Pokemon::Pokemon(int id, const std::string& name, const std::string& type1, 
                 const std::string& type2, int baseHP, int baseAttack, int baseDefense,
                 int baseSpAttack, int baseSpDefense, int baseSpeed, int level,
                 int evolvesIntoId, int evolutionLevel)
    : id(id), name(name), type1(type1), type2(type2), baseHP(baseHP),
      baseAttack(baseAttack), baseDefense(baseDefense), baseSpAttack(baseSpAttack),
      baseSpDefense(baseSpDefense), baseSpeed(baseSpeed), level(level),
      currentHP(0), maxHP(0), attack(0), defense(0), spAttack(0),
      spDefense(0), speed(0), isFainted(false), experience(0),
      evolvesIntoId(evolvesIntoId), evolutionLevel(evolutionLevel) {
    
    calculateStats();
    currentHP = maxHP;
    expToNextLevel = static_cast<int>(std::pow(level, 3) * 0.8);
}

void Pokemon::calculateStats() {
    maxHP = ((2 * baseHP + 31) * level) / 100 + level + 10;
    attack = ((2 * baseAttack + 31) * level) / 100 + 5;
    defense = ((2 * baseDefense + 31) * level) / 100 + 5;
    spAttack = ((2 * baseSpAttack + 31) * level) / 100 + 5;
    spDefense = ((2 * baseSpDefense + 31) * level) / 100 + 5;
    speed = ((2 * baseSpeed + 31) * level) / 100 + 5;
}

void Pokemon::setCurrentHP(int hp) {
    currentHP = std::max(0, std::min(hp, maxHP));
    if (currentHP == 0) {
        isFainted = true;
    }
}

void Pokemon::setLevel(int newLevel) {
    level = std::max(1, std::min(newLevel, 100));
    calculateStats();
    currentHP = std::min(currentHP, maxHP);
    expToNextLevel = static_cast<int>(std::pow(level, 3) * 0.8);
}

bool Pokemon::takeDamage(int damage) {
    if (damage < 0) return false;
    
    currentHP -= damage;
    if (currentHP < 0) currentHP = 0;
    
    if (currentHP == 0) {
        faint();
        return true; // Desmaiou
    }
    return false; // Ainda consciente
}

void Pokemon::heal(int amount) {
    currentHP = std::min(currentHP + amount, maxHP);
    if (currentHP > 0 && isFainted) {
        isFainted = false;
    }
}

void Pokemon::fullHeal() {
    currentHP = maxHP;
    isFainted = false;
    for (auto& move : moves) {
        move->restoreAllPP();
    }
}

void Pokemon::faint() {
    if (!isFainted) {
        isFainted = true;
        std::cout << "\n" << Utils::Colors::RED << name << " desmaiou!" << Utils::Colors::RESET << std::endl;
    }
}

void Pokemon::revive() {
    if (isFainted) {
        currentHP = maxHP / 2;
        isFainted = false;
    }
}

void Pokemon::gainExperience(int exp) {
    experience += exp;
    
    while (checkLevelUp()) {
        levelUp();
    }
}

bool Pokemon::checkLevelUp() {
    return experience >= expToNextLevel;
}

void Pokemon::levelUp() {
    if (checkLevelUp()) {
        int oldLevel = level;
        
        while (experience >= expToNextLevel && level < 100) {
            level++;
            experience -= expToNextLevel;
            expToNextLevel = static_cast<int>(std::pow(level, 3) * 0.8);
        }
        
        int oldHP = currentHP;
        int oldMaxHP = maxHP;
        
        calculateStats();
        
        if (oldMaxHP > 0) {
            float hpRatio = static_cast<float>(oldHP) / oldMaxHP;
            currentHP = static_cast<int>(maxHP * hpRatio);
        }
        if (currentHP == 0 && !isFainted) {
            currentHP = 1;
        }
        
        if (level > oldLevel) {
            Utils::printColored("\n" + name + " subiu para o nivel " + 
                               std::to_string(level) + "!", Utils::Colors::GREEN);
        }
    }
}

bool Pokemon::canEvolve() const {
    return evolvesIntoId > 0 && level >= evolutionLevel;
}

std::shared_ptr<Pokemon> Pokemon::evolve(std::shared_ptr<Pokedex> pokedex) {
    if (!canEvolve()) {
        return nullptr;
    }
    
    auto evolvedPokemon = pokedex->getPokemonById(evolvesIntoId);
    if (!evolvedPokemon) {
        return nullptr;
    }
    
    evolvedPokemon->setLevel(level);
    evolvedPokemon->setExperience(experience);
    evolvedPokemon->setMoves(moves);
    
    float hpRatio = static_cast<float>(currentHP) / maxHP;
    evolvedPokemon->setCurrentHP(static_cast<int>(evolvedPokemon->getMaxHP() * hpRatio));
    
    std::cout << "\n" << Utils::Colors::MAGENTA;
    std::cout << "Parabens! " << name << " evoluiu para " << evolvedPokemon->getName() << "!\n";
    std::cout << Utils::Colors::RESET;
    
    return evolvedPokemon;
}

bool Pokemon::addMove(std::shared_ptr<Move> move) {
    if (moves.size() >= 4) return false;
    
    for (const auto& existingMove : moves) {
        if (existingMove->getName() == move->getName()) return false;
    }
    
    moves.push_back(move);
    return true;
}

bool Pokemon::hasMove(const std::string& moveName) const {
    for (const auto& move : moves) {
        if (move->getName() == moveName) return true;
    }
    return false;
}

std::shared_ptr<Move> Pokemon::getMove(int index) const {
    if (index >= 0 && index < static_cast<int>(moves.size())) {
        return moves[index];
    }
    return nullptr;
}

void Pokemon::printStats() const {
    std::cout << "\n=== " << name << " ===\n";
    std::cout << "Nivel: " << level;
    if (canEvolve()) {
        std::cout << " (Pronto para evoluir!)";
    }
    std::cout << "\nHP: " << getHPBar() << " " << currentHP << "/" << maxHP;
    if (isFainted) {
        std::cout << " [DESMAIADO]";
    }
    std::cout << "\nTipo: " << type1;
    if (!type2.empty()) std::cout << "/" << type2;
    std::cout << "\nExperiencia: " << experience << "/" << expToNextLevel;
    std::cout << "\nAtaque: " << attack;
    std::cout << "\nDefesa: " << defense;
    std::cout << "\nVelocidade: " << speed;
    
    if (!moves.empty()) {
        std::cout << "\n\nMovimentos:\n";
        for (size_t i = 0; i < moves.size(); i++) {
            std::cout << "  " << (i + 1) << ". " << moves[i]->getName();
            std::cout << " (" << moves[i]->getCurrentPP() << "/" << moves[i]->getMaxPP() << " PP)\n";
        }
    }
    std::cout << "==========================\n";
}

std::string Pokemon::getHPBar() const {
    int barWidth = 20;
    float percentage = static_cast<float>(currentHP) / maxHP;
    int filledWidth = static_cast<int>(percentage * barWidth);
    
    std::string bar;
    
    if (percentage > 0.5) bar += Utils::Colors::GREEN;
    else if (percentage > 0.2) bar += Utils::Colors::YELLOW;
    else bar += Utils::Colors::RED;
    
    for (int i = 0; i < barWidth; i++) {
        if (i < filledWidth) bar += "#";
        else bar += ".";
    }
    
    bar += Utils::Colors::RESET;
    return bar;
}

std::shared_ptr<Pokemon> Pokemon::clone() const {
    auto clone = std::make_shared<Pokemon>(id, name, type1, type2, baseHP, 
                                           baseAttack, baseDefense, baseSpAttack,
                                           baseSpDefense, baseSpeed, level,
                                           evolvesIntoId, evolutionLevel);
    
    clone->currentHP = currentHP;
    clone->maxHP = maxHP;
    clone->attack = attack;
    clone->defense = defense;
    clone->spAttack = spAttack;
    clone->spDefense = spDefense;
    clone->speed = speed;
    clone->isFainted = isFainted;
    clone->experience = experience;
    clone->expToNextLevel = expToNextLevel;
    
    for (const auto& move : moves) {
        clone->moves.push_back(move->clone());
    }
    
    return clone;
}

std::string Pokemon::serialize() const {
    std::stringstream ss;
    ss << id << "," << name << "," << type1 << "," << type2 << ",";
    ss << level << "," << currentHP << "," << maxHP << ",";
    ss << experience << "," << evolvesIntoId << "," << evolutionLevel;
    
    ss << "," << moves.size();
    for (const auto& move : moves) {
        ss << "," << move->getName();
    }
    
    return ss.str();
}

std::shared_ptr<Pokemon> Pokemon::deserialize(const std::string& data, std::shared_ptr<Pokedex> pokedex) {
    std::vector<std::string> parts = Utils::split(data, ',');
    if (parts.size() < 10) return nullptr;
    
    try {
        int id = std::stoi(parts[0]);
        std::string name = parts[1];
        std::string type1 = parts[2];
        std::string type2 = parts[3];
        int level = std::stoi(parts[4]);
        int currentHP = std::stoi(parts[5]);
        int maxHP = std::stoi(parts[6]);
        int experience = std::stoi(parts[7]);
        int evolvesIntoId = std::stoi(parts[8]);
        int evolutionLevel = std::stoi(parts[9]);
        
        auto basePokemon = pokedex->getPokemonById(id);
        if (!basePokemon) return nullptr;
        
        auto pokemon = std::make_shared<Pokemon>(id, name, type1, type2,
            basePokemon->getBaseHP(), basePokemon->getBaseAttack(), basePokemon->getBaseDefense(),
            basePokemon->getBaseSpAttack(), basePokemon->getBaseSpDefense(), basePokemon->getBaseSpeed(),
            level, evolvesIntoId, evolutionLevel);
        
        pokemon->currentHP = currentHP;
        pokemon->maxHP = maxHP;
        pokemon->experience = experience;
        
        if (parts.size() > 10) {
            int numMoves = std::stoi(parts[10]);
            for (int i = 0; i < numMoves && (11 + i) < parts.size(); i++) {
                std::string moveName = parts[11 + i];
                auto move = pokedex->getMoveByName(moveName);
                if (move) pokemon->addMove(move);
            }
        }
        
        return pokemon;
        
    } catch (const std::exception& e) {
        return nullptr;
    }
}

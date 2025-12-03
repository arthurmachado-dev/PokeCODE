#include "../include/Pokemon.h"
#include "../include/Utils.h"
#include <iostream>
#include <algorithm>
#include <cmath>
#include <sstream>

Pokemon::Pokemon(int id, const std::string& name, const std::string& type1, 
                 const std::string& type2, int baseHP, int baseAttack, int baseDefense,
                 int baseSpAttack, int baseSpDefense, int baseSpeed, int level)
    : id(id), name(name), type1(type1), type2(type2), baseHP(baseHP),
      baseAttack(baseAttack), baseDefense(baseDefense), baseSpAttack(baseSpAttack),
      baseSpDefense(baseSpDefense), baseSpeed(baseSpeed), level(level),
      currentHP(0), maxHP(0), attack(0), defense(0), spAttack(0),
      spDefense(0), speed(0), isFainted(false), experience(0) {
    
    calculateStats();
    currentHP = maxHP;
    
    // Calcular experiência para próximo nível
    expToNextLevel = static_cast<int>(std::pow(level, 3) * 0.8);
}

void Pokemon::calculateStats() {
    // HP = ((2 * Base + IV) * Level) / 100 + Level + 10
    maxHP = ((2 * baseHP + 31) * level) / 100 + level + 10;
    
    // Outras stats = ((2 * Base + IV) * Level) / 100 + 5
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
}

bool Pokemon::takeDamage(int damage) {
    currentHP -= damage;
    if (currentHP < 0) {
        currentHP = 0;
    }
    
    if (currentHP == 0) {
        faint();
        return true;
    }
    return false;
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
    
    // Restaurar PP dos movimentos
    for (auto& move : moves) {
        move->restoreAllPP();
    }
}

void Pokemon::faint() {
    isFainted = true;
    std::cout << name << " desmaiou!\n";
}

void Pokemon::revive() {
    if (isFainted) {
        currentHP = maxHP / 2;
        isFainted = false;
        std::cout << name << " foi revivido!\n";
    }
}

void Pokemon::gainExperience(int exp) {
    experience += exp;
    std::cout << name << " ganhou " << exp << " pontos de experiência!\n";
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
        
        // Salvar HP atual antes do cálculo
        int oldHP = currentHP;
        int oldMaxHP = maxHP;
        
        // Recalcular stats
        calculateStats();
        
        // Restaurar HP proporcionalmente
        if (oldMaxHP > 0) {
            float hpRatio = static_cast<float>(oldHP) / oldMaxHP;
            currentHP = static_cast<int>(maxHP * hpRatio);
        }
        if (currentHP == 0 && !isFainted) {
            currentHP = 1;
        }
        
        if (level > oldLevel) {
            Utils::printColored("\n" + name + " subiu para o nível " + 
                               std::to_string(level) + "!", Utils::Colors::GREEN);
        }
    }
}

bool Pokemon::addMove(std::shared_ptr<Move> move) {
    if (moves.size() >= 4) {
        return false;
    }
    
    // Verificar se o movimento já existe
    for (const auto& existingMove : moves) {
        if (existingMove->getName() == move->getName()) {
            return false;
        }
    }
    
    moves.push_back(move);
    return true;
}

bool Pokemon::hasMove(const std::string& moveName) const {
    for (const auto& move : moves) {
        if (move->getName() == moveName) {
            return true;
        }
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
    Utils::printSeparator('=');
    std::cout << Utils::Colors::BOLD << name << Utils::Colors::RESET << " ";
    std::cout << "Nv." << level << std::endl;
    
    std::cout << "HP: " << getHPBar() << std::endl;
    std::cout << "   " << currentHP << "/" << maxHP << std::endl;
    
    std::cout << "\nTipo: " << Utils::Colors::CYAN << type1;
    if (!type2.empty()) {
        std::cout << "/" << type2;
    }
    std::cout << Utils::Colors::RESET << std::endl;
    
    std::cout << "\nEstatísticas:" << std::endl;
    std::cout << "  Ataque: " << attack << std::endl;
    std::cout << "  Defesa: " << defense << std::endl;
    std::cout << "  Sp.Ataque: " << spAttack << std::endl;
    std::cout << "  Sp.Defesa: " << spDefense << std::endl;
    std::cout << "  Velocidade: " << speed << std::endl;
    
    if (!moves.empty()) {
        std::cout << "\nMovimentos:" << std::endl;
        for (size_t i = 0; i < moves.size(); i++) {
            std::cout << "  " << (i + 1) << ". " << moves[i]->getName();
            std::cout << " (" << moves[i]->getCurrentPP() << "/" << moves[i]->getMaxPP() << " PP)";
            std::cout << std::endl;
        }
    }
    Utils::printSeparator('=');
}

std::string Pokemon::getHPBar() const {
    int barWidth = 20;
    float percentage = static_cast<float>(currentHP) / maxHP;
    int filledWidth = static_cast<int>(percentage * barWidth);
    
    std::string bar;
    
    // Escolher cor baseada no HP
    if (percentage > 0.5) {
        bar += Utils::Colors::GREEN;
    } else if (percentage > 0.2) {
        bar += Utils::Colors::YELLOW;
    } else {
        bar += Utils::Colors::RED;
    }
    
    // Criar barra
    for (int i = 0; i < barWidth; i++) {
        if (i < filledWidth) {
            bar += "█";
        } else {
            bar += "░";
        }
    }
    
    bar += Utils::Colors::RESET;
    return bar;
}

std::shared_ptr<Pokemon> Pokemon::clone() const {
    auto clone = std::make_shared<Pokemon>(id, name, type1, type2, baseHP, 
                                           baseAttack, baseDefense, baseSpAttack,
                                           baseSpDefense, baseSpeed, level);
    
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
    
    // Clonar movimentos
    for (const auto& move : moves) {
        clone->moves.push_back(move->clone());
    }
    
    return clone;
}
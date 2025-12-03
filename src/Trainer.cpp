#include "../include/Trainer.h"
#include "../include/Utils.h"
#include "../include/Pokemon.h"
#include "../include/Pokedex.h"
#include <iostream>
#include <algorithm>
#include <sstream>

Trainer::Trainer(const std::string& name, bool isPlayer)
    : name(name), money(1000), isPlayer(isPlayer), activePokemon(nullptr) {}

void Trainer::setName(const std::string& newName) {
    name = newName;
}

void Trainer::setMoney(int amount) {
    money = std::max(0, amount);
}

bool Trainer::addPokemon(std::shared_ptr<Pokemon> pokemon) {
    if (team.size() < 6) {
        team.push_back(pokemon);
        
        if (!activePokemon && !pokemon->getIsFainted()) {
            activePokemon = pokemon;
        }
        
        return true;
    }
    return false;
}

bool Trainer::removePokemon(int index) {
    if (index >= 0 && index < static_cast<int>(team.size())) {
        if (team[index] == activePokemon) {
            team.erase(team.begin() + index);
            chooseNewActivePokemon();
        } else {
            team.erase(team.begin() + index);
        }
        return true;
    }
    return false;
}

bool Trainer::switchPokemon(int index) {
    if (index >= 0 && index < static_cast<int>(team.size())) {
        auto pokemon = team[index];
        
        if (pokemon->getIsFainted()) {
            std::cout << pokemon->getName() << " esta desmaiado!\n";
            return false;
        }
        
        if (pokemon == activePokemon) {
            std::cout << pokemon->getName() << " ja esta em batalha!\n";
            return false;
        }
        
        activePokemon = pokemon;
        std::cout << "Vai, " << pokemon->getName() << "!\n";
        return true;
    }
    return false;
}

bool Trainer::hasUsablePokemon() const {
    for (const auto& pokemon : team) {
        if (!pokemon->getIsFainted()) return true;
    }
    return false;
}

int Trainer::getUsablePokemonCount() const {
    int count = 0;
    for (const auto& pokemon : team) {
        if (!pokemon->getIsFainted()) count++;
    }
    return count;
}

void Trainer::setActivePokemon(std::shared_ptr<Pokemon> pokemon) {
    activePokemon = pokemon;
}

bool Trainer::chooseNewActivePokemon() {
    for (size_t i = 0; i < team.size(); i++) {
        if (!team[i]->getIsFainted() && team[i] != activePokemon) {
            activePokemon = team[i];
            return true;
        }
    }
    return false;
}

void Trainer::printTeam() const {
    std::cout << "\n=== Time de " << name << " ===\n";
    
    if (team.empty()) {
        std::cout << "Time vazio!\n";
        return;
    }
    
    int usable = getUsablePokemonCount();
    std::cout << "Pokemon disponiveis: " << usable << "/" << team.size() << "\n\n";
    
    for (size_t i = 0; i < team.size(); i++) {
        std::cout << (i + 1) << ". ";
        
        if (team[i] == activePokemon) {
            std::cout << Utils::Colors::GREEN << "[ATIVO] " << Utils::Colors::RESET;
        }
        
        if (team[i]->getIsFainted()) {
            std::cout << Utils::Colors::RED;
        }
        
        std::cout << team[i]->getName();
        std::cout << " Nv." << team[i]->getLevel();
        std::cout << " HP: " << team[i]->getHPBar();
        std::cout << " (" << team[i]->getCurrentHP() << "/" << team[i]->getMaxHP() << ")";
        std::cout << Utils::Colors::RESET;
        
        if (team[i]->canEvolve()) {
            std::cout << " [PRONTO PARA EVOLUIR]";
        }
        
        std::cout << "\n   Tipo: " << team[i]->getType1();
        if (!team[i]->getType2().empty()) {
            std::cout << "/" << team[i]->getType2();
        }
        std::cout << std::endl;
    }
}

void Trainer::healTeam() {
    for (auto& pokemon : team) {
        pokemon->fullHeal();
    }
    std::cout << "Time curado completamente!\n";
}

std::shared_ptr<Trainer> Trainer::clone() const {
    auto clone = std::make_shared<Trainer>(name, isPlayer);
    clone->money = money;
    
    for (const auto& pokemon : team) {
        clone->team.push_back(pokemon->clone());
    }
    
    if (activePokemon) {
        for (size_t i = 0; i < team.size(); i++) {
            if (team[i] == activePokemon) {
                clone->activePokemon = clone->team[i];
                break;
            }
        }
    }
    
    return clone;
}

std::string Trainer::serialize() const {
    std::stringstream ss;
    ss << name << "\n";
    ss << money << "\n";
    ss << isPlayer << "\n";
    ss << team.size() << "\n";
    
    for (const auto& pokemon : team) {
        ss << pokemon->serialize() << "\n";
    }
    
    if (activePokemon) {
        for (size_t i = 0; i < team.size(); i++) {
            if (team[i] == activePokemon) {
                ss << i << "\n";
                break;
            }
        }
    } else {
        ss << "-1\n";
    }
    
    return ss.str();
}

std::shared_ptr<Trainer> Trainer::deserialize(const std::string& data, std::shared_ptr<Pokedex> pokedex) {
    std::vector<std::string> lines = Utils::split(data, '\n');
    if (lines.size() < 5) return nullptr;
    
    try {
        std::string name = lines[0];
        int money = std::stoi(lines[1]);
        bool isPlayer = std::stoi(lines[2]) != 0;
        int teamSize = std::stoi(lines[3]);
        
        auto trainer = std::make_shared<Trainer>(name, isPlayer);
        trainer->setMoney(money);
        
        int lineIndex = 4;
        for (int i = 0; i < teamSize && lineIndex < lines.size(); i++) {
            auto pokemon = Pokemon::deserialize(lines[lineIndex], pokedex);
            if (pokemon) trainer->addPokemon(pokemon);
            lineIndex++;
        }
        
        if (lineIndex < lines.size()) {
            int activeIndex = std::stoi(lines[lineIndex]);
            if (activeIndex >= 0 && activeIndex < trainer->getTeamSize()) {
                trainer->setActivePokemon(trainer->getTeam()[activeIndex]);
            }
        }
        
        return trainer;
        
    } catch (const std::exception& e) {
        return nullptr;
    }
}

#include "../include/Trainer.h"
#include "../include/Utils.h"
#include <iostream>
#include <algorithm>

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
        
        // Definir primeiro Pokémon como ativo se não houver um
        if (!activePokemon) {
            activePokemon = pokemon;
        }
        
        return true;
    }
    return false;
}

bool Trainer::removePokemon(int index) {
    if (index >= 0 && index < static_cast<int>(team.size())) {
        // Se estiver removendo o Pokémon ativo, escolher um novo
        if (team[index] == activePokemon) {
            team.erase(team.begin() + index);
            if (!team.empty()) {
                activePokemon = team[0];
            } else {
                activePokemon = nullptr;
            }
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
        
        // Verificar se o Pokémon está desmaiado
        if (pokemon->getIsFainted()) {
            std::cout << pokemon->getName() << " está desmaiado e não pode entrar na batalha!\n";
            return false;
        }
        
        // Verificar se já é o Pokémon ativo
        if (pokemon == activePokemon) {
            std::cout << pokemon->getName() << " já está em batalha!\n";
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
        if (!pokemon->getIsFainted()) {
            return true;
        }
    }
    return false;
}

void Trainer::setActivePokemon(std::shared_ptr<Pokemon> pokemon) {
    activePokemon = pokemon;
}

bool Trainer::chooseNewActivePokemon() {
    for (size_t i = 0; i < team.size(); i++) {
        if (!team[i]->getIsFainted() && team[i] != activePokemon) {
            activePokemon = team[i];
            std::cout << name << " enviou " << activePokemon->getName() << "!\n";
            return true;
        }
    }
    return false;
}

void Trainer::printTeam() const {
    Utils::printColored("\n=== Time de " + name + " ===", Utils::Colors::CYAN);
    
    if (team.empty()) {
        std::cout << "Time vazio!\n";
        return;
    }
    
    for (size_t i = 0; i < team.size(); i++) {
        std::cout << "\n" << (i + 1) << ". ";
        
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
        
        std::cout << "\n   Tipo: " << team[i]->getType1();
        if (!team[i]->getType2().empty()) {
            std::cout << "/" << team[i]->getType2();
        }
    }
    std::cout << std::endl;
}

void Trainer::healTeam() {
    for (auto& pokemon : team) {
        pokemon->fullHeal();
    }
    std::cout << "O time de " << name << " foi curado!\n";
}

std::shared_ptr<Trainer> Trainer::clone() const {
    auto clone = std::make_shared<Trainer>(name, isPlayer);
    clone->money = money;
    
    // Clonar Pokémon
    for (const auto& pokemon : team) {
        clone->team.push_back(pokemon->clone());
    }
    
    // Definir Pokémon ativo
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
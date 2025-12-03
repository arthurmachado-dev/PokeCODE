#include "../include/GameController.h"
#include "../include/Utils.h"
#include <iostream>
#include <limits>

GameController::GameController() 
    : gameRunning(true), battleCount(0) {
    setupGame();
}

GameController::~GameController() {}

void GameController::setupGame() {
    // Inicializar Pokédex
    pokedex = std::make_shared<Pokedex>();
    
    // Carregar dados
    if (!pokedex->loadPokemonFromFile("data/pokemon.txt")) {
        std::cerr << "Erro ao carregar Pokémon. Usando dados padrão.\n";
    }
    
    if (!pokedex->loadMovesFromFile("data/moves.txt")) {
        std::cerr << "Erro ao carregar movimentos. Usando dados padrão.\n";
    }
    
    // Criar jogador
    std::string playerName;
    std::cout << Utils::Colors::CYAN;
    Utils::printCentered("POKÉMON BATTLE");
    Utils::printCentered("Sistema de Batalha Pokémon");
    std::cout << Utils::Colors::RESET << "\n";
    
    std::cout << "Digite seu nome: ";
    std::getline(std::cin, playerName);
    
    if (playerName.empty()) {
        playerName = "Ash";
    }
    
    player = std::make_shared<Trainer>(playerName, true);
    
    // Adicionar time inicial
    auto starterTeam = pokedex->getStarterTeam();
    for (auto& pokemon : starterTeam) {
        player->addPokemon(pokemon);
    }
    
    std::cout << "\nBem-vindo, " << playerName << "!\n";
    std::cout << "Você recebeu 3 Pokémon iniciais.\n";
    
    // Criar oponente padrão
    opponent = std::make_shared<Trainer>("Gary", false);
    auto cpuTeam = pokedex->getCPUTeam();
    for (auto& pokemon : cpuTeam) {
        opponent->addPokemon(pokemon);
    }
    
    Utils::waitForEnter();
}

void GameController::run() {
    while (gameRunning) {
        mainMenu();
    }
    
    Utils::printColored("\nObrigado por jogar Pokémon Battle!", Utils::Colors::GREEN);
}

void GameController::mainMenu() {
    clearScreen();
    printHeader("MENU PRINCIPAL");
    
    std::cout << "1. Nova Batalha\n";
    std::cout << "2. Continuar Batalha\n";
    std::cout << "3. Ver Time\n";
    std::cout << "4. Pokédex\n";
    std::cout << "5. Opções\n";
    std::cout << "6. Sair\n";
    
    int choice = getMenuChoice(1, 6);
    
    switch (choice) {
        case 1:
            startNewBattle();
            break;
        case 2:
            continueBattle();
            break;
        case 3:
            trainerMenu();
            break;
        case 4:
            pokedexMenu();
            break;
        case 5:
            optionsMenu();
            break;
        case 6:
            gameRunning = false;
            break;
    }
}

void GameController::startNewBattle() {
    // Criar novo oponente
    opponent = std::make_shared<Trainer>("Treinador " + std::to_string(battleCount + 1), false);
    auto cpuTeam = pokedex->getCPUTeam();
    for (auto& pokemon : cpuTeam) {
        opponent->addPokemon(pokemon);
    }
    
    // Curar time do jogador
    player->healTeam();
    
    battleMenu();
}

void GameController::continueBattle() {
    if (battleCount == 0) {
        std::cout << "Nenhuma batalha em andamento.\n";
        Utils::waitForEnter();
        return;
    }
    
    battleMenu();
}

void GameController::battleMenu() {
    // Criar sistema de batalha
    auto battleSystem = std::make_shared<BattleSystem>(player, opponent, pokedex);
    battleSystem->startBattle();
    
    while (!battleSystem->isBattleOver()) {
        std::cout << "\nO que " << player->getActivePokemon()->getName() << " deve fazer?\n";
        std::cout << "1. Lutar\n";
        std::cout << "2. Pokémon\n";
        std::cout << "3. Mochila\n";
        std::cout << "4. Fugir\n";
        
        int choice = getMenuChoice(1, 4);
        
        switch (choice) {
            case 1: // Lutar
                {
                    battleSystem->printAvailableMoves();
                    std::cout << "0. Voltar\n";
                    
                    int moveChoice = getMenuChoice(0, 4) - 1;
                    if (moveChoice >= 0) {
                        battleSystem->playerAttack(moveChoice);
                    }
                }
                break;
                
            case 2: // Pokémon
                {
                    player->printTeam();
                    std::cout << "Escolha um Pokémon para trocar (0 para cancelar): ";
                    
                    int pokemonChoice = getMenuChoice(0, player->getTeamSize()) - 1;
                    if (pokemonChoice >= 0) {
                        battleSystem->playerSwitchPokemon(pokemonChoice);
                    }
                }
                break;
                
            case 3: // Mochila
                battleSystem->playerUseItem();
                break;
                
            case 4: // Fugir
                if (battleSystem->playerAttemptRun()) {
                    return;
                }
                break;
        }
    }
    
    battleCount++;
    Utils::waitForEnter();
}

void GameController::trainerMenu() {
    clearScreen();
    printHeader("MEU TIME");
    
    player->printTeam();
    
    std::cout << "\n1. Ver detalhes de um Pokémon\n";
    std::cout << "2. Curar time\n";
    std::cout << "3. Voltar\n";
    
    int choice = getMenuChoice(1, 3);
    
    switch (choice) {
        case 1:
            {
                std::cout << "Escolha um Pokémon (1-" << player->getTeamSize() << "): ";
                int pokemonChoice = getMenuChoice(1, player->getTeamSize()) - 1;
                
                auto team = player->getTeam();
                if (pokemonChoice >= 0 && pokemonChoice < static_cast<int>(team.size())) {
                    team[pokemonChoice]->printStats();
                }
                Utils::waitForEnter();
            }
            break;
            
        case 2:
            player->healTeam();
            Utils::waitForEnter();
            break;
            
        case 3:
            return;
    }
    
    trainerMenu(); // Mostrar menu novamente
}

void GameController::pokedexMenu() {
    clearScreen();
    printHeader("POKÉDEX");
    
    std::cout << "Pokémon registrados: " << pokedex->getPokemonCount() << "\n";
    std::cout << "Movimentos registrados: " << pokedex->getMoveCount() << "\n\n";
    
    std::cout << "1. Listar todos Pokémon\n";
    std::cout << "2. Listar todos movimentos\n";
    std::cout << "3. Buscar Pokémon por nome\n";
    std::cout << "4. Buscar movimento por nome\n";
    std::cout << "5. Voltar\n";
    
    int choice = getMenuChoice(1, 5);
    
    switch (choice) {
        case 1:
            pokedex->printAllPokemon();
            Utils::waitForEnter();
            break;
            
        case 2:
            pokedex->printAllMoves();
            Utils::waitForEnter();
            break;
            
        case 3:
            {
                std::cout << "Nome do Pokémon: ";
                std::string name;
                std::getline(std::cin, name);
                
                auto pokemon = pokedex->getPokemonByName(name);
                if (pokemon) {
                    pokemon->printStats();
                } else {
                    std::cout << "Pokémon não encontrado.\n";
                }
                Utils::waitForEnter();
            }
            break;
            
        case 4:
            {
                std::cout << "Nome do movimento: ";
                std::string name;
                std::getline(std::cin, name);
                
                auto move = pokedex->getMoveByName(name);
                if (move) {
                    std::cout << "\n" << move->getName() << "\n";
                    std::cout << "Tipo: " << move->getType() << "\n";
                    std::cout << "Categoria: " << move->getCategory() << "\n";
                    std::cout << "Poder: " << move->getPower() << "\n";
                    std::cout << "Precisão: " << move->getAccuracy() << "%\n";
                    std::cout << "PP: " << move->getMaxPP() << "\n";
                } else {
                    std::cout << "Movimento não encontrado.\n";
                }
                Utils::waitForEnter();
            }
            break;
            
        case 5:
            return;
    }
    
    pokedexMenu(); // Mostrar menu novamente
}

void GameController::optionsMenu() {
    clearScreen();
    printHeader("OPÇÕES");
    
    std::cout << "1. Salvar jogo\n";
    std::cout << "2. Carregar jogo\n";
    std::cout << "3. Instruções\n";
    std::cout << "4. Créditos\n";
    std::cout << "5. Voltar\n";
    
    int choice = getMenuChoice(1, 5);
    
    switch (choice) {
        case 1:
            if (saveGame("data/save_data.txt")) {
                std::cout << "Jogo salvo com sucesso!\n";
            } else {
                std::cout << "Erro ao salvar jogo.\n";
            }
            Utils::waitForEnter();
            break;
            
        case 2:
            if (loadGame("data/save_data.txt")) {
                std::cout << "Jogo carregado com sucesso!\n";
            } else {
                std::cout << "Erro ao carregar jogo.\n";
            }
            Utils::waitForEnter();
            break;
            
        case 3:
            clearScreen();
            printHeader("INSTRUÇÕES");
            std::cout << "POKÉMON BATTLE - Sistema de Batalha Pokémon\n\n";
            std::cout << "Como jogar:\n";
            std::cout << "1. Escolha 'Nova Batalha' para começar uma batalha\n";
            std::cout << "2. Durante a batalha, você tem 4 opções:\n";
            std::cout << "   - LUTAR: Usar movimentos do Pokémon\n";
            std::cout << "   - POKÉMON: Trocar de Pokémon\n";
            std::cout << "   - MOCHILA: Usar itens\n";
            std::cout << "   - FUGIR: Tentar escapar da batalha\n";
            std::cout << "3. Derrote todos Pokémon do oponente para vencer\n";
            std::cout << "4. Seus Pokémon ganham experiência e podem evoluir\n";
            std::cout << "5. Use a Pokédex para ver informações\n\n";
            std::cout << "Controles: Use números para selecionar opções\n";
            Utils::waitForEnter();
            break;
            
        case 4:
            clearScreen();
            printHeader("CRÉDITOS");
            std::cout << "POKÉMON BATTLE\n";
            std::cout << "Desenvolvido para o trabalho final de POO - UFMG\n\n";
            std::cout << "Funcionalidades implementadas:\n";
            std::cout << "- Sistema de batalha turn-based\n";
            std::cout << "- 151 Pokémon da primeira geração\n";
            std::cout << "- 100+ movimentos/ataques\n";
            std::cout << "- Sistema de tipos com vantagens/desvantagens\n";
            std::cout << "- Experiência e níveis\n";
            std::cout << "- Interface colorida\n";
            std::cout << "- Pokédex pesquisável\n";
            std::cout << "- IA básica para CPU\n\n";
            std::cout << "Desenvolvido em C++11\n";
            Utils::waitForEnter();
            break;
            
        case 5:
            return;
    }
    
    optionsMenu(); // Mostrar menu novamente
}

void GameController::clearScreen() const {
    // Limpar tela (ANSI escape code)
    std::cout << "\033[2J\033[1;1H";
}

void GameController::printHeader(const std::string& title) const {
    Utils::printSeparator('=');
    Utils::printCentered(title);
    Utils::printSeparator('=');
    std::cout << "\n";
}

int GameController::getMenuChoice(int min, int max) const {
    int choice;
    while (true) {
        std::cout << "\nEscolha (" << min << "-" << max << "): ";
        std::cin >> choice;
        
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Escolha inválida. Tente novamente.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

bool GameController::saveGame(const std::string& filename) {
    // Implementação simplificada de salvamento
    std::string content = player->getName() + "\n";
    content += std::to_string(battleCount) + "\n";
    
    return Utils::writeFile(filename, content);
}

bool GameController::loadGame(const std::string& filename) {
    // Implementação simplificada de carregamento
    std::string content = Utils::readFile(filename);
    if (content.empty()) {
        return false;
    }
    
    auto lines = Utils::split(content, '\n');
    if (lines.size() >= 2) {
        player->setName(lines[0]);
        battleCount = std::stoi(lines[1]);
        return true;
    }
    
    return false;
}
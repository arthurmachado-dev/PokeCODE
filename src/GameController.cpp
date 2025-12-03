#include "../include/GameController.h"
#include "../include/Utils.h"
#include "../include/Pokedex.h"
#include "../include/Trainer.h"
#include "../include/BattleSystem.h"
#include "../include/Pokemon.h"
#include "../include/Move.h"
#include <iostream>
#include <limits>
#include <fstream>
#include <sstream>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#else
#include <dirent.h>
#include <sys/stat.h>
#endif

GameController::GameController() 
    : gameRunning(true), battleCount(0) {
    setupGame();
}

GameController::~GameController() {}

void GameController::setupGame() {
    pokedex = std::make_shared<Pokedex>();
    
    if (!pokedex->loadPokemonFromFile("data/pokemon_with_evolution.txt")) {
        std::cerr << "Erro ao carregar Pokemon.\n";
    }
    
    if (!pokedex->loadMovesFromFile("data/moves_simple.txt")) {
        std::cerr << "Erro ao carregar movimentos.\n";
    }
    
    // Tentar carregar save automático primeiro
    if (loadGame("data/save_auto.sav")) {
        std::cout << "\nJogo carregado automaticamente!\n";
        Utils::waitForEnter();
        return;
    }
    
    std::string playerName;
    std::cout << Utils::Colors::CYAN;
    std::cout << "========================================\n";
    std::cout << "           POKEMON BATTLE SYSTEM\n";
    std::cout << "========================================\n";
    std::cout << Utils::Colors::RESET << "\n";
    
    std::cout << "Digite seu nome: ";
    std::getline(std::cin, playerName);
    
    if (playerName.empty()) playerName = "Ash";
    
    player = std::make_shared<Trainer>(playerName, true);
    
    auto starterTeam = pokedex->getStarterTeam();
    for (auto& pokemon : starterTeam) {
        player->addPokemon(pokemon);
    }
    
    std::cout << "\nBem-vindo, " << playerName << "!\n";
    std::cout << "Voce recebeu 3 Pokemon iniciais.\n";
    
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
    
    Utils::printColored("\nObrigado por jogar Pokemon Battle!", Utils::Colors::GREEN);
}

void GameController::mainMenu() {
    clearScreen();
    printHeader("MENU PRINCIPAL");
    
    std::cout << "1. Nova Batalha\n";
    std::cout << "2. Ver Time\n";
    std::cout << "3. Capturar Pokemon\n";
    std::cout << "4. Pokedex\n";
    std::cout << "5. Opcoes\n";
    std::cout << "6. Sair\n";
    
    int choice = getMenuChoice(1, 6);
    
    switch (choice) {
        case 1:
            startNewBattle();
            break;
        case 2:
            trainerMenu();
            break;
        case 3:
            captureMenu();
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
    opponent = std::make_shared<Trainer>("Treinador " + std::to_string(battleCount + 1), false);
    auto cpuTeam = pokedex->getCPUTeam();
    for (auto& pokemon : cpuTeam) {
        opponent->addPokemon(pokemon);
    }
    
    player->healTeam();
    
    battleMenu();
}

void GameController::battleMenu() {
    auto battleSystem = std::make_shared<BattleSystem>(player, opponent, pokedex);
    battleSystem->startBattle();
    
    while (!battleSystem->isBattleOver()) {
        auto activePokemon = player->getActivePokemon();
        if (!activePokemon || activePokemon->getIsFainted()) {
            std::cout << "\nSeu Pokemon desmaiou! Escolha outro:\n";
            player->printTeam();
            std::cout << "Escolha um Pokemon para continuar (1-" << player->getTeamSize() << "): ";
            int pokeChoice = getMenuChoice(1, player->getTeamSize()) - 1;
            battleSystem->playerSwitchPokemon(pokeChoice);
            continue;
        }
        
        std::cout << "\nO que " << activePokemon->getName() << " deve fazer?\n";
        std::cout << "1. Lutar\n";
        std::cout << "2. Pokemon\n";
        std::cout << "3. Mochila\n";
        std::cout << "4. Fugir\n";
        
        int choice = getMenuChoice(1, 4);
        
        switch (choice) {
            case 1: {
                battleSystem->printAvailableMoves();
                std::cout << "0. Voltar\n";
                
                int moveChoice = getMenuChoice(0, 4) - 1;
                if (moveChoice >= 0) {
                    battleSystem->playerAttack(moveChoice);
                }
                break;
            }
                
            case 2: {
                player->printTeam();
                std::cout << "Escolha um Pokemon para trocar (0 para cancelar): ";
                
                int pokemonChoice = getMenuChoice(0, player->getTeamSize()) - 1;
                if (pokemonChoice >= 0) {
                    battleSystem->playerSwitchPokemon(pokemonChoice);
                }
                break;
            }
                
            case 3:
                battleSystem->playerUseItem();
                break;
                
            case 4:
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
    
    std::cout << "\n1. Ver detalhes de um Pokemon\n";
    std::cout << "2. Curar time\n";
    std::cout << "3. Liberar Pokemon\n";
    std::cout << "4. Evoluir Pokemon\n";
    std::cout << "5. Voltar\n";
    
    int choice = getMenuChoice(1, 5);
    
    switch (choice) {
        case 1: {
            std::cout << "Escolha um Pokemon (1-" << player->getTeamSize() << "): ";
            int pokemonChoice = getMenuChoice(1, player->getTeamSize()) - 1;
            
            auto team = player->getTeam();
            if (pokemonChoice >= 0 && pokemonChoice < static_cast<int>(team.size())) {
                team[pokemonChoice]->printStats();
            }
            Utils::waitForEnter();
            break;
        }
            
        case 2:
            player->healTeam();
            Utils::waitForEnter();
            break;
            
        case 3: {
            std::cout << "Escolha um Pokemon para liberar (1-" << player->getTeamSize() << "): ";
            int pokemonChoice = getMenuChoice(1, player->getTeamSize()) - 1;
            if (player->removePokemon(pokemonChoice)) {
                std::cout << "Pokemon liberado!\n";
            }
            Utils::waitForEnter();
            break;
        }
            
        case 4: {
            std::cout << "Escolha um Pokemon para evoluir (1-" << player->getTeamSize() << "): ";
            int pokemonChoice = getMenuChoice(1, player->getTeamSize()) - 1;
            
            auto team = player->getTeam();
            if (pokemonChoice >= 0 && pokemonChoice < static_cast<int>(team.size())) {
                auto pokemon = team[pokemonChoice];
                
                if (pokemon->canEvolve()) {
                    std::cout << "\n" << pokemon->getName() << " pode evoluir!\n";
                    std::cout << "Deseja evoluir? (1=Sim, 2=Nao): ";
                    int evolveChoice = getMenuChoice(1, 2);
                    
                    if (evolveChoice == 1) {
                        auto evolved = pokemon->evolve(pokedex);
                        if (evolved) {
                            if (player->removePokemon(pokemonChoice)) {
                                player->addPokemon(evolved);
                                std::cout << "Evolucao concluida!\n";
                                evolved->printStats();
                            }
                        }
                    }
                } else {
                    std::cout << pokemon->getName() << " nao pode evoluir agora.\n";
                    if (pokemon->getEvolvesIntoId() > 0) {
                        std::cout << "Precisa atingir nivel " << pokemon->getEvolutionLevel() << " para evoluir.\n";
                    } else {
                        std::cout << "Este Pokemon nao possui evolucao.\n";
                    }
                }
            }
            Utils::waitForEnter();
            break;
        }
            
        case 5:
            return;
    }
    
    trainerMenu();
}

void GameController::captureMenu() {
    clearScreen();
    printHeader("CAPTURAR POKEMON");
    
    if (!player->canAddMorePokemon()) {
        std::cout << "Seu time esta cheio! (Maximo: 6 Pokemon)\n";
        std::cout << "Libere espaco no seu time para capturar mais Pokemon.\n";
        Utils::waitForEnter();
        return;
    }
    
    std::cout << "Escolha o tipo de Pokemon para capturar:\n";
    std::cout << "1. Pokemon Iniciante (Nivel 5-10)\n";
    std::cout << "2. Pokemon Intermediario (Nivel 10-20)\n";
    std::cout << "3. Pokemon Avancado (Nivel 20-30)\n";
    std::cout << "4. Pokemon Aleatorio\n";
    std::cout << "5. Voltar\n";
    
    int choice = getMenuChoice(1, 5);
    
    if (choice == 5) return;
    
    int minLevel = 5, maxLevel = 10;
    switch (choice) {
        case 1: minLevel = 5; maxLevel = 10; break;
        case 2: minLevel = 10; maxLevel = 20; break;
        case 3: minLevel = 20; maxLevel = 30; break;
        case 4: minLevel = 5; maxLevel = 30; break;
    }
    
    int captureChance = 70;
    
    std::cout << "\nJogando Pokebola...\n";
    
    if (Utils::chance(captureChance / 100.0)) {
        auto newPokemon = pokedex->createRandomPokemon(Utils::randomInt(minLevel, maxLevel));
        if (newPokemon) {
            if (player->addPokemon(newPokemon)) {
                std::cout << "\n" << Utils::Colors::GREEN;
                std::cout << "Parabens! Voce capturou " << newPokemon->getName() << "!\n";
                std::cout << Utils::Colors::RESET;
                newPokemon->printStats();
            } else {
                std::cout << "Nao foi possivel adicionar o Pokemon ao time.\n";
            }
        }
    } else {
        std::cout << "\n" << Utils::Colors::RED;
        std::cout << "O Pokemon escapou da Pokebola!\n";
        std::cout << Utils::Colors::RESET;
    }
    
    Utils::waitForEnter();
}

void GameController::pokedexMenu() {
    clearScreen();
    printHeader("POKEDEX");
    
    std::cout << "Pokemon registrados: " << pokedex->getPokemonCount() << "\n";
    std::cout << "Movimentos registrados: " << pokedex->getMoveCount() << "\n\n";
    
    std::cout << "1. Listar todos Pokemon\n";
    std::cout << "2. Listar todos movimentos\n";
    std::cout << "3. Buscar Pokemon por nome\n";
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
            
        case 3: {
            std::cout << "Nome do Pokemon: ";
            std::string name;
            std::getline(std::cin, name);
            
            auto pokemon = pokedex->getPokemonByName(name);
            if (pokemon) {
                pokemon->printStats();
            } else {
                std::cout << "Pokemon nao encontrado.\n";
            }
            Utils::waitForEnter();
            break;
        }
            
        case 4: {
            std::cout << "Nome do movimento: ";
            std::string name;
            std::getline(std::cin, name);
            
            auto move = pokedex->getMoveByName(name);
            if (move) {
                std::cout << "\n" << move->getName() << "\n";
                std::cout << "Tipo: " << move->getType() << "\n";
                std::cout << "Categoria: " << move->getCategory() << "\n";
                std::cout << "Poder: " << move->getPower() << "\n";
                std::cout << "Precisao: " << move->getAccuracy() << "%\n";
                std::cout << "PP: " << move->getMaxPP() << "\n";
            } else {
                std::cout << "Movimento nao encontrado.\n";
            }
            Utils::waitForEnter();
            break;
        }
            
        case 5:
            return;
    }
    
    pokedexMenu();
}

void GameController::optionsMenu() {
    clearScreen();
    printHeader("OPCOES");
    
    std::cout << "1. Salvar jogo\n";
    std::cout << "2. Carregar jogo\n";
    std::cout << "3. Novo jogo (Perde progresso atual)\n";
    std::cout << "4. Instrucoes\n";
    std::cout << "5. Creditos\n";
    std::cout << "6. Voltar\n";
    
    int choice = getMenuChoice(1, 6);
    
    switch (choice) {
        case 1:
            saveMenu();
            break;
            
        case 2:
            loadMenu();
            break;
            
        case 3: {
            std::cout << "Tem certeza que deseja iniciar novo jogo?\n";
            std::cout << "Todo progresso sera perdido. (1=Sim, 2=Nao): ";
            int confirm = getMenuChoice(1, 2);
            if (confirm == 1) {
                setupGame();
                battleCount = 0;
                std::cout << "Novo jogo iniciado!\n";
            }
            Utils::waitForEnter();
            break;
        }
            
        case 4:
            clearScreen();
            printHeader("INSTRUCOES");
            std::cout << "Como jogar:\n";
            std::cout << "1. Nova Batalha: Inicia batalha contra CPU\n";
            std::cout << "2. Ver Time: Gerencia seus Pokemon (curar, evoluir, liberar)\n";
            std::cout << "3. Capturar Pokemon: Adiciona novos Pokemon ao time\n";
            std::cout << "4. Pokedex: Consulta informacoes\n";
            std::cout << "\nDurante a batalha:\n";
            std::cout << "- Lutar: Usa movimentos\n";
            std::cout << "- Pokemon: Troca de Pokemon\n";
            std::cout << "- Mochila: Usa itens (Pocao)\n";
            std::cout << "- Fugir: Tenta escapar\n";
            Utils::waitForEnter();
            break;
            
        case 5:
            clearScreen();
            printHeader("CREDITOS");
            std::cout << "Pokemon Battle System\n";
            std::cout << "Trabalho Final de POO - UFMG\n";
            std::cout << "Desenvolvido em C++11\n";
            std::cout << "Funcionalidades:\n";
            std::cout << "- Sistema de batalha completo\n";
            std::cout << "- Evolucao de Pokemon\n";
            std::cout << "- Salvamento de progresso\n";
            std::cout << "- Captura de novos Pokemon\n";
            Utils::waitForEnter();
            break;
            
        case 6:
            return;
    }
    
    optionsMenu();
}

void GameController::saveMenu() {
    clearScreen();
    printHeader("SALVAR JOGO");
    
    std::cout << "Escolha um slot para salvar:\n";
    std::cout << "1. Slot 1\n";
    std::cout << "2. Slot 2\n";
    std::cout << "3. Slot 3\n";
    std::cout << "4. Salvar automaticamente (sobrescreve auto-save)\n";
    std::cout << "5. Voltar\n";
    
    int choice = getMenuChoice(1, 5);
    
    if (choice == 5) return;
    
    std::string filename;
    switch (choice) {
        case 1: filename = "data/save_1.sav"; break;
        case 2: filename = "data/save_2.sav"; break;
        case 3: filename = "data/save_3.sav"; break;
        case 4: filename = "data/save_auto.sav"; break;
    }
    
    std::cout << "Digite um nome para este save (opcional): ";
    std::string saveName;
    std::getline(std::cin, saveName);
    
    if (saveName.empty()) {
        saveName = "Save " + std::to_string(choice);
    }
    
    if (saveGame(filename)) {
        std::cout << "\n" << Utils::Colors::GREEN;
        std::cout << "Jogo salvo com sucesso!\n";
        std::cout << "Arquivo: " << filename << "\n";
        std::cout << "Nome: " << saveName << "\n";
        std::cout << "Time salvo: " << player->getTeamSize() << " Pokemon\n";
        std::cout << Utils::Colors::RESET;
    } else {
        std::cout << "\n" << Utils::Colors::RED;
        std::cout << "Erro ao salvar jogo.\n";
        std::cout << Utils::Colors::RESET;
    }
    
    Utils::waitForEnter();
}

void GameController::loadMenu() {
    clearScreen();
    printHeader("CARREGAR JOGO");
    
    std::vector<std::string> saveFiles = getSaveFiles();
    
    if (saveFiles.empty()) {
        std::cout << "Nenhum arquivo de salvamento encontrado.\n";
        Utils::waitForEnter();
        return;
    }
    
    std::cout << "Arquivos de salvamento disponiveis:\n";
    for (size_t i = 0; i < saveFiles.size(); i++) {
        std::cout << (i + 1) << ". " << saveFiles[i] << "\n";
    }
    std::cout << (saveFiles.size() + 1) << ". Voltar\n";
    
    int choice = getMenuChoice(1, saveFiles.size() + 1);
    
    if (choice == static_cast<int>(saveFiles.size() + 1)) {
        return;
    }
    
    std::string filename = saveFiles[choice - 1];
    
    if (loadGame(filename)) {
        std::cout << "\n" << Utils::Colors::GREEN;
        std::cout << "Jogo carregado com sucesso!\n";
        std::cout << "Nome: " << player->getName() << "\n";
        std::cout << "Batalhas: " << battleCount << "\n";
        std::cout << "Time: " << player->getTeamSize() << " Pokemon\n";
        std::cout << Utils::Colors::RESET;
    } else {
        std::cout << "\n" << Utils::Colors::RED;
        std::cout << "Erro ao carregar jogo.\n";
        std::cout << Utils::Colors::RESET;
    }
    
    Utils::waitForEnter();
}

std::vector<std::string> GameController::getSaveFiles() const {
    std::vector<std::string> saveFiles;
    
#ifdef _WIN32
    // Para Windows: usar funções ANSI (A) em vez de Unicode (W)
    CreateDirectoryA("data", NULL);
    
    WIN32_FIND_DATAA findFileData;
    std::string searchPath = "data\\*.sav";
    HANDLE hFind = FindFirstFileA(searchPath.c_str(), &findFileData);
    
    if (hFind != INVALID_HANDLE_VALUE) {
        do {
            // Verificar se não é um diretório
            if (!(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)) {
                std::string filename = findFileData.cFileName;
                // Verificar extensão .sav
                if (filename.length() >= 4 && 
                    filename.substr(filename.length() - 4) == ".sav") {
                    saveFiles.push_back("data/" + filename);
                }
            }
        } while (FindNextFileA(hFind, &findFileData) != 0);
        FindClose(hFind);
    }
#else
    // Para Linux/Mac
    mkdir("data", 0777);
    
    DIR *dir;
    struct dirent *ent;
    if ((dir = opendir("data/")) != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            std::string filename = ent->d_name;
            if (filename.length() > 4 && 
                filename.substr(filename.length() - 4) == ".sav") {
                saveFiles.push_back(std::string("data/") + filename);
            }
        }
        closedir(dir);
    }
#endif
    
    return saveFiles;
}

void GameController::clearScreen() const {
    #ifdef _WIN32
        system("cls");
    #else
        system("clear");
    #endif
}

void GameController::printHeader(const std::string& title) const {
    std::cout << "========================================\n";
    std::cout << "           " << title << "\n";
    std::cout << "========================================\n\n";
}

int GameController::getMenuChoice(int min, int max) const {
    int choice;
    while (true) {
        std::cout << "\nEscolha (" << min << "-" << max << "): ";
        std::cin >> choice;
        
        if (std::cin.fail() || choice < min || choice > max) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Escolha invalida. Tente novamente.\n";
        } else {
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            return choice;
        }
    }
}

bool GameController::saveGame(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) {
        std::cout << "Erro ao criar arquivo de salvamento.\n";
        return false;
    }
    
    file << "POKEMON_SAVE_v2\n";
    file << battleCount << "\n";
    file << player->getName() << "\n";
    file << "Time size: " << player->getTeamSize() << "\n";
    
    std::string trainerData = player->serialize();
    file << trainerData;
    
    file.close();
    return true;
}

bool GameController::loadGame(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Arquivo de salvamento nao encontrado: " << filename << "\n";
        return false;
    }
    
    std::string version;
    std::getline(file, version);
    
    if (version != "POKEMON_SAVE_v1" && version != "POKEMON_SAVE_v2") {
        std::cout << "Versao de salvamento invalida.\n";
        return false;
    }
    
    std::string battleCountStr;
    std::getline(file, battleCountStr);
    battleCount = std::stoi(battleCountStr);
    
    if (version == "POKEMON_SAVE_v2") {
        std::string playerName;
        std::getline(file, playerName);
        std::string teamSizeLine;
        std::getline(file, teamSizeLine);
    }
    
    std::stringstream trainerData;
    std::string line;
    while (std::getline(file, line)) {
        trainerData << line << "\n";
    }
    
    auto loadedPlayer = Trainer::deserialize(trainerData.str(), pokedex);
    if (!loadedPlayer) {
        std::cout << "Erro ao carregar dados do treinador.\n";
        return false;
    }
    
    player = loadedPlayer;
    file.close();
    
    return true;
}

#ifndef GAMECONTROLLER_H
#define GAMECONTROLLER_H

#include <memory>
#include <string>
#include <vector>  // ADICIONAR ESTA LINHA

class Pokedex;
class Trainer;
class BattleSystem;
class Pokemon;
class Move;

class GameController {
private:
    std::shared_ptr<Pokedex> pokedex;
    std::shared_ptr<Trainer> player;
    std::shared_ptr<Trainer> opponent;
    
    bool gameRunning;
    int battleCount;
    
    void mainMenu();
    void battleMenu();
    void pokedexMenu();
    void trainerMenu();
    void captureMenu();
    void optionsMenu();
    void saveMenu();
    void loadMenu();
    
    void setupGame();
    void startNewBattle();
    
    void clearScreen() const;
    void printHeader(const std::string& title) const;
    int getMenuChoice(int min, int max) const;
    
    bool saveGame(const std::string& filename);
    bool loadGame(const std::string& filename);
    std::vector<std::string> getSaveFiles() const;
    
public:
    GameController();
    ~GameController();
    
    void run();
    
    std::shared_ptr<Pokedex> getPokedex() const { return pokedex; }
    std::shared_ptr<Trainer> getPlayer() const { return player; }
};

#endif

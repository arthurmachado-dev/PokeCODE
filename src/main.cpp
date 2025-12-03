#include "../include/GameController.h"
#include <iostream>

int main() {
    try {
        GameController game;
        game.run();
    } catch (const std::exception& e) {
        std::cerr << "Erro fatal: " << e.what() << std::endl;
        return 1;
    } catch (...) {
        std::cerr << "Erro fatal desconhecido" << std::endl;
        return 1;
    }
    return 0;
}

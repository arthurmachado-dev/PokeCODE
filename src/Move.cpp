#include "../include/Move.h"
#include <stdexcept>

Move::Move(int id, const std::string& name, const std::string& type, 
           const std::string& category, int power, int accuracy, int pp)
    : id(id), name(name), type(type), category(category), power(power),
      accuracy(accuracy), maxPP(pp), currentPP(pp) {
    
    if (maxPP <= 0) maxPP = 1;
}

void Move::setCurrentPP(int pp) {
    currentPP = std::max(0, std::min(pp, maxPP));
}

bool Move::use() {
    if (currentPP > 0) {
        currentPP--;
        return true;
    }
    return false;
}

void Move::restorePP(int amount) {
    currentPP = std::min(currentPP + amount, maxPP);
}

void Move::restoreAllPP() {
    currentPP = maxPP;
}

std::shared_ptr<Move> Move::clone() const {
    auto clone = std::make_shared<Move>(id, name, type, category, power, accuracy, maxPP);
    clone->currentPP = currentPP;
    return clone;
}

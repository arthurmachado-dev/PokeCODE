#ifndef MOVE_H
#define MOVE_H

#include <string>
#include <memory>

class Move {
private:
    int id;
    std::string name;
    std::string type;
    std::string category;
    int power;
    int accuracy;
    int maxPP;
    int currentPP;
    
public:
    Move(int id, const std::string& name, const std::string& type, 
         const std::string& category, int power, int accuracy, int pp);
    
    int getId() const { return id; }
    std::string getName() const { return name; }
    std::string getType() const { return type; }
    std::string getCategory() const { return category; }
    int getPower() const { return power; }
    int getAccuracy() const { return accuracy; }
    int getMaxPP() const { return maxPP; }
    int getCurrentPP() const { return currentPP; }
    
    void setCurrentPP(int pp);
    
    bool use();
    void restorePP(int amount);
    void restoreAllPP();
    
    std::shared_ptr<Move> clone() const;
};

#endif

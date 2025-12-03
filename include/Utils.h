#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <vector>

namespace Utils {
    std::string toLower(const std::string& str);
    std::string toUpper(const std::string& str);
    std::string trim(const std::string& str);
    std::vector<std::string> split(const std::string& str, char delimiter);
    
    bool fileExists(const std::string& filename);
    std::string readFile(const std::string& filename);
    bool writeFile(const std::string& filename, const std::string& content);
    
    int randomInt(int min, int max);
    double randomDouble(double min, double max);
    bool chance(double probability);
    
    namespace Colors {
        extern const std::string RESET;
        extern const std::string RED;
        extern const std::string GREEN;
        extern const std::string YELLOW;
        extern const std::string BLUE;
        extern const std::string MAGENTA;
        extern const std::string CYAN;
        extern const std::string WHITE;
        extern const std::string BOLD;
        extern const std::string DIM;
    }
    
    void printColored(const std::string& text, const std::string& color);
    void printCentered(const std::string& text, int width = 80);
    void printSeparator(char ch = '-', int length = 80);
    void waitForEnter();
    void clearInputBuffer();
    
    std::string createProgressBar(int current, int max, int width = 20, 
                                  const std::string& fill = "#", 
                                  const std::string& empty = ".");
}

#endif

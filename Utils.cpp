#include "../include/Utils.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <random>
#include <chrono>

namespace {
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count());
}

namespace Utils {
    // Strings
    std::string toLower(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::tolower(c); });
        return result;
    }
    
    std::string toUpper(const std::string& str) {
        std::string result = str;
        std::transform(result.begin(), result.end(), result.begin(),
                      [](unsigned char c) { return std::toupper(c); });
        return result;
    }
    
    std::string trim(const std::string& str) {
        size_t start = str.find_first_not_of(" \t\n\r");
        size_t end = str.find_last_not_of(" \t\n\r");
        
        if (start == std::string::npos) {
            return "";
        }
        
        return str.substr(start, end - start + 1);
    }
    
    std::vector<std::string> split(const std::string& str, char delimiter) {
        std::vector<std::string> result;
        std::stringstream ss(str);
        std::string item;
        
        while (std::getline(ss, item, delimiter)) {
            result.push_back(item);
        }
        
        return result;
    }
    
    // Arquivos
    bool fileExists(const std::string& filename) {
        std::ifstream file(filename);
        return file.good();
    }
    
    std::string readFile(const std::string& filename) {
        std::ifstream file(filename);
        if (!file.is_open()) {
            return "";
        }
        
        std::stringstream buffer;
        buffer << file.rdbuf();
        return buffer.str();
    }
    
    bool writeFile(const std::string& filename, const std::string& content) {
        std::ofstream file(filename);
        if (!file.is_open()) {
            return false;
        }
        
        file << content;
        file.close();
        return true;
    }
    
    // Números
    int randomInt(int min, int max) {
        std::uniform_int_distribution<int> dist(min, max);
        return dist(rng);
    }
    
    double randomDouble(double min, double max) {
        std::uniform_real_distribution<double> dist(min, max);
        return dist(rng);
    }
    
    bool chance(double probability) {
        std::uniform_real_distribution<double> dist(0.0, 1.0);
        return dist(rng) <= probability;
    }
    
    // Cores (ANSI escape codes)
    namespace Colors {
        const std::string RESET = "\033[0m";
        const std::string RED = "\033[31m";
        const std::string GREEN = "\033[32m";
        const std::string YELLOW = "\033[33m";
        const std::string BLUE = "\033[34m";
        const std::string MAGENTA = "\033[35m";
        const std::string CYAN = "\033[36m";
        const std::string WHITE = "\033[37m";
        const std::string BOLD = "\033[1m";
        const std::string DIM = "\033[2m";
    }
    
    // Interface
    void printColored(const std::string& text, const std::string& color) {
        std::cout << color << text << Colors::RESET << std::endl;
    }
    
    void printCentered(const std::string& text, int width) {
        int padding = (width - static_cast<int>(text.length())) / 2;
        if (padding < 0) padding = 0;
        
        std::cout << std::string(padding, ' ') << text << std::endl;
    }
    
    void printSeparator(char ch, int length) {
        std::cout << std::string(length, ch) << std::endl;
    }
    
    void waitForEnter() {
        std::cout << "\nPressione Enter para continuar...";
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    void clearInputBuffer() {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    
    // Barras de progresso
    std::string createProgressBar(int current, int max, int width, 
                                 const std::string& fill, 
                                 const std::string& empty) {
        if (max <= 0) max = 1;
        if (current > max) current = max;
        if (current < 0) current = 0;
        
        float percentage = static_cast<float>(current) / max;
        int filledWidth = static_cast<int>(percentage * width);
        
        std::string bar;
        for (int i = 0; i < width; i++) {
            if (i < filledWidth) {
                bar += fill;
            } else {
                bar += empty;
            }
        }
        
        return bar;
    }
}
CXX = g++
CXXFLAGS = -std=c++11 -Iinclude
TARGET = pokemon_battle
SRC = src/*.cpp

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) $(SRC) -o $(TARGET)

run: $(TARGET)
	./$(TARGET)

# Limpar
clean:
	rm -f $(TARGET) *.o

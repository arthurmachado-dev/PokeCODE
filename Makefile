# Makefile para Pokémon Battle System
# Compilador
CXX = g++
CXXFLAGS = -std=c++11 -Wall -Wextra -Iinclude

# Nome do executável
TARGET = pokemon_battle

# Diretórios
SRCDIR = src
INCDIR = include
OBJDIR = obj
DATADIR = data

# Arquivos fonte
SOURCES = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

# Regra principal
all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp | $(OBJDIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR):
	mkdir -p $(OBJDIR)

# Limpar arquivos compilados
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Executar o programa
run: $(TARGET)
	./$(TARGET)

# Regra para garantir que os diretórios de dados existam
init:
	mkdir -p $(DATADIR) $(INCDIR) $(SRCDIR) $(OBJDIR)
	@echo "Estrutura de diretórios criada!"

.PHONY: all clean run init
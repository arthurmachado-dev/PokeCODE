# PokeCODE
Turma: TF Integrantes: Caio Henrique Pereira de Freitas, Jullia Guedes Freire, Marcio Arthur Machado de Oliveira Professor: Fernando Skackauskas Dias

Este documento descreve o projeto PokéCode, um sistema desenvolvido em C++
que simula batalhas baseadas no universo Pokémon. O projeto utiliza programação
orientada a objetos, leitura de arquivos externos e uma arquitetura modular para organizar
os dados de Pokémon, ataques e lógica de combate.

# Descrição do Projeto
O PokéCode implementa um sistema interativo no qual o usuário seleciona um
Pokémon para participar de uma batalha contra um oponente gerado aleatoriamente, esse
combate continua até que um deles seja derrotado, encerrando o jogo com uma mensagem
final.

Os dados referentes aos Pokémon e seus movimentos são carregados a partir de
arquivos texto, permitindo fácil expansão e modificação. O projeto tem como objetivo
demonstrar alguns conceitos fundamentais de C++, que foram absorvidos durante a
disciplina Programação e Desenvolvimento de software II DCC204, como por exemplo
classes e métodos, encapsulamento, utilização de arquivos externos, e aaplica isso além de
simulando um simples de combate baseado no universo Pokémon.
Pré-requisitos

Para compilar e executar o projeto PokéCode, é necessário possuir:

  ● Compilador C++ com suporte ao padrão C++11 ou superior (g++)
  
  ● Utilitário Make
  
  ● Ambiente compatível com Linux, macOS ou Windows por meio do WSL ou MinGW
  
# Instalação

Para instalar o projeto, primeiro faça o download e acesse a pasta do PokéCode, em
seguida compile o programa utilizando o Makefile, após a compilação, o executável será
criado com o nome poke e poderá ser executado diretamente pelo terminal.

Após a execução do programa, o sistema carregará automaticamente as
informações presentes nos arquivos da pasta data/. O usuário deverá selecionar um
Pokémon dentre os que estiverem disponíveis e em seguida, será iniciada uma batalha
contra um Pokémon adversário que sera gerado aleatoriamente. Durante a batalha, o
sistema vai exibir ataques utilizados, danos e pontos de vida restantes.
Estrutura do Projeto

A organização de diretórios segue o padrão abaixo:
  /src Arquivos fonte (.cpp)
  /include Arquivos de cabeçalho (.h)
  /data Arquivos de dados externos
  /obj Arquivos objeto gerados na compilação
  Makefile Automação da compilação
  README.md Documento de descrição do projeto
  
# Descrição dos componentes principais
  ● main.cpp
  Responsável por iniciar o programa, carregar dados, solicitar entrada do usuário e
  executar a batalha.
  
  ● Pokemon.h / Pokemon.cpp
  Define e implementa a classe responsável pela representação dos Pokémon,
  contendo atributos como nome, vida, ataque, defesa e tipo.
  
  ● BattleSystem.h / BattleSystem.cpp
  Implementa a lógica central do sistema, incluindo leitura de arquivos, realização dos
  turnos de batalha e determinação do vencedor.
  
# Construção do projeto
  
  O projeto PokéCoede foi desenvolvido utilizando as seguintes ferramentas:
  
  ● C++ (padrão C++11)
  
  ● Makefile para automatizar a compilação
  
  ● Estrutura modularizada com arquivos de cabeçalho e implementação
  
  ● Arquivos externos em formato .txt para armazenamento de dados dos Pokémon e
  ataques, como por emeplo o pokemon.txt
  
  
  # Autores 
  Caio Henrique Pereira de Freitas,
 Jullia Guedes Freire,
 Marcio Arthur Machado de Oliveira

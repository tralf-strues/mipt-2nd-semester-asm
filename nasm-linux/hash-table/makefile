AllWarnings    = -Wall -Wextra -pedantic
SomeWarnings   = -Wall
LittleWarnings = 
NoWarnings     = -w

Linker   = -lSDL2 -pthread
Compiler = -g $(AllWarnings)

SrcDir = src
BinDir = bin
IntDir = $(BinDir)/intermediates
LibDir = libs

Libs = $(wildcard $(LibDir)/*.a)
Deps = $(wildcard $(SrcDir)/*.h) $(wildcard $(LibDir)/*.h)
Objs = $(IntDir)/main.o

$(BinDir)/define.out: $(Objs) $(Libs) $(Deps)
	g++ -o $(BinDir)/define.out $(Linker) $(Objs) $(Libs) 

$(IntDir)/main.o: $(SrcDir)/main.cpp $(Deps)
	g++ -o $(IntDir)/main.o -c $(SrcDir)/main.cpp $(Compiler)
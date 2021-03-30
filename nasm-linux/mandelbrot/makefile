AllWarnings    = -Wall -Wextra -pedantic
SomeWarnings   = -Wall
LittleWarnings = 
NoWarnings     = -w

Linker   = -lSDL2 -pthread
Compiler = -mavx2 -mavx -msse4 -O3 $(NoWarnings)

SrcDir = src
BinDir = bin
IntDir = $(BinDir)/intermediates
LibDir = libs

Libs = $(wildcard $(LibDir)/*.a)
Deps = $(wildcard $(SrcDir)/*.h) $(wildcard $(LibDir)/*.h)
Objs = $(IntDir)/main.o $(IntDir)/display.o 

$(BinDir)/a.out: $(Objs) $(Libs) $(Deps)
	g++ -o $(BinDir)/a.out $(Linker) $(Objs) $(Libs) 

$(IntDir)/main.o: $(SrcDir)/main.cpp $(Deps)
	g++ -o $(IntDir)/main.o -c $(SrcDir)/main.cpp $(Compiler)

$(IntDir)/display.o: $(SrcDir)/display.cpp $(Deps)
	g++ -o $(IntDir)/display.o -c $(SrcDir)/display.cpp $(Compiler)
# ===== SETTINGS ========

# Compile option
CXX= g++ 
CXXFLAGS= 
# LIBS=  
# DEFS= -DUSE_SOME_DEFS

# Where to compile
DIR_TOP= ~/Documents/TicTacToe
DIR_SRC= $(DIR_TOP)/src
DIR_BIN= $(DIR_TOP)/bin

# What to compile
HEADER= Command.h Display.h GameObj.h Menu.h Player.h Input.h 

# Other utilities
RM= rm -f

# ===== END OF SETTINGS =======
all: TicTacToe
	$(DIR_BIN)/TicTacToe
TicTacToe: main.o Game.o Player.o Menu.o GameObj.o
	$(CXX) main.o Game.o Player.o Menu.o GameObj.o -o $@ $(DEFS) && mv $@ $(DIR_BIN)

main.o: main.cpp Game.h
	$(CXX) -c main.cpp $(DEFS)
Game.o: Game.cpp Command.h Display.h GameObj.h Menu.h Player.h Input.h
	$(CXX) -c Game.cpp $(DEFS)
Player.o: Player.cpp GameObj.h Command.h
	$(CXX) -c Player.cpp $(DEFS)
Menu.o: Menu.cpp Display.h
	$(CXX) -c Menu.cpp $(DEFS)
GameObj.o: GameObj.cpp
	$(CXX) -c GameObj.cpp $(DEFS)

cleanobj:
	$(RM) *.o TicTacToe
cleanprog:
	$(RM) $(DIR_BIN)/TicTacToe
clean: cleanobj cleanprog

.PHONY: clean cleanobj cleanprog all TicTacToe
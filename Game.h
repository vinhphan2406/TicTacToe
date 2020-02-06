#include "GameObj.h"
#include "Player.h"
#include "Display.h"
#include "Menu.h"
#include "Command.h"
#include "Input.h"

#include <vector>

#define BOARD_DIM_1 2
#define BOARD_DIM_2 2

#define WIN_CONDITION 5

// #ifndef CLIENT_H
// #define CLIENT_H

// class Client {
//     private:
//         Game* game;
//         Player* player;
//         bool isExiting;
//         enum class GameState {
//             StartGame,
//             RunGame,
//             EndGame
//         } currentGameState;
//     public:
//         Client(){
//             this->game = NULL;
//         }
//         ~Client(){
//             if (this->game != NULL) delete this->game;
//         }
//         int initialize();
//         int finalize();
//         int inputOption(Tree<MenuComponent>*);
// };

// #endif

class ICore {
    public:
        virtual ~ICore() = default;
        virtual bool makeMove(Position) = 0;
        virtual bool printBoard() = 0;
        virtual bool checkWin() = 0;
        virtual bool checkDraw() = 0;
        virtual bool switchPlayer() = 0;
        virtual bool nextTurn() = 0;
        virtual Player* getSelectedPlayer() = 0;
        virtual int getNumOfTurns() const = 0;
        bool isRunning;
};

class TicTacToe : public ICore {
    private:
        Board* board;
        Player* player1;
        Player* player2;
        int numOfTurns;
        Player* selectedPlayer;
        Position nextMove;
    public:
        TicTacToe();
        ~TicTacToe();
        bool switchPlayer() override;
        bool checkWin(Position, int);
        bool checkWin() override;
        bool checkDraw() override;
        bool nextTurn();
        bool printBoard() override;
        Player* getSelectedPlayer() override;
        bool makeMove(Position) override;
        int getNumOfTurns() const override;
};

class MakeMoveCommand : public ICommand{
    private:
        ICore* core;
        Position pos;
    public:
        MakeMoveCommand(ICore*, Position _pos = Position());
        // Reset position
        // INPUT:
        //  _pos: new position
        // OUTPUT:
        //  true: success
        //  false: position out of range or already occupied

        // bool SetPosition(Position _pos);

        // Make move on the board
        // OUTPUT:
        //  0: success
        //  1: position already taken
        int execute() override;
};

class CheckWinCommand : public ICommand {
    private:
        ICore* core;
    public:
        CheckWinCommand(ICore*);
        int execute() override;
};

class CheckDrawCommand : public ICommand {
    private:
        ICore* core;
    public:
        CheckDrawCommand(ICore*);
        int execute() override;
};

class DisplayBoard : public IDisplay {
    private:
        ICore* core;
    public:
        DisplayBoard(ICore*);
        ~DisplayBoard();
        // Print board to console output
        // OUTPUT:
        //  - 0: success
        int print() override;
};


class DisplayMessage : public IDisplay {
    private:
        ICore* core;
        std::string message;
    public:
        DisplayMessage(ICore*, std::string);
        int print() override;
};


// #ifndef END_GAME_MENU_H
// #define END_GAME_MENU_H

// class EndGameMenu : public Tree<MenuComponent>{
//     public:
//         EndGameMenu(Game* _game, std::string _title = "", std::string _desc = "");
// };

// class StartGameCommand : public ICommand {
//     private:
//         Game* game;
//     public:
//         StartGameCommand(Game* _game): game(_game){}
//         int execute() override;
// };

// class ResetGameCommand : public ICommand {
//     private:
//         Game* game;
//     public:
//         ResetGameCommand(Game* _game): game(_game){}
//         int execute() override;
// };

// // class ExitGameCommand : public ICommand {
// //     private:
// //         Game*& game;
// //     public:
// //         ExitGameCommand(Game*& _game): game(_game){}
// //         int execute() override;
// // };

// #endif

class IGame {
    public:
        virtual ~IGame() = default;
        virtual bool handleEvent() = 0;
        virtual bool update() = 0;
        virtual bool render() = 0;
        virtual void init() = 0;
        virtual bool isExiting() = 0;
        virtual bool clean() = 0;
};

class Game : public IGame{
    private:
        ICore* core;
        IDisplay* display;
        IInput* input;
        IDisplay* error_message;
        // std::queue<ICommand*> events;
    public:
        Game(): core(NULL), display(NULL), input(NULL), error_message(NULL){}
        ~Game(){
            clean();
        }
        bool handleEvent() override;
        bool update() override;
        bool render() override;
        void init() override;
        bool isExiting() override;
        bool clean() override;
};

class RunGameCommand : public ICommand {
    private:
        IGame* game;
    public:
        RunGameCommand();
        ~RunGameCommand();
        int execute() override;
};

#ifndef START_GAME_MENU_H
#define START_GAME_MENU_H

class StartGameMenu{
    private:
        Tree<MenuComponent>* menu;
    public:

};

#endif
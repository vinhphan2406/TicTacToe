#include "GameObj.h"
#include "Player.h"
#include "Display.h"
#include "Menu.h"
#include "Command.h"
#include "Input.h"

#include <vector>
#include <stack>

#define BOARD_DIM_1 8
#define BOARD_DIM_2 8

#define WIN_CONDITION 5

class ICore {
    public:
        virtual ~ICore() = default;
        virtual ICommand::Result makeMove(Position) = 0;
        virtual bool printBoard() = 0;
        virtual bool checkWin() = 0;
        virtual bool checkDraw() = 0;
        virtual bool switchPlayer() = 0;
        virtual bool nextTurn() = 0;
        virtual Player* getSelectedPlayer() const = 0;
        virtual int getNumOfTurns() const = 0;
};

class TicTacToeCore : public ICore {
    private:
        Board* board;
        Player* player1;
        Player* player2;
        int numOfTurns;
        Player* selectedPlayer;
        Position nextMove;
    public:
        TicTacToeCore();
        ~TicTacToeCore();
        bool switchPlayer() override;
        bool checkWin(Position, int);
        bool checkWin() override;
        bool checkDraw() override;
        bool nextTurn();
        bool printBoard() override;
        Player* getSelectedPlayer() const override;
        ICommand::Result makeMove(Position) override;
        int getNumOfTurns() const override;
};

enum class GameState {
    START_MENU,
    IN_GAME,
    END_MENU,
    EXIT_GAME
};

class IGame {
    public:
        virtual ~IGame() = default;
        virtual bool handleEvent() = 0;
        virtual bool update() = 0;
        virtual bool render() = 0;
        virtual void init() = 0;
        virtual bool isExiting() const;
        virtual GameState getNextState() const;
        virtual bool clean() = 0;
    protected:
        friend class ExitStateCommand;
        friend class NextStateCommand;
        bool exit_state;
        GameState next_state;
};

#ifndef IN_GAME_MENU_H
#define IN_GAME_MENU_H

class InGame : public IGame {
    private:
        ICore* core;
        IInput* input;
        // std::queue<ICommand*> events;
        std::vector<IDisplay*> main_display;
        std::vector<IDisplay*> message;
    public:
        InGame(): core(NULL), input(NULL){}
        ~InGame(){}
        bool handleEvent() override;
        bool update() override;
        bool render() override;
        void init() override;
        bool clean() override;
};

#endif


#ifndef START_GAME_MENU_H
#define START_GAME_MENU_H

class StartGameMenu : public IGame {
    private:
        Tree<MenuComponent>* menu;
        Player* user;
        std::vector<IDisplay*> main_display;
        std::vector<IDisplay*> message;
        IInput* input;
        bool start;
    public:
        StartGameMenu(){}
        ~StartGameMenu(){}
        bool handleEvent() override;
        bool update() override;
        bool render() override;
        void init() override;
        bool clean() override;
};

#endif


#ifndef END_GAME_MENU_H
#define END_GAME_MENU_H

class EndGameMenu : public IGame {
    private:
        Tree<MenuComponent>* menu;
        Player* user;
        std::vector<IDisplay*> main_display;
        std::vector<IDisplay*> message;
        IInput* input;
        bool start;
    public:
        EndGameMenu(){
            this->exit_state = false;
            this->next_state = GameState::END_MENU;
        }
        ~EndGameMenu(){}
        bool handleEvent() override;
        bool update() override;
        bool render() override;
        void init() override;
        bool clean() override;
};

#endif

#ifndef GAME_COMMAND_H
#define GAME_COMMAND_H

class NextStateCommand : public ICommand {
    private:
        IGame* component;
        GameState newState;
    public:
        NextStateCommand(IGame* _component, GameState _state = GameState::EXIT_GAME): component(_component), newState(_state){}
        Result execute() override;
};

class ExitStateCommand : public ICommand {
    private:
        IGame* component;
    public:
        ExitStateCommand(IGame* _component): component(_component){}
        Result execute() override;
};

class IdleCommand : public ICommand {
    public:
        IdleCommand(){}
        Result execute(){
            return Result::SUCCESS;
        }
};

class MakeMoveCommand : public ICommand{
    private:
        ICore* core;
        Position pos;
    public:
        MakeMoveCommand(ICore*, Position _pos = Position());
        ~MakeMoveCommand(){}
        // Make move on the board
        // OUTPUT:
        //  0: success
        //  1: position already taken
        Result execute() override;
};

#endif

#ifndef GAME_DISPLAY_H
#define GAME_DISPLAY_H

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
        std::string message;
    public:
        DisplayMessage(std::string);
        int print() override;
};

#endif

#ifndef GAME_INPUT_H
#define GAME_INPUT_H

#include "limits"

class InputPosition : public IInput {
    private:
        std::string error_message;
        int startNum;
    public:
        InputPosition(std::string _error_msg = "Re-input position: ", int _startNum = 1): 
        error_message(_error_msg), startNum(_startNum){}
        Object input() override;
};

class InputInt : public IInput {
    private:
        std::string error_message;
    public:
        InputInt(std::string _error_msg = "Re-input option: "): error_message(_error_msg){}
        Object input() override;
};

#endif

#ifndef GAME_FRAMEWORK_H
#define GAME_FRAMEWORK_H

class GameFramework {
    public:
        GameFramework(): currentState_flag(GameState::START_MENU){
            this->current_states.push(states[GameState::START_MENU]);
        }
        ~GameFramework(){
            for (std::unordered_map<GameState, IGame*>::iterator it = states.begin(); it != states.end(); it++){
                if ((*it).second != NULL) delete (*it).second;
            }
        }
        bool run();
    private:
        std::stack<IGame*> current_states;
        GameState currentState_flag;
        std::unordered_map<GameState, IGame*> states = {
            {GameState::START_MENU, new StartGameMenu},
            {GameState::IN_GAME, new InGame},
            {GameState::END_MENU, new EndGameMenu},
            {GameState::EXIT_GAME, NULL},
        };
};

#endif
#include "GameObj.h"

#ifndef I_COMMAND_H
#define I_COMMAND_H

class ICommand {
    public:
        virtual ~ICommand() = default;
        enum class Result {
            NO_COMMAND = -1,
            SUCCESS = 0,
            OUT_OF_RANGE = 1,
            OCCUPIED = 2,
            NOT_INITIALIZED = 3,
        };
        // Execute command
        // OUTPUT
        //  - 0: success
        //  - n > 0: error code
        virtual Result execute() = 0;
};

// class MakeMoveCommand : public ICommand{
//     private:
//         Board* board;
//         Color color;
//         Position pos;
//     public:
//         MakeMoveCommand(Board* _board, Color _color, Position _pos = Position());
//         // Reset position
//         // INPUT:
//         //  _pos: new position
//         // OUTPUT:
//         //  true: success
//         //  false: position out of range or already occupied
//         bool SetPosition(Position _pos);
//         // Make move on the board
//         // OUTPUT:
//         //  0: success
//         //  1: position already taken
//         int execute() override;
// };

// class AddPlayerCommand : public ICommand{
//     private:
//         GameState* game;
//         Player* player;
//     public:
//         AddPlayerCommand(GameState* _game): game(_game), player(NULL){}
//         bool SelectPlayer(Player* _player);
//         void execute() override;
// };
// class RemovePlayerCommand : public ICommand{
//     private:
//         GameState* game;
//         int64_t player_id;
//     public:
//         RemovePlayerCommand(GameState* _game): game(_game), player_id(-1){}
//         bool SelectPlayer(int64_t _player_id);
//         void execute() override;
// };
// class AddBoardCommand : public ICommand{
//     private:
//         GameState* game;
//         Board* board;
//     public:
//         AddBoardCommand(GameState* _game): game(_game), board(NULL){}
//         bool SelectBoard(Board* _board);
//         void execute() override;
// };
// class RemoveBoardCommand : public ICommand{
//     private:
//         GameState* game;
//     public:

// };
#endif
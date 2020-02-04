#ifndef PLAYER_H
#define PLAYER_H

#include "GameObj.h"
#include "Command.h"
#include <queue>

class Player {
    private:
        int64_t id;
        Color color;
        static int64_t initCount;
        std::queue<ICommand*> commands;
    public:
        Player(Color _color = Color::none){
            this->initCount++;
            this->id = this->initCount;
            this->color = _color;
        }
        ~Player(){}
        bool operator==(const Player& _player) const {
            return this->id == _player.id;
        }
        int getID(){
            return this->id;
        }
        Color getColor(){
            return this->color;
        }
        bool setColor(Color _color){
            this->color = _color;
            return true;
        }
        // Store command in command queue
        // INPUT:
        //  + _command: pointer of command
        // OUTPUT:
        //  + true: success
        bool storeCommand(ICommand* _command);
        // Execute command from command queue
        // OUTPUT:
        //  + -1: no command in queue
        //  + n > 0: error code of command
        //  + 0: success
        int executeCommand();
        // // Pop command off command queue
        // // OUTPUT:
        // //  + true: success
        // //  + false: no command left
        // bool popCommand();
};
#endif
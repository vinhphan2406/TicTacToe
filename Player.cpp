#include "Player.h"

int64_t Player::initCount = 0;

bool Player::storeCommand(ICommand* _command){
    commands.push(_command);
    return true;
}

int Player::executeCommand(){
    if (!this->commands.empty()){
        int output_code = 0;
        if (this->commands.front() != NULL) {
            output_code = this->commands.front()->execute();
            delete this->commands.front();
            this->commands.pop();
        }
        return output_code;
    }
    return -1;
}

// bool Player::popCommand(){
//     if (this->commands.empty()) return false;
//     delete this->commands.front();
//     this->commands.pop();
//     return true;
// }
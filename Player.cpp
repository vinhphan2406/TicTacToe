#include "Player.h"

int64_t Player::initCount = 0;

bool Player::storeCommand(ICommand* _command){
    this->commands.push(_command);
    return true;
}

ICommand::Result Player::executeCommand(){
    if (!this->commands.empty()){
        ICommand::Result output_code = ICommand::Result::SUCCESS;
        if (this->commands.front() != NULL) {
            output_code = this->commands.front()->execute();
            delete this->commands.front();
            this->commands.pop();
        }
        return output_code;
    }
    return ICommand::Result::NO_COMMAND;
}

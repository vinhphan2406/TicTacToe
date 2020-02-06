#include "Player.h"

int64_t Player::initCount = 0;

bool Player::storeCommand(ICommand* _command){
    this->commands.push(_command);
    return true;
}

int Player::executeCommand(){
    if (!this->commands.empty()){
        int output_code = 0;
        if (this->commands.front() != NULL) {
            try {
                output_code = this->commands.front()->execute();
            }
            catch (std::exception& e){
                std::cout << e.what() << std::endl;
                this->commands.pop();
            }
            this->commands.pop();
        }
        return output_code;
    }
    return -1;
}

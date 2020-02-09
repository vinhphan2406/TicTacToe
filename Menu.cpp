#include "Menu.h"

bool MenuComponent::loadNextAction(ICommand* _command){
    this->commands.push_back(_command);
    return true;
}

bool MenuComponent::removeLastAction(){
    if (this->commands.back() != NULL) delete this->commands.back();
    this->commands.pop_back();
    return true;
}

ICommand::Result MenuComponent::doAction(){
    if (this->commands.empty()) return ICommand::Result::NO_COMMAND;
    ICommand::Result result;
    for (std::vector<ICommand*>::iterator it = this->commands.begin(); it != this->commands.end(); it++){
        if ((*it) != NULL) result = (*it)->execute();
        if (result != ICommand::Result::SUCCESS) break;
    }
    return result;
}

ICommand::Result ChooseOptionCommand::execute(){
    if (this->menu->hasChild()){
        try {
            this->menu->moveDown(this->option);
            if (this->menu->hasChild() == false) this->menu->moveUp();
            this->menu->getChildData(this->option)->doAction();
        }
        catch (std::invalid_argument& e){
            std::cout << e.what() << std::endl;
            return Result::OUT_OF_RANGE;
        }
        catch (std::out_of_range& e){
            std::cout << e.what() << std::endl;
            return Result::NOT_INITIALIZED;
        }
    }
    else {
        try {
            this->menu->getCurrentData()->doAction();
        }
        catch (std::invalid_argument& e){
            std::cout << e.what() << std::endl;
            return Result::OUT_OF_RANGE;
        }
    }
    return Result::SUCCESS;
}

ICommand::Result ReturnPreviousMenu::execute(){
    if (this->menu->hasChild() == true){
        this->menu->moveUp();
        if (this->menu->moveUp() == false) return Result::OUT_OF_RANGE;
    }
    else {
        this->menu->moveUp();
    }
    return Result::SUCCESS;
}


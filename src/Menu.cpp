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
            this->menu->moveDown(this->option - this->startNum);
            this->menu->getCurrentData()->doAction();
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

int DisplayMenu::print(){
    if (this->menu == NULL) return 1;
    if (this->menu->hasChild()){
        std::cout << this->menu->getCurrentData()->getTitle() << std::endl;
        if (this->printDesc == true) std::cout << this->menu->getCurrentData()->getDescription() << std::endl;
        int counter = 1;
        for (int i = 0; i < this->menu->getChildSize(); i++){
            if (this->menu->getChildData(i)->getHiddenState() == false){
                if (this->printIdx) std::cout << counter++ << ". ";
                std::cout << this->menu->getChildData(i)->getTitle() << std::endl;
            }
        }
    }
    return 0;
}

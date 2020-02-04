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

int MenuComponent::doAction(){
    int result = 0;
    for (std::vector<ICommand*>::iterator it = this->commands.begin(); it != this->commands.end(); it++){
        if ((*it) != NULL) result = (*it)->execute();
        if (result != 0) break;
    }
    return result;
}

int ChooseOptionCommand::execute(){
    if (this->menu->hasChild()){
        try {
            this->menu->moveDown(this->option);
            if (this->menu->hasChild() == false) this->menu->moveUp();
            this->menu->getChildData(this->option)->doAction();
        }
        catch (std::invalid_argument& e){
            std::cout << e.what() << std::endl;
            return 1;
        }
        catch (std::out_of_range& e){
            std::cout << e.what() << std::endl;
            return 2;
        }
    }
    else {
        try {
            this->menu->getCurrentData()->doAction();
        }
        catch (std::invalid_argument& e){
            std::cout << e.what() << std::endl;
            return 1;
        }
    }
    return 0;
}

int ReturnPreviousMenu::execute(){
    if (this->menu->hasChild() == true){
        this->menu->moveUp();
        if (this->menu->moveUp() == false) return 1;
    }
    else {
        this->menu->moveUp();
    }
    return 0;
}


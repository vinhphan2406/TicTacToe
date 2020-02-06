#include "Display.h"

int DisplayMenu::print(){
    if (this->menu == NULL) return 1;
    std::cout << this->menu->getCurrentData()->getTitle() << std::endl;
    if (this->printDesc == true) std::cout << this->menu->getCurrentData()->getDescription() << std::endl;
    if (this->menu->hasChild() == true){
        int counter = 1;
        for (int i = 0; i < this->menu->getChildSize(); i++){
            if (this->menu->getChildData(i)->getHiddenState() == false)
                if (this->printIdx) std::cout << counter++ << ". ";
                std::cout << this->menu->getChildData(i)->getTitle() << std::endl;
        }
    }
    return 0;
}
#include "Display.h"

// int DisplayBoard::print(){
//     for (int i = 0; i < this->board->getNumOfRow(); i++){
//         for (int j = 0; j < this->board->getNumOfCol(); j++){
//             Piece* piece = this->board->getSquare(Position(i,j)).getPiece();
//             if (piece != NULL){
//                 std::cout << (char)piece->getColor();
//             }
//             else std::cout << (char)Color::none;
//         }
//         std::cout << '\n';
//     }
//     return 0;
// }

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
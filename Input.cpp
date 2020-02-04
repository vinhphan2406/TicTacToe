#include "Input.h"

InputPosition::InputPosition(std::string _message){
    this->message = _message;
}

Position InputPosition::input(){
    int x, y;
    std::cout << this->message;
    try {
        std::cin >> x >> y;
    }
    catch (std::exception& e){
        std::cout << e.what() << std::endl;
        return Position();
    }
    return Position(x, y);
}
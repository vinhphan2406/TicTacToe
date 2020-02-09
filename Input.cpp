#include "Input.h"

#include "limits"

InputPosition::InputPosition(std::string _message, std::string _error_msg){
    this->message = _message;
    if (_error_msg != "") this->error_message = _error_msg;
    else this->error_message = _message;
}

Position InputPosition::input(){
    int x, y;
    std::cout << this->message;
    while (!(std::cin >> x >> y)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << this->error_message;
    }
    return Position(x, y);
}
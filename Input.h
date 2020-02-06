#include "GameObj.h"

#include <fstream>

class IInput {
    public:
        virtual ~IInput() = default;
        virtual Position input() = 0;
};

class InputPosition : public IInput {
    private:
        std::string message;
    public:
        InputPosition(std::string _message = "Input position: ");
        Position input() override;
};

class InputJSONFile {
    
};
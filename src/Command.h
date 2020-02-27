#ifndef I_COMMAND_H
#define I_COMMAND_H

class ICommand {
    public:
        virtual ~ICommand() = default;
        enum class Result {
            NO_COMMAND = -1,
            SUCCESS = 0,
            OUT_OF_RANGE = 1,
            OCCUPIED = 2,
            NOT_INITIALIZED = 3,
        };
        // Execute command
        // OUTPUT
        //  - 0: success
        //  - n > 0: error code
        virtual Result execute() = 0;
};
#endif
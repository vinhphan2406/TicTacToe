

#ifndef I_DISPLAY_H
#define I_DISPLAY_H

class IDisplay{
    private:
    public:
        virtual ~IDisplay() = default;
        // Print object to console output
        // OUTPUT:
        //  - 0: success
        //  - n > 0: error code
        virtual int print() = 0;
};

#endif



#ifndef I_DISPLAY_H
#define I_DISPLAY_H

#include "GameObj.h"
#include "Menu.h"

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

// class DisplayBoard : public IDisplay{
//     private:
//         Board* board;
//     public:
//         DisplayBoard(Board* _board): board(_board){}
//         // Print board to console output
//         // OUTPUT:
//         //  - 0: success
//         int print() override;
// };

class DisplayMenu : public IDisplay{
    private:
        Tree<MenuComponent>* menu;
        bool printDesc;
        bool printIdx;
    public:
        DisplayMenu(Tree<MenuComponent>* _menu, bool _desc = false, bool _idx = true): 
        menu(_menu), printDesc(_desc), printIdx(_idx){}
        // Print current submenu
        // OUTPUT:
        //  - 0: success
        int print() override;
};
#endif
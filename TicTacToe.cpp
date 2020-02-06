#include "GameObj.h"
#include "Player.h"
#include "Display.h"
#include "Menu.h"
#include "Game.h"

int main(){
    ICommand* run = new RunGameCommand();
    run->execute();
    delete run;
    return 0;
}
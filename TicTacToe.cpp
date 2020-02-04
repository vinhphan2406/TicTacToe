#include "GameObj.h"
#include "Player.h"
#include "Display.h"
#include "Menu.h"
#include "Game.h"

// int Game(Board* board, Player* player1, Player* player2){
//     int numOfTurns = 1;
//     DisplayBoard display(board);
//     int x,y;
//     Player* selectedPlayer = NULL;
//     int result;
    
//     while (true){
//         display.print();

//         if (numOfTurns % 2 == 0) selectedPlayer = player1;
//         else selectedPlayer = player2;

//         int error_code = 0;
//         do {
//             std::cout << "Player " << selectedPlayer->getID() << " turn: ";
//             std::cin >> x >> y;

//             selectedPlayer->storeCommand(new MakeMoveCommand(board, selectedPlayer->getColor(), Position(x,y)));
//             try {
//                 error_code = selectedPlayer->executeCommand();
//             }
//             catch (std::out_of_range& e){
//                 std::cout << e.what() << std::endl;
//                 error_code = 1;
//             }
//             catch (std::invalid_argument& e){
//                 std::cout << e.what() << std::endl;
//                 error_code = 2;
//             }
//             selectedPlayer->popCommand();
//         }
//         while (error_code != 0);

//         if (board->checkWinCondition(Position(x,y))){
//             result = (numOfTurns % 2 == 1)?1:-1;
//             break;
//         }

//         if (board->checkDrawCondition()){
//             result = 0;
//             break;
//         } 
//         numOfTurns++;
//     }
//     delete board;
//     delete player1;
//     delete player2;

//     return result;
// }

int main(){
    ICommand* run = new RunGameCommand();
    run->execute();
    delete run;
    return 0;
}
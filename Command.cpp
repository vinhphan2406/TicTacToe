#include "Command.h"

// MakeMoveCommand::MakeMoveCommand(Board* _board, Color _color, Position _pos){
//     if (_board == NULL) throw std::bad_alloc();
//     this->board = _board;
//     this->color = _color;
//     this->pos = _pos;
// }
// // bool MakeMoveCommand::SetPosition(Position _pos){
// //     if (this->pos.col < 0 || this->pos.col >= this->board->getNumOfCol()
// //     || this->pos.row < 0 || this->pos.row >= board->getNumOfRow()
// //     || this->board->getSquare(this->pos).checkOccupied() == true)
// //         return false;
// //     this->pos = _pos;
// //     return true;
// // }
// int MakeMoveCommand::execute(){
//     if (this->pos.col >= board->getNumOfCol() || this->pos.row >= board->getNumOfRow())
//         throw std::out_of_range("Position out of range!");
//     if (this->pos.col < 0 || this->pos.row < 0)
//         throw std::invalid_argument("Position not specified");
//     if (this->board->getSquare(this->pos).checkOccupied() == true)
//         return 1; 
//     this->board->getSquare(this->pos).setPiece(Piece(this->color));
//     return 0;
// }

// bool AddPlayerCommand::SelectPlayer(Player* _player){
//     if (_player == NULL) return false;
//     this->player = _player;
//     return true;
// }
// void AddPlayerCommand::execute() {
//     if (this->player == NULL) throw std::invalid_argument("Player not selected!");
//     else this->game->addPlayer(player);
// }

// bool RemovePlayerCommand::SelectPlayer(int64_t _player_id){
//     if (_player_id < 0) return false;
//     this->player_id = _player_id;
//     return true;
// }
// void RemovePlayerCommand::execute() {
//     if (this->player_id == -1) throw std::invalid_argument("Player ID invalid or not specified!");
//     else this->game->removePlayer(player_id);
// }

// bool AddBoardCommand::SelectBoard(){

// }

// class RemoveBoardCommand : public ICommand{

// };
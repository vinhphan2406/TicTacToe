#include "Game.h"

#include <sstream>

TicTacToe::TicTacToe(){
    this->board = new Board(BOARD_DIM_1, BOARD_DIM_2);
    this->player1 = new Player(Color::white);
    this->player2 = new Player(Color::black);
    this->selectedPlayer = this->player1;
    this->numOfTurns = 0;
    this->isRunning = true;
    this->nextMove = Position();
}

TicTacToe::~TicTacToe(){
    if (this->board != NULL) delete this->board;
    if (this->player1 != NULL) delete this->player1;
    if (this->player2 != NULL) delete this->player2;
}

bool TicTacToe::switchPlayer(){
    if (selectedPlayer != NULL){
        if (this->selectedPlayer == player2) this->selectedPlayer = player1;
        else this->selectedPlayer = player2;
    }
    else this->selectedPlayer = player1;
    return true;
}


bool TicTacToe::checkWin(Position _pos, int _mode){
    if (this->board == NULL) throw std::invalid_argument("Board not initialized!");
    if (_pos == Position()) return false;
    // if (this->listOfPlayers.size() != 2) throw std::invalid_argument("Player number invalid!");
    Position startPos, endPos;
    int step_x, step_y;
    switch (_mode){
        // Check 5-piece horizontal line
        case 0:
            startPos = _pos.fromPos(-(WIN_CONDITION - 1), 0);
            endPos = _pos.fromPos(WIN_CONDITION - 1, 0);
            step_x = 1;
            step_y = 0;
            break;
        // Check 5-piece vertical line
        case 1:
            startPos = _pos.fromPos(0, -(WIN_CONDITION - 1));
            endPos = _pos.fromPos(0, WIN_CONDITION - 1);
            step_x = 0;
            step_y = 1;
            break;
        // Check 5-piece left diagonal line (y = x)
        case 2:
            startPos = _pos.fromPos(-(WIN_CONDITION - 1), -(WIN_CONDITION - 1));
            endPos = _pos.fromPos(WIN_CONDITION - 1, WIN_CONDITION - 1);
            step_x = 1;
            step_y = 1;
            break;
        // Check 5-piece right diagonal line (y = -x)
        case 3:
            startPos = _pos.fromPos(-(WIN_CONDITION - 1), WIN_CONDITION - 1);
            endPos = _pos.fromPos(WIN_CONDITION - 1, -(WIN_CONDITION - 1));
            step_x = 1;
            step_y = -1;
            break;
        // Check 5-piece line
        default:
            return checkWin(_pos, 0) || checkWin(_pos, 1)
            || checkWin(_pos, 2) || checkWin(_pos, 3);
            break;
    }
    Position currentPos = startPos;
    Color currentColor = this->board->getSquare(_pos).getPiece()->getColor();
    int pieceCount = 0; 
    while (true){
        // Check if currentPos exist in board
        if (currentPos.row >= 0 && currentPos.row < this->board->getNumOfRow()
        && currentPos.col >= 0 && currentPos.col < this->board->getNumOfCol()){
            // Check if no piece in next square
            if (this->board->getSquare(currentPos).getPiece() != NULL){
                // Count if piece in next square
                if (this->board->getSquare(currentPos).getPiece()->getColor() == currentColor) pieceCount++;
                // Reset if different piece in next square
                else pieceCount = 0;
            }
            // Reset if no piece in next square
            else pieceCount = 0;
        }
        // Break when either 5-piece line exist or end of check region
        if (currentPos == endPos || pieceCount == WIN_CONDITION) break;
        // Advance current position by 1
        currentPos = currentPos.fromPos(step_x, step_y);
    }
    if (pieceCount == WIN_CONDITION) return true;
    return false;
}

bool TicTacToe::checkWin(){
    return checkWin(this->nextMove, -1);
}

// CheckWinCommand::CheckWinCommand(ICore* _core){
//     if (this->core == NULL) throw std::bad_alloc();
//     this->core = _core;
// }

// ICommand::Result CheckWinCommand::execute(){
//     if (this->core->checkWin()) return 0;
//     return 1;
// }

bool TicTacToe::checkDraw(){
    for (int i = 0; i < this->board->getNumOfRow(); i++){
        for (int j = 0; j < this->board->getNumOfCol(); j++){
            if (this->board->getSquare(Position(i,j)).checkOccupied() == false) return false;
        }
    }
    return true;
}

// CheckDrawCommand::CheckDrawCommand(ICore* _core){
//     if (this->core == NULL) throw std::bad_alloc();
//     this->core = _core;
// }

// ICommand::Result CheckDrawCommand::execute(){
//     if (this->core->checkDraw()) return 0;
//     return 1;
// }


bool TicTacToe::nextTurn(){
    this->numOfTurns++;
    return true;
}

int TicTacToe::getNumOfTurns() const{
    return this->numOfTurns;
}

Player* TicTacToe::getSelectedPlayer(){
    return this->selectedPlayer;
}

bool TicTacToe::printBoard(){
    for (int i = 0; i < this->board->getNumOfRow(); i++){
        for (int j = 0; j < this->board->getNumOfCol(); j++){
            Piece* piece = this->board->getSquare(Position(i,j)).getPiece();
            if (piece != NULL){
                std::cout << (char)piece->getColor();
            }
            else std::cout << (char)Color::none;
        }
        std::cout << '\n';
    }
    return true;
}

DisplayBoard::DisplayBoard(ICore* _core){
    if (_core == NULL) throw std::bad_alloc();
    this->core = _core;
}

DisplayBoard::~DisplayBoard(){
    this->core = NULL;
}

int DisplayBoard::print(){
    if (this->core->printBoard()) return 0;
    return 1;
}

ICommand::Result TicTacToe::makeMove(Position _pos){
    if (_pos.col >= board->getNumOfCol() || _pos.col < 0 
    || _pos.row >= board->getNumOfRow() || _pos.row < 0)
        return ICommand::Result::OUT_OF_RANGE;

    if (this->board->getSquare(_pos).checkOccupied() == true)
        return ICommand::Result::OCCUPIED; 

    this->board->getSquare(_pos).setPiece(Piece(this->selectedPlayer->getColor()));
    this->nextMove = _pos;

    return ICommand::Result::SUCCESS;
}

MakeMoveCommand::MakeMoveCommand(ICore* _core, Position _pos){
    if (_core == NULL) throw std::bad_alloc();
    this->core = _core;
    this->pos = _pos;
}

ICommand::Result MakeMoveCommand::execute(){
    return this->core->makeMove(this->pos);
}

DisplayMessage::DisplayMessage(ICore* _core, std::string _message){
    if (_core == NULL) throw std::bad_alloc();
    this->core = _core;
    this->message = _message;
}

int DisplayMessage::print(){
    std::cout << this->message << std::endl;
    return 0;
}

void Game::init(){
    this->core = new TicTacToe();
    this->display = new DisplayBoard(this->core);
    this->input = new InputPosition("Input next position: ", "Invalid input! Please re-enter: ");
    this->error_message = NULL;
    this->end_message = NULL;
}

bool Game::handleEvent(){
    // Skip first turn
    if (this->core->getNumOfTurns() == 0) return true;
    std::cout << "Player " << this->core->getSelectedPlayer()->getID() << " turn. ";
    Position nextMove;
    while (true){
        nextMove = this->input->input();
        if (!(nextMove == Position())) break;
    }
    this->core->getSelectedPlayer()->storeCommand(new MakeMoveCommand(this->core, nextMove));
    // std::stringstream output_stream;
    // output_stream << "Player " << this->core->getSelectedPlayer()->getID() << " wins." << std::endl;
    // this->events.push(new DisplayMessage(this->core, output_stream.str()));

    // this->events.push(new CheckDrawCommand(this->core));
    return true;
}

bool Game::update(){
    // Skip first turn
    if (this->core->getNumOfTurns() == 0){
        this->core->nextTurn();
        return true;
    }

    std::stringstream error_stream;
    std::stringstream end_stream;
    try {
        ICommand::Result result = this->core->getSelectedPlayer()->executeCommand();
        switch (result){
            case ICommand::Result::SUCCESS:
                if (this->core->checkWin()){
                    end_stream << "Player " << this->core->getSelectedPlayer()->getID() << " wins.";
                    this->core->isRunning = false;
                }
                else if (this->core->checkDraw()){
                    end_stream << "Game draws.";
                    this->core->isRunning = false;
                }
                this->core->switchPlayer();
                this->core->nextTurn();
                break;
            case ICommand::Result::OUT_OF_RANGE:
                error_stream << "Position: Out of range!";
                break;
            case ICommand::Result::OCCUPIED:
                error_stream << "Illegal move!";
                break;
            default:
                break;
        }
    }
    catch (std::exception& e){
        std::cout << "Exception occurred!\n" << e.what() << std::endl;
        this->core->isRunning = false;
        return false;
    }
    // Load error message if stream not empty
    if (error_stream.rdbuf()->in_avail() != 0) this->error_message = new DisplayMessage(this->core, error_stream.str());
    // Load end message if stream not empty
    if (end_stream.rdbuf()->in_avail() != 0) this->end_message = new DisplayMessage(this->core, end_stream.str());
    return true;
}

bool Game::render(){
    if (this->display != NULL){
        this->display->print();
    }
    // Print error message if available
    if (this->error_message != NULL){
        this->error_message->print();
        delete this->error_message;
        this->error_message = NULL;
        return false;
    }
    // Print end message if available
    if (this->end_message != NULL){
        this->end_message->print();
        delete this->end_message;
        this->end_message = NULL;
    }
    return true;
}

bool Game::clean(){
    if (this->core != NULL) delete this->core;
    if (this->display != NULL) delete this->display;
    if (this->input != NULL) delete this->input;
    this->core = NULL;
    this->display = NULL;
    this->input = NULL;
    return true;
}

bool Game::isExiting(){
    return !this->core->isRunning;
}

RunGameCommand::RunGameCommand(){
    this->game = new Game();
}

RunGameCommand::~RunGameCommand(){
    if (this->game != NULL) delete this->game;
    this->game = NULL;
}

ICommand::Result RunGameCommand::execute(){
    this->game->init();
    while (!game->isExiting()){
        this->game->handleEvent();
        this->game->update();
        this->game->render();
    }
    this->game->clean();
    return Result::SUCCESS;
}

// StartGameMenu::StartGameMenu(std::string _title, std::string _desc){
//     insertNode(new MenuComponent(_title, _desc));
//     insertNode(new MenuComponent("Start"));
//     getChildData(0)->loadNextAction(new RunGameCommand());
//     insertNode(new MenuComponent("Exit"));
// }

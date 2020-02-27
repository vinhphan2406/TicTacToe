#include "Game.h"

#include <sstream>

TicTacToeCore::TicTacToeCore(){
    this->board = new Board(BOARD_DIM_1, BOARD_DIM_2);
    this->player1 = new Player(Color::white);
    this->player2 = new Player(Color::black);
    this->selectedPlayer = this->player2;
    this->numOfTurns = 0;
    this->nextMove = Position();
}

TicTacToeCore::~TicTacToeCore(){
    if (this->board != NULL) delete this->board;
    if (this->player1 != NULL) delete this->player1;
    if (this->player2 != NULL) delete this->player2;
}

bool TicTacToeCore::switchPlayer(){
    if (selectedPlayer != NULL){
        if (this->selectedPlayer == player2) this->selectedPlayer = player1;
        else this->selectedPlayer = player2;
    }
    else this->selectedPlayer = player1;
    return true;
}


bool TicTacToeCore::checkWin(Position _pos, int _mode){
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

bool TicTacToeCore::checkWin(){
    return checkWin(this->nextMove, -1);
}

bool TicTacToeCore::checkDraw(){
    bool isDraw = true;
    this->board->traverse<bool>([](Square& _square, bool& _result){
        if (_square.checkOccupied() == false) _result = false;
    }, isDraw);
    return isDraw;
}


bool TicTacToeCore::nextTurn(){
    this->numOfTurns++;
    return true;
}

int TicTacToeCore::getNumOfTurns() const{
    return this->numOfTurns;
}

Player* TicTacToeCore::getSelectedPlayer() const {
    return this->selectedPlayer;
}

bool TicTacToeCore::printBoard(){
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

ICommand::Result TicTacToeCore::makeMove(Position _pos){
    if (_pos.col >= board->getNumOfCol() || _pos.col < 0 
    || _pos.row >= board->getNumOfRow() || _pos.row < 0)
        return ICommand::Result::OUT_OF_RANGE;

    if (this->board->getSquare(_pos).checkOccupied() == true)
        return ICommand::Result::OCCUPIED; 

    this->board->getSquare(_pos).setPiece(Piece(this->selectedPlayer->getColor()));
    this->nextMove = _pos;

    return ICommand::Result::SUCCESS;
}


DisplayMessage::DisplayMessage(std::string _message){
    this->message = _message;
}

int DisplayMessage::print(){
    std::cout << this->message << std::endl;
    return 0;
}

bool IGame::isExiting() const {
    return this->exit_state;
}

GameState IGame::getNextState() const {
    return this->next_state;
}

// -------------------
// IN GAME
// -------------------

void InGame::init(){
    this->core = new TicTacToeCore();
    this->main_display.push_back(new DisplayBoard(this->core));
    
    this->input = new InputPosition();
    this->exit_state = false;
    this->next_state = GameState::IN_GAME;
}

bool InGame::handleEvent(){
    // Skip first turn
    if (this->core->getNumOfTurns() == 0) {
        this->core->getSelectedPlayer()->storeCommand(new IdleCommand());
        return true;
    }
    // Input next move
    this->core->getSelectedPlayer()->storeCommand(new MakeMoveCommand(this->core, this->input->input().getObjectValue<Position>()));
    return true;
}

bool InGame::update(){
    try {
        // Execute command from selected player's perspective
        ICommand::Result result = this->core->getSelectedPlayer()->executeCommand();
        switch (result){
            case ICommand::Result::SUCCESS:
                if (this->core->checkWin()){
                    std::stringstream end_stream;
                    end_stream << "Player " << this->core->getSelectedPlayer()->getID() << " wins.";
                    this->message.push_back(new DisplayMessage(end_stream.str()));
                    this->exit_state = true;
                }
                else if (this->core->checkDraw()){
                    this->message.push_back(new DisplayMessage("Game draws."));
                    this->exit_state = true;
                }
                else {
                    this->core->switchPlayer();
                    this->core->nextTurn();
                    std::stringstream input_stream;
                    input_stream << "Player " << this->core->getSelectedPlayer()->getID() << " turns. Input position: ";
                    this->message.push_back(new DisplayMessage(input_stream.str()));
                }
                break;
            case ICommand::Result::OUT_OF_RANGE:
                this->message.push_back(new DisplayMessage("Position out of range! Re-input position: "));
                break;
            case ICommand::Result::OCCUPIED:
                this->message.push_back(new DisplayMessage("Illegal move! Re-input position: "));
                break;
            case ICommand::Result::NO_COMMAND:
                this->message.push_back(new DisplayMessage("No input command! Re-input position: "));
                break;
            default:
                break;
        }
    }
    catch (std::exception& e){
        std::stringstream exception_stream;
        exception_stream << "Exception occurred!\n" << e.what();
        this->message.push_back(new DisplayMessage(exception_stream.str()));
        this->exit_state = false;
        return false;
    }
    if (this->isExiting()){
        std::stringstream stats_stream;
        stats_stream << "Number of moves: " << this->core->getNumOfTurns();
        this->message.push_back(new DisplayMessage(stats_stream.str()));
        this->next_state = GameState::END_MENU;
    }
}

bool InGame::render(){
    // Print game elements
    for (std::vector<IDisplay*>::iterator it = this->main_display.begin(); it != this->main_display.end(); it++){
        (*it)->print();
    }

    // Print message if available (clear after print)
    for (std::vector<IDisplay*>::iterator it = this->message.begin(); it != this->message.end(); it++){
        (*it)->print();
        delete *it;
    }
    this->message.clear();

    return true;
}

bool InGame::clean(){
    if (this->core != NULL) delete this->core;
    if (this->input != NULL) delete this->input;
    this->core = NULL;
    this->input = NULL;

    // Release any game element initialized
    for (std::vector<IDisplay*>::iterator it = this->main_display.begin(); it != this->main_display.end(); it++){
        delete (*it);
    }
    this->main_display.clear();

    return true;
}

// -------------------
// START GAME MENU
// -------------------

void StartGameMenu::init(){
    this->menu = new Tree<MenuComponent>;
    std::unordered_map<std::string, int> menu_iter = {
        {"Start", 0},
        {"Exit", 1}
    };
    this->menu->insertNode(new MenuComponent("TicTacToe", "Current version: 0.2\n Made by lolipop"));

    this->menu->insertNode(new MenuComponent("Start"));
    this->menu->getChildData(menu_iter["Start"])->loadNextAction(new NextStateCommand(this, GameState::IN_GAME));
    this->menu->getChildData(menu_iter["Start"])->loadNextAction(new ExitStateCommand(this));

    this->menu->insertNode(new MenuComponent("Exit"));
    this->menu->getChildData(menu_iter["Exit"])->loadNextAction(new NextStateCommand(this, GameState::EXIT_GAME));
    this->menu->getChildData(menu_iter["Exit"])->loadNextAction(new ExitStateCommand(this));

    this->user = new Player();

    this->exit_state = false;
    this->next_state = GameState::START_MENU;
    this->start = false;

    this->main_display.push_back(new DisplayMenu(this->menu));

    this->input = new InputInt();
}

bool StartGameMenu::handleEvent(){
    if (!this->start){
        this->user->storeCommand(new IdleCommand());
        this->start = true;
        return true;
    }
    this->user->storeCommand(new ChooseOptionCommand(this->menu, this->input->input().getObjectValue<int>()));
    return true;
}

bool StartGameMenu::update(){
    ICommand::Result result = this->user->executeCommand();
    switch (result){
        case ICommand::Result::SUCCESS:
            if (!this->isExiting()){
                this->message.push_back(new DisplayMessage("Choose option: "));
            }
            break;
        default:
            this->message.push_back(new DisplayMessage("Invalid input! Re-choose option: "));
            break;
    }
    return true;
}

bool StartGameMenu::render(){
    // Print main elements (menu)
    for (std::vector<IDisplay*>::iterator it = this->main_display.begin(); it != this->main_display.end(); it++){
        (*it)->print();
    }

    // Print message if available
    for (std::vector<IDisplay*>::iterator it = this->message.begin(); it != this->message.end(); it++){
        (*it)->print();
        delete *it;
    }
    this->message.clear();

}

bool StartGameMenu::clean(){
    delete this->menu;
    delete this->input;
    delete this->user;

    // Release any menu element initialized
    for (std::vector<IDisplay*>::iterator it = this->main_display.begin(); it != this->main_display.end(); it++){
        delete (*it);
    }
    this->main_display.clear();

    // Release any menu message initialized
    for (std::vector<IDisplay*>::iterator it = this->message.begin(); it != this->message.end(); it++){
        delete (*it);
    }
    this->message.clear();
    return true;
}

// -------------------
// END GAME MENU
// -------------------

void EndGameMenu::init(){
    this->menu = new Tree<MenuComponent>;
    std::unordered_map<std::string, int> menu_iter = {
        {"Play again", 0},
        {"Go to main menu", 1}
    };
    this->menu->insertNode(new MenuComponent("Game ended."));

    this->menu->insertNode(new MenuComponent("Play again"));
    this->menu->getChildData(menu_iter["Play again"])->loadNextAction(new NextStateCommand(this, GameState::IN_GAME));
    this->menu->getChildData(menu_iter["Play again"])->loadNextAction(new ExitStateCommand(this));

    this->menu->insertNode(new MenuComponent("Go to main menu"));
    this->menu->getChildData(menu_iter["Go to main menu"])->loadNextAction(new NextStateCommand(this, GameState::START_MENU));
    this->menu->getChildData(menu_iter["Go to main menu"])->loadNextAction(new ExitStateCommand(this));

    this->user = new Player();

    this->exit_state = false;
    this->next_state = GameState::END_MENU;
    this->start = false;

    this->main_display.push_back(new DisplayMenu(this->menu));

    this->input = new InputInt();
}

bool EndGameMenu::handleEvent(){
    if (!this->start){
        this->user->storeCommand(new IdleCommand());
        this->start = true;
        return true;
    }
    this->user->storeCommand(new ChooseOptionCommand(this->menu, this->input->input().getObjectValue<int>()));
    return true;
}

bool EndGameMenu::update(){
    ICommand::Result result = this->user->executeCommand();
    switch(result){
        case ICommand::Result::SUCCESS:
            if (!this->isExiting()){
                this->message.push_back(new DisplayMessage("Choose option: "));
            }
            break;
        default:
            this->message.push_back(new DisplayMessage("Invalid input! Re-choose option: "));
            break;
    }
    return true;
}

bool EndGameMenu::render(){
    // Print main elements (menu)
    for (std::vector<IDisplay*>::iterator it = this->main_display.begin(); it != this->main_display.end(); it++){
        (*it)->print();
    }

    // Print message if available
    for (std::vector<IDisplay*>::iterator it = this->message.begin(); it != this->message.end(); it++){
        (*it)->print();
        delete *it;
    }
    this->message.clear();
    
    return true;
}

bool EndGameMenu::clean(){
    delete this->menu;
    delete this->input;
    delete this->user;
    // Release any game element initialized
    for (std::vector<IDisplay*>::iterator it = this->main_display.begin(); it != this->main_display.end(); it++){
        delete (*it);
    }
    this->main_display.clear();
    return true;
}
// -------------------
// GAME COMMAND
// -------------------

ICommand::Result NextStateCommand::execute(){
    this->component->next_state = this->newState;
    return Result::SUCCESS;
}

ICommand::Result ExitStateCommand::execute(){
    this->component->exit_state = true;
    return Result::SUCCESS;
}

MakeMoveCommand::MakeMoveCommand(ICore* _core, Position _pos){
    if (_core == NULL) throw std::bad_alloc();
    this->core = _core;
    this->pos = _pos;
}

ICommand::Result MakeMoveCommand::execute(){
    return this->core->makeMove(this->pos);
}

// -------------------
// GAME INPUT
// -------------------

Object InputPosition::input(){
    int x, y;
    while (!(std::cin >> x >> y)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << this->error_message;
    }
    return Object(Position(x - startNum, y - startNum));
}

Object InputInt::input(){
    int x;
    while (!(std::cin >> x)){
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << this->error_message;
    }
    return Object(x);
}
// -------------------
// GAME FRAMEWORK
// -------------------

bool GameFramework::run(){
    IGame* top_state = this->current_states.top();

    while (this->currentState_flag != GameState::EXIT_GAME){
        top_state->init();
        while (!top_state->isExiting()){
            top_state->handleEvent();
            top_state->update();
            top_state->render();
            if (top_state->getNextState() != this->currentState_flag){
                this->currentState_flag = top_state->getNextState();
                if (top_state->isExiting()){
                    top_state->clean();
                    this->current_states.pop();
                }
                this->current_states.push(this->states[this->currentState_flag]);
                top_state = this->current_states.top();
                break;
            }
        }
    }

    return true;
}
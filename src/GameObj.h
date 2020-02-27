#ifndef GAME_OBJ_H
#define GAME_OBJ_H

#include <iostream>
#include <exception>
#include <functional>


enum class Color {
    black = 'x',
    white = 'o',
    yellow = '*',
    green = '#',
    none = '-'
};

class Image {
    private:
    public:
};

struct Position {
    int col;
    int row;
    Position(int _row = -1, int _col = -1) : col(_col), row(_row){}
    Position(const Position& _piece){
        // Copy constructor
        this->col = _piece.col;
        this->row = _piece.row;
    }
    ~Position(){}
    void operator=(const Position& _pos){
        // Assignment operator
        this->col = _pos.col;
        this->row = _pos.row;
    }
    bool operator==(const Position& _pos) const{
        // Comparison operator
        if (this->col == _pos.col && this->row == _pos.row) return true;
        return false;
    }
    bool operator!=(const Position& _pos) const{
        return !operator==(_pos);
    }
    Position fromPos(int _row, int _col){
        // Get position from current position
        return Position(this->row + _row, this->col + _col);
    }
};

class Piece {
    private:
        Color color;
        Image* image;
    public:
        Piece(Color _color = Color::none) : color(_color){}
        Piece(const Piece& _piece){
            // Copy constructor
            this->color = _piece.color;
        }
        ~Piece(){}
        void operator=(const Piece& _piece){
            this->color = _piece.color;
        }
        bool operator==(const Piece& _piece) const{
            if (this->color == _piece.color) return true;
            return false;
        }
        bool setColor(Color _color){
            this->color = _color;
            return true;
        }
        Color getColor() const{
            return this->color;
        }
};

class Square {
    private:
        Piece* piece;
    public:
        Square(Piece* _piece = NULL){
            this->piece = _piece;
        }
        Square(const Square& _square){
            // Copy constructor
            if (_square.piece != NULL){                
                this->piece = new Piece(_square.piece->getColor());
            }
        }
        ~Square(){
            this->removePiece();
        }
        Piece* getPiece() const {
            return this->piece;
        }
        // Set piece on square
        // INPUT:
        // - _piece: info on piece
        // OUTPUT:
        // - true: 
        bool setPiece(const Piece& _piece){
            this->removePiece();
            this->piece = new Piece(_piece);
            return true;
        }
        // Remove piece from square
        // OUTPUT:
        // - true: if piece already occupied
        // - false: if piece not occupied 
        bool removePiece(){
            if (this->piece != NULL){
                delete this->piece;
                this->piece = NULL;
                return true;
            }
            return false;
        }
        // Check if square is occupied by a piece
        // OUTPUT:
        // - true: if occupied
        // - false: if not occupied
        bool checkOccupied(){
            return (this->piece != NULL);
        }
};

class Board {
    private:
        int numOfCol;
        int numOfRow;
        Square** square;
    public:
        Board(int _col = 1, int _row = 1){
            if (_col <= 0 || _row <= 0) throw std::invalid_argument("Board not specified!");
            this->numOfCol = _col;
            this->numOfRow = _row;
            this->square = new Square*[this->numOfRow];
            for (int i = 0; i < this->numOfCol; i++){
                this->square[i] = new Square[this->numOfCol];
            }
        }
        ~Board(){
            for (int i = 0; i < this->numOfCol; i++){
                delete[] square[i];
            }
            delete[] square;
        }
        bool setNumOfCol(int _col){
            if (_col <= 0) return false;
            this->numOfCol = _col;
            return true;
        }
        bool setNumOfRow(int _row){
            if (_row <= 0) return false;
            this->numOfCol = _row;
            return true;
        }
        int getNumOfCol() const{
            return this->numOfCol;
        }
        int getNumOfRow() const{
            return this->numOfRow;
        }
        Square& getSquare(Position _pos){
            if (_pos.col >= this->numOfCol && _pos.col < 0
            && _pos.row >= this->numOfRow && _pos.row < 0)
                throw std::out_of_range("Position out of range!");
            if (this->square == NULL || this->square[_pos.row] == NULL)
                throw std::bad_alloc();
            return this->square[_pos.row][_pos.col];
        }
        Square& operator[](Position _pos){
            this->getSquare(_pos);
        }
        template <class T>
        void traverse(std::function<void(Square&, T&)> f, T& pParam){
            for (int i = 0; i < this->numOfCol; i++){
                for (int j = 0; j < this->numOfRow; j++){
                    f(this->square[i][j], pParam);
                }
            }
        }
        void traverse(std::function<void(Square&)> f){
            for (int i = 0; i < this->numOfCol; i++){
                for (int j = 0; j < this->numOfRow; j++){
                    f(this->square[i][j]);
                }
            }
        }
};
#endif
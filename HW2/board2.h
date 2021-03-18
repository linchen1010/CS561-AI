#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <list>
#include <cmath>
#include <stack>
#include <set>
#include "piece.h"

#define ROWS  8
#define COLS  8

class Board {
    public:
        int white_left;
        int black_left;
        int white_king;
        int black_king;
        vector<Piece> b_piece; // store black piece info // should use map to(?)
        vector<Piece> w_piece; // store white piece info
        vector<vector<char> > board;
        vector<pair<int, int>> path;
        
        vector<Piece> get_all_piece(char color);
        vector<Board> get_all_moves(Piece piece);

        void piece_jump(Piece piece, int row_dir, Board cur_board, vector<Board> &boards);
        void king_jump(Piece piece, Board cur_board, vector<Board> &boards);
        void piece_move(Piece piece, int row_dir, int col_dir, Board cur_board, vector<Board> &boards);
        bool can_jump(Piece piece);
        int evaluate(string color);

        void init_board(); // init board informa tion
        void get_piece_info(); // get piece info from given board.
        void print_board();
        

    private:
        bool could_jump(Piece piece, int row_dir, int col_dir, Board &cur_board);
        Piece get_piece(int row, int col);

};

int Board::evaluate(string color) {
    if(color == "WHITE") return this->white_left - this->black_left + (this->white_king * 2 - this->black_king * 2);
    else return this->black_left - this->white_left + (this->black_king * 2 - this->white_king * 2);
}

vector<Board> Board::get_all_moves(Piece piece) {
    
    vector<Board> boards;
    Board cur_board = *this;
   
    if(piece.isKing) {
        //cout << "2123";
        if(cur_board.can_jump(piece)) {
            king_jump(piece, cur_board, boards);
        } else {
            piece_move(piece, -1, -1, cur_board, boards);
            piece_move(piece, -1, 1, cur_board, boards);
            piece_move(piece, 1, -1, cur_board, boards);
            piece_move(piece, 1, 1, cur_board, boards);
        }
    }
    else if(piece.color == 'b') {
        if(cur_board.can_jump(piece)) {
            piece_jump(piece, 1, cur_board, boards);
        } else {
            piece_move(piece, 1, -1, cur_board, boards);
            piece_move(piece, 1, 1, cur_board, boards);
        }
        
    }
    else if(piece.color == 'w') {
        if(cur_board.can_jump(piece)) {
            piece_jump(piece, -1, cur_board, boards);
        } else {
            piece_move(piece, -1, -1, cur_board, boards);
            piece_move(piece, -1, 1, cur_board, boards);
        }
    }
    return boards;
}

void Board::king_jump(Piece piece, Board cur_board, vector<Board> &boards) {
    if(!cur_board.can_jump(piece)) {
        cur_board.init_board();
        cur_board.get_piece_info();
        boards.push_back(cur_board);
        return;
    }
    
    vector<int> col_dir {-1, 1};
    vector<int> row_dir {-1, 1};

    for(int row: row_dir) {
        for(int col : col_dir) {
            if(could_jump(piece, row, col, cur_board)) {
                Piece new_piece(piece.row + 2*row, piece.col + 2*col, piece.color, true);
                Board copy = cur_board;
                copy.board[piece.row + row][piece.col + col] = '.';
                copy.path.push_back({new_piece.row, new_piece.col});
                swap(copy.board[piece.row][piece.col], copy.board[new_piece.row][new_piece.col]);

                // if reach end, make king
                king_jump(new_piece, copy, boards);
            }
        }
    }
}

void Board::piece_jump(Piece piece, int row_dir, Board cur_board, vector<Board> &boards) {
    if(!cur_board.can_jump(piece)) {
        cur_board.init_board();
        cur_board.get_piece_info();
        boards.push_back(cur_board);
        return;
    }
    vector<int> col {-1,1};
    
    for(int col_dir : col) {
        if(could_jump(piece, row_dir, col_dir, cur_board)) {
            Piece new_piece(piece.row + 2*row_dir, piece.col + 2*col_dir, piece.color, false);
            Board copy = cur_board;
            copy.board[piece.row + row_dir][piece.col + col_dir] = '.';
            copy.path.push_back({new_piece.row, new_piece.col});
            swap(copy.board[piece.row][piece.col], copy.board[new_piece.row][new_piece.col]);

            // if reach end, make king
            if((new_piece.row == 0 || new_piece.row == ROWS-1)) {
                copy.board[new_piece.row][new_piece.col] = toupper(copy.board[new_piece.row][new_piece.col]);
                copy.init_board();
                copy.get_piece_info();
                boards.push_back(copy);
                return;
            }
            piece_jump(new_piece, row_dir, copy, boards);
        }
    }
}

bool Board::could_jump(Piece piece, int row_dir, int col_dir, Board &cur_board) {
    // boundary check
    int nRow = piece.row + row_dir;
    int nCol = piece.col + col_dir;
    int nnRow = piece.row + 2 * row_dir;
    int nnCol = piece.col + 2 * col_dir;

    if(nRow < 0 || nRow >= ROWS || nCol < 0 || nCol >= COLS) return false;
    if(nnRow < 0 || nnRow >= ROWS || nnCol < 0 || nnCol >= COLS) return false;
    
    if(cur_board.board[nRow][nCol] == '.' || cur_board.board[nRow][nCol] == piece.color) return false;
    else {
        if(cur_board.board[nnRow][nnCol] == '.') return true;
    }
    return false;
}

bool Board::can_jump(Piece piece) {
    if(piece.isKing) return could_jump(piece, -1, -1, *this) || could_jump(piece, -1, 1, *this) || could_jump(piece, 1, -1, *this) || could_jump(piece, 1, 1, *this);
    else if(piece.color == 'w') return could_jump(piece, -1, -1, *this) || could_jump(piece, -1, 1, *this);
    else if(piece.color == 'b') return could_jump(piece, 1, -1, *this) || could_jump(piece, 1, 1, *this);
    
    return false;
}

void Board::piece_move(Piece piece, int row_dir, int col_dir, Board cur_board, vector<Board> &boards) {
    if(piece.row + row_dir < 0 || piece.row + row_dir >= ROWS || piece.col + col_dir < 0 || piece.col + col_dir >= COLS) return;
    if(cur_board.board[piece.row+row_dir][piece.col+col_dir] == '.') {
        Board copy = cur_board;
        Piece new_piece(piece.row + row_dir, piece.col + col_dir, piece.color, piece.isKing);
        copy.path.push_back({new_piece.row, new_piece.col});
        swap(copy.board[piece.row][piece.col], copy.board[new_piece.row][new_piece.col]);
        boards.push_back(copy);
    }
}

Piece Board::get_piece(int row, int col) {
    if(this->board[row][col] == 'W' || this->board[row][col] == 'B') {
        return Piece(row, col, tolower(this->board[row][col]), true);
    } else return Piece(row, col, this->board[row][col], false);
}   

void Board::get_piece_info() {
    
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLS; col++) {
            if(this->board[row][col] == 'w') {
                this->white_left++;
                w_piece.push_back(Piece(row, col, 'w', false));
            }
            else if(this->board[row][col] == 'W') {
               // this->white_left++;
                this->white_king++;
                w_piece.push_back(Piece(row, col, 'w', true));
            }
            else if(this->board[row][col] == 'b') {
                this->black_left++;
                b_piece.push_back(Piece(row, col, 'b', false));
            }
            else if(this->board[row][col] == 'B') {
                //this->black_left++;
                this->black_king++;
                b_piece.push_back(Piece(row, col, 'b', true));
            }
        }
    }
}

void Board::init_board() {
    this->white_left = 0;
    this->black_left = 0;
    this->white_king = 0;
    this->black_king = 0;
    this->b_piece.clear();
    this->w_piece.clear();
}

void Board::print_board() {
    for(int i = 0; i < 8; i++) {
        for(int j = 0; j < 8; j++) {
            cout << this->board[i][j];
        }
        cout << "\n";
    }
}
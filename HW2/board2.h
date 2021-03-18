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

        //void piece_move(Piece selected_piece, Piece cur_piece, int row_dir, int col_dir, Board cur_board, vector<Board> &boards);
        //void piece_jump(Piece piece, Board cur_board, int row, int col, vector<Piece> skipped, vector<Piece> path, vector<Board> &boards);
        void piece_jump(Piece piece, int row_dir, int col_dir, Board cur_board, vector<Board> &boards);
        bool valid(Piece piece, int row, int col);
        //void piece_move(Piece selected_piece, int start_row, int stop_row, int row_dir, int col_dir, Board cur_board, vector<Piece> skipped, vector<Board> &boards);

        vector<Board> dfs(Piece piece, int row, int col, int row_dir, int col_dir, Board cur_board, vector<Piece> skipped);

        void init_board(); // init board informa tion
        void get_piece_info(vector<vector<char> > board); // get piece info from given board.
        void print_board();
        void remove(vector<Piece> skipped);
        void move(Piece start, Piece end);
        bool can_jump(Piece piece);

    private:
        bool could_jump(Piece piece, int row_dir, int col_dir, Board &cur_board);
        Piece get_piece(int row, int col);
        void board_update(vector<vector<char> > &board, vector<Piece> skipped);
        void set_new_board(Piece start, Piece end, vector<Piece> skipped);
};

vector<Board> Board::get_all_moves(Piece piece) {
    vector<Board> boards;
    

    Board cur_board = *this;
    vector<Piece> skipped;
    vector<Piece> path;
    int up = -1, left = -1;
    int down = 1, right = 1;

    //moveBoard.push_back(moved_board);
    if(piece.isKing) {
        // king_jump/move
    }
    else if(piece.color == 'b') {
        if(could_jump(piece, down, left, cur_board)) {
            piece_jump(piece, down, left, cur_board, boards);
        }
        if(could_jump(piece, down, right, cur_board)) {
            cout << "###" << endl;
            piece_jump(piece, down, right, cur_board, boards);
        }
    }
    else if(piece.color == 'w') {
        if(cur_board.can_jump(piece)) piece_jump(piece, -1, 1, cur_board, boards);
    }
    return boards;
}

void Board::piece_jump(Piece piece, int row_dir, int col_dir, Board cur_board, vector<Board> &boards) {
    if(!cur_board.can_jump(piece)) {
        boards.push_back(cur_board);
        return;
    }
    // cout << piece.color << endl;
    
    if(could_jump(piece, row_dir, col_dir, cur_board)) {
        Piece new_piece(piece.row + 2*row_dir, piece.col + 2*col_dir, piece.color, false);
        Board copy = cur_board;
        copy.board[piece.row + row_dir][piece.col + col_dir] = '.';
        swap(copy.board[piece.row][piece.col], copy.board[piece.row + 2*row_dir][piece.col + 2 * col_dir]);

        // if reach end, make king
        if((piece.row + 2 * row_dir == 0 || piece.row + 2 * row_dir == ROWS-1)) {
            copy.board[piece.row + 2*row_dir][piece.col + 2 *col_dir] = toupper(copy.board[piece.row + 2*row_dir][piece.col + 2 *col_dir]);
            boards.push_back(copy);
            return;
        }

        piece_jump(new_piece, row_dir, col_dir, copy, boards);
    }
    col_dir = -col_dir;
    if(could_jump(piece, row_dir, col_dir, cur_board)) {
        Piece new_piece(piece.row + 2*row_dir, piece.col + 2*col_dir, piece.color, false);
        Board copy = cur_board;
        copy.board[piece.row + row_dir][piece.col + col_dir] = '.';
        swap(copy.board[piece.row][piece.col], copy.board[piece.row + 2*row_dir][piece.col + 2 * col_dir]);

        // if reach end, make king
        if((piece.row + 2 * row_dir == 0 || piece.row + 2 * row_dir == ROWS-1)) {
            copy.board[piece.row + 2*row_dir][piece.col + 2 *col_dir] = toupper(copy.board[piece.row + 2*row_dir][piece.col + 2 *col_dir]);
            boards.push_back(copy);
            return;
        }
        piece_jump(new_piece, row_dir, col_dir, copy, boards);
    }
        
    
     
}

bool Board::could_jump(Piece piece, int row_dir, int col_dir, Board &cur_board) {
    // boundary check
    int nRow = piece.row + row_dir;
    int nCol = piece.col + col_dir;
    int nnRow = piece.row + 2 * row_dir;
    int nnCol = piece.col + 2 * col_dir;

    if(nRow < 0 || nRow >= ROWS || nCol < 0 || nCol >= COLS) return false;
    //cout << "1" << endl;
    if(nnRow < 0 || nnRow >= ROWS || nnCol < 0 || nnCol >= COLS) return false;
    
    if(cur_board.board[nRow][nCol] == '.' || cur_board.board[nRow][nCol] == piece.color) return false;
    else {
        if(cur_board.board[nnRow][nnCol] == '.') return true;
    }
    return false;
}

bool Board::can_jump(Piece piece) {
    if(piece.color == 'w') return could_jump(piece, -1, -1, *this) || could_jump(piece, -1, 1, *this);
    else if(piece.color == 'b') return could_jump(piece, 1, -1, *this) || could_jump(piece, 1, 1, *this);
    return false;
}

void Board::set_new_board(Piece start, Piece end, vector<Piece> skipped) {
    this->move(start, end);
    this->init_board();
    this->get_piece_info(this->board);
    this->remove(skipped);
}

void Board::move(Piece start, Piece end) {
    
    char tmp = this->board[start.row][start.col];
    this->board[start.row][start.col] = this->board[end.row][end.col];
    this->board[end.row][end.col] = tmp;

    if(end.row == 0 || end.row == ROWS - 1) {
        this->board[end.row][end.col] = toupper(this->board[end.row][end.col]);
        if(end.color == 'w') {
            this->white_king += 1;
            this->white_left -= 1;
        } else {
            this->black_king += 1;
            this->black_left -= 1;
        }
    }
}

void Board::remove(vector<Piece> skipped) {
    for(Piece piece : skipped) {
        this->board[piece.row][piece.col] = '.';
        if(piece.color == 'w') {
            this->white_left--;
            if(piece.isKing) this->white_king --;
        } 
        else {
            this->black_left--;
            if(piece.isKing) this->black_king --;
        }
    }
}

Piece Board::get_piece(int row, int col) {
    if(this->board[row][col] == 'W' || this->board[row][col] == 'B') {
        return Piece(row, col, tolower(this->board[row][col]), true);
    } else return Piece(row, col, this->board[row][col], false);
}   

void Board::get_piece_info(vector<vector<char> > board) {
    this->board = board;
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLS; col++) {
            if(board[row][col] == 'w') {
                this->white_left++;
                w_piece.push_back(Piece(row, col, 'w', false));
            }
            else if(board[row][col] == 'W') {
               // this->white_left++;
                this->white_king++;
                w_piece.push_back(Piece(row, col, 'w', true));
            }
            else if(board[row][col] == 'b') {
                this->black_left++;
                b_piece.push_back(Piece(row, col, 'b', false));
            }
            else if(board[row][col] == 'B') {
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
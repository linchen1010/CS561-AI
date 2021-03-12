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
#include "piece.h"

#define ROWS  8
#define COLS  8

using namespace std;

class Board {
    public:
        int white_left;
        int black_left;
        int white_king;
        int black_king;
        vector<Piece> b_piece; // store black piece info // should use map to(?)
        vector<Piece> w_piece; // store white piece info
        vector<vector<char> > board;
        vector<Piece> get_all_piece(char color);
        Piece get_piece(int row, int col);
        vector<vector<char> > create_board_after_move(vector<vector<char> > &board, Piece selected);
        unordered_map<string, vector<Piece> > get_valid_moves(Piece piece); // get valid move via traverse in 4 direction
        void traverse_left_up(int start_row, int stop_row, char color, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves);
        void traverse_right_up(int start_row, int stop_row, char color, int right, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves);
        void traverse_left_down(int start_row, int stop_row, char color, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves);
        void traverse_right_down(int start_row, int stop_row, char color, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves);
        //
        
        void init_board(); // init board informa tion
        void get_piece_info(vector<vector<char> > board); // get piece info from given board.
        void print_board();
        
        
        //Piece get_piece(int row, int col);
};

vector<Piece> Board::get_all_piece(char color) {
    vector<Piece> pieces;
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            if(this->board[row][col] == color) pieces.push_back(get_piece(row,col));
        }
    }
    return pieces;
}

unordered_map<string, vector<Piece> > Board::get_valid_moves(Piece piece) {
    unordered_map<string, vector<Piece> > moves;
    vector<Piece> skipped;
    int left = piece.col - 1;
    int right = piece.col + 1;
    int row = piece.row;

    if(piece.color == 'b'  || piece.isKing) {
        traverse_left_down(row + 1, min(row + 3, ROWS), piece.color, left, skipped, moves);
        traverse_right_down(row + 1, min(row + 3, ROWS), piece.color, right, skipped, moves);
    }
    if(piece.color == 'w' || piece.isKing) {
        traverse_left_up(row - 1, max(row - 3, -1), piece.color, left, skipped, moves);
        traverse_right_up(row - 1, max(row - 3, -1), piece.color, right, skipped, moves);
    }

    return moves;
}

void Board::traverse_left_up(int start_row, int stop_row, char color, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves) {
    vector<Piece> last;
    //cout << "#==" << endl;
    for(int row = start_row; row > stop_row; row--) {
        if(left < 0) break; // bundary check
        Piece curr_piece = this->get_piece(row,left);
        //cout << row << "," << left << endl;
        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                break;
            } else if(!skipped.empty()) {
                vector<Piece> tmp = last;
                tmp.insert(tmp.end(), skipped.begin(), skipped.end());
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = tmp;
            } else {
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = last;
            }
            if(!last.empty()) {
                int row_stop = max(row-3, 0);  
                traverse_left_up(row-1, row_stop, color, left-1, last, moves);
                traverse_right_up(row-1, row_stop, color, left+1, last, moves);
            }
            break;
        }
        else if(curr_piece.color == color) {
            break;
        }
        else {
            last.push_back(curr_piece);
        }
        left --; 
    }
}

void Board::traverse_right_up(int start_row, int stop_row, char color, int right, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves) {
    vector<Piece> last;
    //cout << "--" << left << endl;
    //cout << start_row << "-" << stop_row << "-" << right << endl;
    for(int row = start_row; row > stop_row; row--) {
        //cout << "1" << endl;
        if(right >= COLS) break; // bundary check
        Piece curr_piece = this->get_piece(row,right);
        //cout << curr_piece.row << ", " << curr_piece.col << " " << curr_piece.color << endl;
        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                //cout << "2" << endl;
                break;
            } else if(!skipped.empty()) {
                //cout << "3" << endl;
                vector<Piece> tmp = last;
                tmp.insert(tmp.end(), skipped.begin(), skipped.end());
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = tmp;
            } else {
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = last;
            }
            if(!last.empty()) { // handle skip
                int row_stop = max(row-3, 0);
                traverse_left_up(row-1, row_stop, color, right-1, last, moves);
                traverse_right_up(row-1, row_stop, color, right+1, last, moves);
            }
            break;
        }
        else if(curr_piece.color == color) break;
        else last.push_back(curr_piece);

        right ++;
    }
}

void Board::traverse_left_down(int start_row, int stop_row, char color, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves) {
    vector<Piece> last;
    for(int row = start_row; row < stop_row; row++) {
        if(left < 0) break; // bundary check
        Piece curr_piece = this->get_piece(row,left);
       
        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                break;
            } else if(!skipped.empty()) {
                vector<Piece> tmp = last;
                tmp.insert(tmp.end(), skipped.begin(), skipped.end());
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = tmp;
            } else {
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = last;
            }
            if(!last.empty()) {
                int row_stop = min(row+3, ROWS);  
                traverse_left_down(row+1, row_stop, color, left-1, last, moves);
                traverse_right_down(row+1, row_stop, color, left+1, last, moves);
            }
            break;
        }
        else if(curr_piece.color == color) {
            break;
        }
        else {
            last.push_back(curr_piece);
        }
        left --; 
    }
}

void Board::traverse_right_down(int start_row, int stop_row, char color, int right, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves) {
    vector<Piece> last;
    for(int row = start_row; row < stop_row; row++) {
        if(right >= COLS) break; // bundary check
        Piece curr_piece = this->get_piece(row,right);
        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                break;
            } else if(!skipped.empty()) {
                vector<Piece> tmp = last;
                tmp.insert(tmp.end(), skipped.begin(), skipped.end());
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = tmp;
            } else {
                moves[to_string(curr_piece.row)+to_string(curr_piece.col)] = last;
            }
            if(!last.empty()) {
                int row_stop = min(row+3, ROWS);  
                traverse_left_down(row+1, row_stop, color, right-1, last, moves);
                traverse_right_down(row+1, row_stop, color, right+1, last, moves);
            }
            break;
        }
        else if(curr_piece.color == color) {
            break;
        }
        else {
            last.push_back(curr_piece);
        }
        right++; 
    }
}


Piece Board::get_piece(int row, int col) {
    if(this->board[row][col] == 'w') {
        return Piece(row, col, 'w', false);
    }
    else if(this->board[row][col] == 'W') {
        return Piece(row, col, 'w', true);
    }
    else if(this->board[row][col] == 'b') {
        return Piece(row, col, 'b', false);
    }
    else if(this->board[row][col] == 'B') {
        return Piece(row, col, 'b', true);
    }
    else return Piece(row, col, '.', false);
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
                this->white_left++;
                this->white_king++;
                w_piece.push_back(Piece(row, col, 'w', true));
            }
            else if(board[row][col] == 'b') {
                this->black_left++;
                b_piece.push_back(Piece(row, col, 'b', false));
            }
            else if(board[row][col] == 'B') {
                this->black_left++;
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






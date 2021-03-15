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
        unordered_map<string, vector<Piece> > get_valid_moves(Piece piece); // get valid move via traverse in 4 direction


        void traverse_left_up(int start_row, int stop_row, Piece piece, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board);
        void traverse_right_up(int start_row, int stop_row, Piece piece, int right, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board);
        void traverse_left_down(int start_row, int stop_row, Piece piece, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board);
        void traverse_right_down(int start_row, int stop_row, Piece piece, int right, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board);
        //
        
        void init_board(); // init board informa tion
        void get_piece_info(vector<vector<char> > board); // get piece info from given board.
        void print_board();
        void move(Piece piece, int row, int col);
        void remove(vector<Piece> skipped);
        int evaluate(string color);
        string winner();

    private:
        string pos_to_str(int row, int col);
        Piece get_piece_from_board(vector<vector<char> > board, int row, int col);
        void board_update(vector<vector<char> > &board, vector<Piece> skipped);
        bool same_goal(unordered_map<string, vector<Piece> > moves, vector<Piece> skipped, int row, int col);
};

bool Board::same_goal(unordered_map<string, vector<Piece> > moves, vector<Piece> skipped, int row, int col) {
    string pos = pos_to_str(row, col);
    if(moves.count(pos) && moves[pos].size() == skipped.size()) {
        return true;
    } else {
        return false;
    }
}

Piece Board::get_piece_from_board(vector<vector<char> > board, int row, int col) {
    if(board[row][col] == 'W' || board[row][col] == 'B') {
        return Piece(row, col, tolower(board[row][col]), true);
    } else return Piece(row, col, board[row][col], false);
}

void Board::board_update(vector<vector<char> > &board, vector<Piece> skipped) {
    for(Piece piece : skipped) {
        board[piece.row][piece.col] = '.';
    }
}

int Board::evaluate(string color) {
    if(color == "WHITE") return this->white_left - this->black_left + (this->white_king * 2 - this->black_king * 2);
    else return this->black_left - this->white_left + (this->black_king * 2 - this->white_king * 2);
}

string Board::winner() {
    if(this->white_left <= 0) {
        return "BLACK";
    } else if(this->black_left <= 0) {
        return "WHITE";
    }
    return "NONE";
}

string Board::pos_to_str(int row, int col) {
    return to_string(row)+to_string(col);
}

void Board::move(Piece piece, int row, int col) {

    char tmp = this->board[piece.row][piece.col];
    this->board[piece.row][piece.col] = this->board[row][col];
    this->board[row][col] = tmp;
    piece.move(row, col);

    if(row == 0 || row == ROWS - 1) {
        
        this->board[row][col] = toupper(this->board[row][col]);
        piece.isKing = true;
        if(piece.color == 'W') {
            this->white_king += 1;
        } else {
            this->black_king += 1;
        }
    }
}

void Board::remove(vector<Piece> skipped) {
    for(Piece piece: skipped) {
        this->board[piece.row][piece.col] = '.';
        if(piece.color == 'w') {
            this->white_left--;
        } 
        else {
            this->black_left--;
        }
    }
}



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
    
    vector<vector<char> > board = this->board;
    board[piece.row][piece.col] = '.';
    if(piece.color == 'b'  || piece.isKing) {
        traverse_left_down(row + 1, min(row + 3, ROWS), piece, left, skipped, moves, board);
        board = this->board;
        board[piece.row][piece.col] = '.';
        traverse_right_down(row + 1, min(row + 3, ROWS), piece, right, skipped, moves, board);
    }
    if(piece.color == 'w' || piece.isKing) {
        board = this->board;
        board[piece.row][piece.col] = '.';
        traverse_left_up(row - 1, max(row - 3, -1), piece, left, skipped, moves, board);
        board = this->board;
        board[piece.row][piece.col] = '.';
        traverse_right_up(row - 1, max(row - 3, -1), piece, right, skipped, moves, board);
    }

    return moves;
}

void Board::traverse_left_up(int start_row, int stop_row, Piece piece, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board) {
    if(start_row < 0 || start_row >= ROWS || stop_row < 0 || stop_row >= ROWS) return;

    vector<Piece> last;

    for(int row = start_row; row > stop_row; row--) {

        if(left < 0) break; // bundary check
        Piece curr_piece = get_piece_from_board(board, row, left);

        //cout << curr_piece.row << "," << curr_piece.col << " " << curr_piece.color << endl;

        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                break;
            } else if(!skipped.empty()) {
                last.insert(last.end(), skipped.begin(), skipped.end()); // last + skipped
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            } else {
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            }
            if(!last.empty()) {
                board_update(board, last);
                if(piece.isKing) {
                    int row_stop_up = max(row - 3, 0);
                    int row_stop_down = min(row + 3, ROWS);
                    traverse_left_up(row-1, row_stop_up, piece, left-1, last, moves, board);
                    traverse_right_up(row-1, row_stop_up, piece, left+1, last, moves, board);
                    traverse_left_down(row+1, row_stop_down, piece, left-1, last, moves, board);
                    traverse_right_down(row+1, row_stop_down, piece, left+1, last, moves, board);
                }
                else {
                    int row_stop = max(row-3, 0);
                    traverse_left_up(row-1, row_stop, piece, left-1, last, moves, board);
                    traverse_right_up(row-1, row_stop, piece, left+1, last, moves, board);
                }
            }
            break;
        }
        else if(curr_piece.color == piece.color) {
            break;
        }
        else {
            last.push_back(curr_piece);
        }
        left --; 
    }
}

void Board::traverse_right_up(int start_row, int stop_row, Piece piece, int right, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board) {
    if(start_row < 0 || start_row >= ROWS || stop_row < 0 || stop_row >= ROWS) return;

    vector<Piece> last;

    for(int row = start_row; row >= stop_row; row--) {

        if(right >= COLS) break; // bundary check
        Piece curr_piece = get_piece_from_board(board, row, right);

        //cout << curr_piece.row << "," << curr_piece.col << "-" << curr_piece.color << "--" << stop_row << endl;

        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                break;
            } else if(!skipped.empty()) {
                last.insert(last.end(), skipped.begin(), skipped.end()); // last + skipped
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            } else {
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            }
            if(!last.empty()) { // handle skip
                board_update(board, last);
                if(piece.isKing) {
                    int row_stop_up = max(row - 3, 0);
                    int row_stop_down = min(row + 3, ROWS);
                    traverse_left_up(row-1, row_stop_up, piece, right-1, last, moves, board);
                    traverse_right_up(row-1, row_stop_up, piece, right+1, last, moves, board);
                    traverse_left_down(row+1, row_stop_down, piece, right-1, last, moves, board);
                    traverse_right_down(row+1, row_stop_down, piece, right+1, last, moves, board);
                }
                else {
                    int row_stop = max(row-3, 0);
                    traverse_left_up(row-1, row_stop, piece, right-1, last, moves, board);
                    traverse_right_up(row-1, row_stop, piece, right+1, last, moves, board);
                }
            }
            break;
        }
        else if(curr_piece.color == piece.color) break;
        else {
            last.push_back(curr_piece);
        }

        right ++;
    }
}

void Board::traverse_left_down(int start_row, int stop_row, Piece piece, int left, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board) {
    if(start_row < 0 || start_row >= ROWS || stop_row < 0 || stop_row >= ROWS) return;

    vector<Piece> last;

    for(int row = start_row; row < stop_row; row++) {

        if(left < 0) break; // bundary check
        Piece curr_piece = get_piece_from_board(board, row, left);

        //cout << curr_piece.row << "," << curr_piece.col << " " << curr_piece.color << endl;

        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                break;
            } else if(!skipped.empty()) {
                last.insert(last.end(), skipped.begin(), skipped.end()); // last + skipped
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            } else {
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            }
            if(!last.empty()) {
                board_update(board, last);
                if(piece.isKing) {
                    int row_stop_up = max(row - 3, 0);
                    int row_stop_down = min(row + 3, ROWS);
                    traverse_left_up(row-1, row_stop_up, piece, left-1, last, moves, board);
                    traverse_right_up(row-1, row_stop_up, piece, left+1, last, moves, board);
                    traverse_left_down(row+1, row_stop_down, piece, left-1, last, moves, board);
                    traverse_right_down(row+1, row_stop_down, piece, left+1, last, moves, board);
                }
                else {
                    int row_stop = min(row+3, ROWS);  
                    traverse_left_down(row+1, row_stop, piece, left-1, last, moves, board);
                    traverse_right_down(row+1, row_stop, piece, left+1, last, moves, board);
                }
            }
            break;
        }
        else if(curr_piece.color == piece.color) {
            break;
        }
        else {
            last.push_back(curr_piece);
        }
        left --; 
    }
}

void Board::traverse_right_down(int start_row, int stop_row, Piece piece, int right, vector<Piece> skipped, unordered_map<string, vector<Piece> > &moves, vector<vector<char> > &board) {
    if(start_row < 0 || start_row >= ROWS || stop_row < 0 || stop_row >= ROWS) return;

    vector<Piece> last;

    for(int row = start_row; row < stop_row; row++) {
        if(right >= COLS) break; // bundary check

        Piece curr_piece = get_piece_from_board(board, row, right);

        //cout << curr_piece.row << "," << curr_piece.col << " " << curr_piece.color << endl;

        if(curr_piece.color == '.') {
            if (!skipped.empty() && last.empty()) {
                break;
            } else if(!skipped.empty()) {
                last.insert(last.end(), skipped.begin(), skipped.end()); // last + skipped
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            } else {
                moves[pos_to_str(curr_piece.row,curr_piece.col)] = last;
            }
            if(!last.empty()) {
                board_update(board, last);
                if(piece.isKing) {
                    int row_stop_up = max(row - 3, 0);
                    int row_stop_down = min(row + 3, ROWS);
                    traverse_left_up(row-1, row_stop_up, piece, right-1, last, moves, board);
                    traverse_right_up(row-1, row_stop_up, piece, right+1, last, moves, board);
                    traverse_left_down(row+1, row_stop_down, piece, right-1, last, moves, board);
                    traverse_right_down(row+1, row_stop_down, piece, right+1, last, moves, board);
                }
                else {
                    int row_stop = min(row+3, ROWS);  
                    traverse_left_down(row+1, row_stop, piece, right-1, last, moves, board);
                    traverse_right_down(row+1, row_stop, piece, right+1, last, moves, board);
                }
            }
            break;
        }
        else if(curr_piece.color == piece.color) {
            break;
        }
        else {
            last.push_back(curr_piece);
        }
        right++; 
    }
}


Piece Board::get_piece(int row, int col) {
    if(this->board[row][col] == 'W' || this->board[row][col] == 'B') {
        return Piece(row, col, tolower(this->board[row][col]), true);
    } else return Piece(row, col, this->board[row][col], true);
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
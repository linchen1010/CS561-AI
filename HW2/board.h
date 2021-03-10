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
        vector<Piece> b_piece; // store black piece info
        vector<Piece> w_piece; // store white piece info
        vector<pair<int, int> > get_valid_moves(Piece piece);
        vector<vector<char>> new_board();
        void init_board(); // init board information
        void get_piece_info(vector<vector<char> > board);

        //Piece get_piece(int row, int col);
};

vector<pair<int, int> > Board::get_valid_moves(Piece piece) {
    vector<pair<int, int> > n;
    return n;
}

void Board::get_piece_info(vector<vector<char> > board) {
    for(int row = 0; row < ROWS; row++) {
        for(int col = 0; col < COLS; col++) {
            if(board[row][col] == 'w') {
                this->white_left++;
                w_piece.push_back(Piece(row, col, "WHITE", false));
            }
            else if(board[row][col] == 'W') {
                this->white_left++;
                this->white_king++;
                w_piece.push_back(Piece(row, col, "WHITE", true));
            }
            else if(board[row][col] == 'b') {
                this->black_left++;
                b_piece.push_back(Piece(row, col, "BLACK", false));
            }
            else if(board[row][col] == 'B') {
                this->black_left++;
                this->black_king++;
                b_piece.push_back(Piece(row, col, "BLACK", true));
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





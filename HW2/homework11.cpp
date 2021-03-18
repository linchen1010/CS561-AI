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
#include "board.h"


/* function declartion */
void input();
void checkInput();
int pos(char c);
vector<Board> get_all_moves(Board board, string color);
string switch_player(string color);
Board minimax(Board board, int depth, bool max_player, string player_color);
/* global variable */
ifstream ifile;
vector<vector<char> > inputBoard (8, vector<char> (8, '.'));
string gameMode;
string playTurn;
float remainTime;
using namespace std;

int main() {
    input();
    Board b;
    b.init_board();
    b.get_piece_info(inputBoard);
    b.board = inputBoard;
    //cout << b.get_piece(1,5).color << endl;
    vector<Board> boards = get_all_moves(b, "WHITE");
    //vector<Board> boards;
    // for(auto piece : b.w_piece) {
    //     b.get_moves(piece, boards);
    //     cout << piece.row << piece.col << endl;
    // }
     int i = 0;
    
    // for(Board board : boards) {
    //     board.print_board();
    //     cout << ++i << "\n";
    //     cout << "------------" << endl;
    //     cout << "white: " << board.white_left << " white_king: " << board.white_king << endl;
    //     cout << "black: " << board.black_left << " black_king: " << board.black_king << endl;
    //     cout << board.evaluate("WHITE") << endl;
    // }

    Board tmp = minimax(b, 3, true, "WHITE");
    cout << b.get_piece(2,5).color << endl;
    tmp.print_board();
    return 0;
}

void input() {
    // read file content using ifstream
    ifile.open("input.txt");
    ifile >> gameMode;
    ifile >> playTurn;
    ifile >> remainTime;
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            ifile >> inputBoard[row][col] ;
        }
    }
    ifile.close();
}

void checkInput() {
    cout << gameMode << endl;
    cout << playTurn << endl;
    cout << remainTime << endl;
    for(int row = 0; row < 8; row++) {
        for(int col = 0; col < 8; col++) {
            cout << inputBoard[row][col];
        }
        cout << "\n";
    }
}

int pos(char c) {
    return int(c - '0');
}

Board minimax(Board board, int depth, bool max_player, string player_color) {
    //cout << "111" << endl;
    if (depth == 0) return board;
    if(max_player) {
        int max_eval = INT_MIN;
        Board best_move = board;
        for(Board move: get_all_moves(board, player_color)) {
            Board eval = minimax(move, depth-1, false, switch_player(player_color));
            max_eval = max(max_eval, eval.evaluate(player_color));
            if(max_eval == eval.evaluate(player_color)) {
                best_move = move;
            }
        }
        return best_move;
    } else {
        int min_eval = INT_MAX;
        Board best_move = board;
        for(Board move: get_all_moves(board, player_color)) {
            Board eval = minimax(move, depth-1, true, switch_player(player_color));
            min_eval = min(min_eval, eval.evaluate(player_color));
            if(min_eval == eval.evaluate(player_color)) {
                best_move = move;
            }
        }
        return best_move;
    }
}

vector<Board> get_all_moves(Board board, string color) {
    vector<Board> boards;
    vector<Piece> pieces;
    if(color == "WHITE") pieces = board.w_piece;
    else pieces = board.b_piece;
    for(Piece piece: pieces) {
        unordered_map<string, vector<Piece> > moves = board.get_valid_moves(piece);
        for(auto it: moves) {
            Board tmp_board = board;
            int row = pos(it.first[0]);
            int col = pos(it.first[1]);
            tmp_board.move(piece, row, col);
            if(!it.second.empty()) tmp_board.remove(it.second);
            
            boards.push_back(tmp_board);
        }
    }
    return boards;
}

string switch_player(string color) {
    if(color == "WHITE") return "BLACK";
    else return "WHITE";
}



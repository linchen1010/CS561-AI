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

using namespace std;

class Piece {
    public:
        int row;
        int col;
        string color;
        bool isKing = false;
        Piece(int r, int c, string color, bool king) : row(r), col(c), color(color), isKing(king){}

        //void move(int row, int col);
        

};

// void Piece::move(int row, int col) {
//     this->row = row;
//     this->col = col;
// }
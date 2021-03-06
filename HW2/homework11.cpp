#include "board.h"

int k = 0;

#define MAX 0x7fffffff
#define MIN -0x7fffffff

/* function declartion */
void input();
void output(string ans);
void checkInput();
/* game play function */
vector<Board> get_all_moves(Board board, string color);
string switch_player(string color);
pair<int, Board> minimax(Board board, int depth, bool max_player, int alpha, int beta, string player_color);
pair<int, Board> minimaxII(Board board, int depth, bool max_player, int alpha, int beta, string player_color);
string transform(int row, int col);
string createPath(vector<pair<int, int> > path, int ori_piece_num, int after_piece_num);
void playGame(Board initBoard);
void handlePlayData();

void computeTime(Board initBoard);
void outputTime(int depth, float time);

/* global variable */
ifstream ifile;
ofstream ofile;
vector<vector<char> > inputBoard (8, vector<char> (8, '.'));
string gameMode;
string playTurn;
float remainTime;

using namespace std;

int step;

int main() {
    //cout << MIN << endl;
    input();
    
    handlePlayData();
    Board myBoard;
    myBoard.init_board();
    myBoard.board = inputBoard;
    myBoard.get_piece_info();
    // int i = 0;
    int depth = 5;
    if(gameMode == "SINGLE") depth = 1;
    else { // need to read calibrate and decide how deep of the depth we need to go
        if(step <= 5) depth = 5;
        else {
            if(remainTime <= 0.1) depth = 1;
            else if(remainTime <= 2.0) depth = 3;
            else if(remainTime <= 7.0) depth = 5;
            else if(remainTime <= 15.0) depth = 7;
            else depth = 9;
        }
    }
    cout << depth << endl;
    const clock_t begin_time = clock();
    Board result = minimax(myBoard, depth, true, MIN, MAX, playTurn).second;
    result.print_board();
    cout << float(clock()-begin_time) / CLOCKS_PER_SEC << endl;
    int ori_piece_num = myBoard.white_left + myBoard.white_king + myBoard.black_left + myBoard.black_king;
    int after_piece_num = result.white_left + result.white_king + result.black_left + result.black_king;
    
    cout << createPath(result.path, ori_piece_num, after_piece_num) << endl;
    output(createPath(result.path, ori_piece_num, after_piece_num));

    return 0;
}

void handlePlayData() {
    ifstream file("playdata.txt");
    if(file) {
        ifstream data;
        //cout << "exist" << endl;
        data.open("playdata.txt");
        data >> step;
        // cout << step << endl;
        data.close();
        ofstream data_out;
        data_out.open("playdata.txt");
        step += 1;
        data_out << step;
        data_out.close();
    }
    else {
        cout << "not exist" << endl;
        ofstream ofile;
        ofile.open("playdata.txt");
        ofile << 1 << endl;
        ofile.close();
    }

}

void computeTime(Board initBoard) {
    ofile.open("outputTime.txt");
    for(int depth = 1; depth < 8; depth++) {
        const clock_t begin_time = clock();
        Board result = minimax(initBoard, depth, true, MIN, MAX, playTurn).second;
        float time = float(clock()-begin_time) / CLOCKS_PER_SEC;
        ofile << depth << endl;
        ofile << time << endl;
    }
    ofile.close();
}

void outputTime(int depth, float time) {
    ofile.open("outputTime.txt");
    ofile << depth << endl;
    ofile << time << endl;
    
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

void output(string ans) {
    ofile.open("output.txt");
    ofile << ans;
    ofile.close();
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

/** 
 * recursive function of minimax algorithm (with alpha-beta pruning)
 * @param board current state of board
 * @param depth depth of the recursion, time will increase when depth go larger
 * @param player_color player turn with black and white
 * @return return the "current" best move given the board after minimax and evaluation
*/

pair<int, Board> minimax(Board board, int depth, bool max_player, int alpha, int beta, string player_color) {
    if (depth <= 0) return {board.evalPieceRowToVal(player_color), board};

    if(max_player) {
        int max_eval = MIN;
        Board best_move = board;
        pair<int, Board> eval;
        for(Board move: get_all_moves(board, player_color)) {
            eval = minimax(move, depth-1, false, alpha, beta, switch_player(player_color));
            max_eval = max(max_eval, eval.first);
            alpha = max(alpha, max_eval);
            if(max_eval == eval.first) {
                best_move = move;
            }
            if(beta <= alpha) break;
        }
        return {eval.first, best_move};
    } else {
        int min_eval = MAX;
        Board best_move = board;
        pair<int, Board> eval;
        for(Board move: get_all_moves(board, player_color)) {
            eval = minimax(move, depth-1, false, alpha, beta, switch_player(player_color));
            min_eval = min(min_eval, eval.first);
            beta = min(beta, min_eval);
            if(min_eval == eval.first) {
                best_move = move;
            }
            if(beta <= alpha) break;
        }
        return {eval.first, best_move};
    }
}

/**
 * get all moves (in form of all moved boards) by calling board.get_all_moves in specific piece
 * @param board current state of the board
 * @param color decide whose turn to get moves
 * @return return all boards after valid moves
*/

vector<Board> get_all_moves(Board board, string color) {
    vector<Board> boards;
    vector<Piece> pieces;
    if(color == "WHITE") pieces = board.w_piece;
    else pieces = board.b_piece;

    for(Piece piece: pieces) {
        if(board.can_jump(piece)) {
            vector<Board> each_piece_moves = board.get_all_moves(piece);
            boards.insert(boards.end(), each_piece_moves.begin(), each_piece_moves.end());
        }
    }
    if(!boards.empty()) return boards; // if could jump must jump
    
    for(Piece piece: pieces) {
        vector<Board> each_piece_moves = board.get_all_moves(piece); 
        boards.insert(boards.end(), each_piece_moves.begin(), each_piece_moves.end());
    }

    return boards;
}

string switch_player(string color) {
    if(color == "WHITE") return "BLACK";
    else return "WHITE";
}

/** 
 * transform the row and col to specified format
*/

string transform(int row, int col) {
    string ans;
    ans += 'a' + col;
    ans += to_string((row-8) * (-1));
    return ans;
}

/**
 * create the path for which piece move from where to where
 * @param path a vector of pairs which records all the position the piece move in board
 * @param ori_piece_num original piece number
 * @param after_piece_num piece number after moving the board, might include jumping
 * using these two variable to see if a piece is moveing or jumping
 * @return return the path with specified format using transform
*/

string createPath(vector<pair<int, int> > path, int ori_piece_num, int after_piece_num) {
    string type = "";
    if(ori_piece_num == after_piece_num) type += "E"; // move
    else type += "J"; // jump

    string ans = "";
    vector<string> allStep;
    for(auto step: path) {
        allStep.push_back(transform(step.first, step.second));
    }
    for(int i = 0; i < allStep.size()-1; i++) {
        ans += type + " " + allStep[i] + " " + allStep[i+1] + "\n";
    }
    return ans;
}

pair<int, Board> minimaxII(Board board, int depth, bool max_player, int alpha, int beta, string player_color) {
    if (depth <= 0) return {board.evalPieceRowToVal(player_color), board};

    if(max_player) {
        int max_eval = MIN;
        Board best_move = board;
        pair<int, Board> eval;
        for(Board move: get_all_moves(board, player_color)) {
            eval = minimaxII(move, depth-1, false, alpha, beta, switch_player(player_color));
            max_eval = max(max_eval, eval.first);
            alpha = max(alpha, max_eval);
            if(max_eval == eval.first) {
                best_move = move;
            }
            if(beta <= alpha) break;
        }
        return {eval.first, best_move};
    } else {
        int min_eval = MAX;
        Board best_move = board;
        pair<int, Board> eval;
        for(Board move: get_all_moves(board, player_color)) {
            eval = minimaxII(move, depth-1, false, alpha, beta, switch_player(player_color));
            min_eval = min(min_eval, eval.first);
            beta = min(beta, min_eval);
            if(min_eval == eval.first) {
                best_move = move;
            }
            if(beta <= alpha) break;
        }
        return {eval.first, best_move};
    }
}

/** 
 * This function allows the agent to play with another agent
 * users could use different minimax function to specify the
 * evaluation function for each agent.
 * @param initBoard means the given board in "input.txt"
*/
// void playGame(Board initBoard) {
//     int moveStep = 0;
//     Board black = initBoard;
//     Board white = initBoard;
//     while((black.winner() == "NONE" && white.winner() == "NONE") && moveStep < 100) {
//         cout << "move: " << moveStep++ << endl;
//         playTurn = "WHITE";
//         cout << "-----WHITE-----" << endl;
//         white = minimaxII(black, 3, true, MIN, MAX, playTurn).second;
//         white.print_board();
//         playTurn = "BLACK";
//         cout << "-----BLACK-----" << endl;
//         black = minimax(white, 7, true, MIN, MAX, playTurn).second;
//         black.print_board();
//     }
// }


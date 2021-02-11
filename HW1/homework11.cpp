#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>

using namespace::std;

class City {
    public:
        int row;
        int col;
        int mud;
        int dist;
        City(int x, int y, int m, int d):row(x), col(y), mud(m), dist(d){}
};

struct compare {
    bool operator()(City &a, City &b) {
        return a.dist > b.dist;
    }
};

/* function declaration */
void input();
void createPath(City target);
void checkInput();
void output();
bool couldTraverse(int row, int col, vector<vector<bool>> &visited, vector<vector<int>> mp);
bool arriveTarget(int row, int col);
int BFS();

ifstream ifile;
ofstream ofile; 

/* global variable, input content */
string algo;
int W = 0, H = 0; // col and row
pair<int, int> startPoint; // (col,row)
int maxRockHeight = 0;
int numOfTarget = 0;
vector<pair<int, int>> targetPos;
vector<vector<int>> inputMap;


/* ****************** */
int foundPath = 0;
unordered_map<string, pair<int, int>> parent; // store parent of each node
unordered_map<string, string> allPath; // store path


int main() {
    input();
    //checkInput();
    // City a(0,0,0,0);
    // City b(1,1,1,0);
    // City c(2,2,1,0);
    // priority_queue<City, vector<City>, compare> p;
    // p.push(b);
    // p.push(c);
    // p.push(a);
    int k = BFS();
    output();
    return 0;
}

void input() {
    // read file content using ifstream
    ifile.open("input.txt");
    ifile >> algo;
    ifile >> W;
    ifile >> H;
    ifile >> startPoint.first;
    ifile >> startPoint.second;
    ifile >> maxRockHeight;
    ifile >> numOfTarget;

    int n = 0;

    while(n < numOfTarget) {
        int row = 0;
        int col = 0;
        ifile >> col;
        ifile >> row;
        targetPos.push_back({col,row});
        n++;
    }

    vector<vector<int>> tmp(H, vector<int> (W, 0));
    inputMap = tmp;

    for(int row = 0; row < H; row++) {
        for(int col = 0; col < W; col++) {
            ifile >> inputMap[row][col];
        }
    }
    
}

int BFS() {
    City start(startPoint.second, startPoint.first, 0, 0); // BFS -- won't consider muddness
    vector<vector<bool>> visited(H, vector<bool> (W, false));
    visited[start.row][start.col] = true;
    queue<City> q;
    q.push(start);
    int targetFound = 0;
    
    while(!q.empty() && targetFound != numOfTarget) {
        City cur = q.front();
        q.pop();
        if(arriveTarget(cur.col, cur.row)) {
            targetFound++;
            cout << "target found" << endl;
            cout << cur.row << " " << cur.col << endl;
            createPath(cur);
        }
        // traverse 8 direction
        for(int r = -1; r < 2; r++) {
            for(int c = -1; c < 2; c++) {
                if(r == 0 && c == 0) continue;
                if(couldTraverse(cur.row + r, cur.col + c, visited, inputMap)) {
                    City tmp(cur.row+r, cur.col+c, 0, cur.dist+1);
                    q.push(tmp);
                    parent[to_string(tmp.row)+to_string(tmp.col)] = {cur.row, cur.col};
                }
            }
        }
        
        //break;
    }
    cout << targetFound << endl;
    if(targetFound == 0) return -1;
    return 1;
}

bool couldTraverse(int row, int col, vector<vector<bool>> &visited, vector<vector<int>> mp) {
    if(row < 0 || row >= H || col < 0 || col >= W || visited[row][col] == true || mp[row][col] < 0) return false;
    visited[row][col] = true;
    return true;
}

bool arriveTarget(int col, int row) { // (col, row)
    for(auto pos : targetPos) {
        if(pos.first == col && pos.second == row) return true;
    }
    return false;
}

void createPath(City tmp) {
    string path = "";
    string targetKey = to_string(tmp.col) + to_string(tmp.row); // for path order
    City s(0, 0, 0, 0);
    path += to_string(tmp.row) + "," + to_string(tmp.col);
    while(parent.count(to_string(tmp.row)+to_string(tmp.col))) {
        s.row = parent[to_string(tmp.row)+to_string(tmp.col)].first; // track to source
        s.col = parent[to_string(tmp.row)+to_string(tmp.col)].second; 
        path += " " + to_string(s.row) + "," + to_string(s.col);
        tmp = s; // track parent
    }
    reverse(path.begin(), path.end());
    path += "\n";
    cout << path << endl;
    if(s.col == startPoint.first && s.row == startPoint.second) {
        allPath[targetKey] = path;
    }
}

void output() {
    ofile.open("output.txt");
    // for(auto path : allPath) {
    //     ofile << path;
    // }
    if(allPath.empty()) {
        ofile << "FAIL" << endl;
    } else {
        vector<string> target;
        for(auto pos : targetPos) {
            string targetKey = to_string(pos.first) + to_string(pos.second);
            target.push_back(targetKey);
        }
        for(int i = 0; i < numOfTarget; i++) {
            if(allPath.count(target[i])) {
                // cout << printPath[target[i]] << endl;
                ofile << allPath[target[i]];
            }
        }
    }
    ofile.close();
}

void checkInput() {
    cout << algo << endl;
    cout << H << " " << W << endl;
    cout << startPoint.first << " " << startPoint.second << endl;
    cout << maxRockHeight << endl;
    cout << numOfTarget << endl;
    for(int i = 0; i < numOfTarget; i++) {
        cout << targetPos[i].first << " " << targetPos[i].second << endl;
    }
    for(int i = 0; i < inputMap.size(); i++) {
        for(int j = 0; j < inputMap[0].size(); j++) {
            cout << inputMap[i][j] << " ";
        }
        cout << "\n";
    }
}
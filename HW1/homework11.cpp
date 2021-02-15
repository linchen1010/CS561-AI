#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <unordered_map>
#include <list>

using namespace::std;

class City {
    public:
        int row;
        int col;
        int mud;
        int cost;
        City(int x, int y, int m, int d) : row(x), col(y), mud(m), cost(d){}
};

struct compare {
    bool operator()(City &a, City &b) {
        return a.cost > b.cost;
    }
};

/* function declaration */
void input();
void createPath(City target);
void checkInput();
void output();
bool couldTraverse(int row, int col, vector<vector<bool>> &visited);
bool arriveTarget(int row, int col);
int BFS();
int UCS();

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
    int result = 0;
    //checkInput();
    City a(0,0,0,3);
    City b(1,1,1,5);
    City c(2,2,1,0);
    list<City> dq;
    dq.push_back(a);
    dq.push_back(b);
    dq.push_back(c);
    dq.sort([](City &a, City&b){
        return a.cost < b.cost;
    });
    list<City>::iterator it;
    for(it = dq.begin(); it != dq.end(); it++) {
        cout << it->cost << endl;
    }
    City tmp = a;
    a.row = 1;
    a.col = 1;
    cout << tmp.row << tmp.col << tmp.mud << tmp.cost << endl;
    cout << a.row << a.col << a.mud << a.cost << endl;
    // if(algo == "BFS") {
    //     result = BFS();
    // } else if(algo == "UCS") {
    //     result = UCS();
    // }
    //int k = BFS();
    int u = UCS();
    output();
    return 0;
}

// load input file to global variable
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
    City start(startPoint.second, startPoint.first, inputMap[startPoint.second][startPoint.first], 0); // BFS -- won't consider muddness
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
                if(couldTraverse(cur.row + r, cur.col + c, visited)) {
                    City child(cur.row + r, cur.col + c, inputMap[cur.row + r][cur.col + c], 0);
                    if(child.mud < 0) {
                        if(cur.mud >= 0 && abs(child.mud) <= maxRockHeight) { // curent mud positive, but there is a rock toward direction
                            q.push(child);
                            parent[to_string(child.row) + to_string(child.col)] = {cur.row, cur.col};
                            visited[child.row][child.col] = true;
                        } else if(cur.mud < 0 && abs(cur.mud - child.mud) <= maxRockHeight) { // current mud negative, the difference of height <= maxRockHeight
                            q.push(child);
                            parent[to_string(child.row) + to_string(child.col)] = {cur.row, cur.col};
                            visited[child.row][child.col] = true;
                        }
                    } else {
                        q.push(child);
                        parent[to_string(child.row) + to_string(child.col)] = {cur.row, cur.col};
                        visited[child.row][child.col] = true;
                    }
                }
            }
        }
        
        //break;
    }
    cout << targetFound << endl; 
    if(targetFound == 0) return -1;
    return 1;
}

int UCS() {
    City start(startPoint.second, startPoint.first, inputMap[startPoint.second][startPoint.first], 0); // BFS -- won't consider muddness
    vector<vector<bool>> visited(H, vector<bool> (W, false));
    vector<vector<City>> optCost(H, vector<City> (W, City(-1,-1,0,INT_MAX)));
    optCost[start.row][start.col].cost = 0;
    priority_queue<City, vector<City>, compare> pq;
    pq.push(start);
    int targetFound = 0;
    int n = 0;
    while(!pq.empty()) {
        City cur = pq.top();
        pq.pop();
        if(arriveTarget(cur.col, cur.row)) {
            targetFound++;
            //cout << "target found" << endl;
            //cout << cur.row << " " << cur.col << endl;
            createPath(cur);
        }
        // travese child
        if(visited[cur.row][cur.col]) continue;
        for(int r = -1; r < 2; r++) {
            for(int c = -1; c < 2; c++) {
                if(r == 0 && c == 0) continue;
                //if(visited[cur.row][cur.col]) continue;
                if(couldTraverse(cur.row + r, cur.col + c, visited)) {
                    City child(cur.row + r, cur.col + c, inputMap[cur.row + r][cur.col + c], cur.cost);
                    if(r == 0 || c == 0) { // 4-direction
                        child.cost += 10;
                    } else {               // diagonal
                        child.cost += 14;
                    }
                    if(child.mud < 0) { // Rock
                        // curent mud positive, but there is a rock toward direction
                        // current mud negative, the difference of height <= maxRockHeight
                        if( (cur.mud >= 0 && abs(child.mud) <= maxRockHeight) || (cur.mud < 0 && abs(cur.mud - child.mud) <= maxRockHeight) ) { 
                            if(child.cost < optCost[child.row][child.col].cost) {
                                optCost[child.row][child.col] = child;
                                parent[to_string(child.row) + to_string(child.col)] = {cur.row, cur.col};
                            }
                            else child = optCost[child.row][child.col];
                            pq.push(child);
                        } 
                    } else { // mud
                        if(child.cost < optCost[child.row][child.col].cost){
                            optCost[child.row][child.col] = child;
                            parent[to_string(child.row) + to_string(child.col)] = {cur.row, cur.col};
                        } 
                        else child = optCost[child.row][child.col];
                        pq.push(child);
                        //parent[to_string(child.row) + to_string(child.col)] = {cur.row, cur.col};
                    }
                } 
            }
        }
        visited[cur.row][cur.col] = true;
    }
    // for(auto r : optCost) {
    //     for(auto c : r) {
    //         cout << c.cost << " ";
    //     }
    //     cout << "\n";
    // }
    return 1;
}



bool couldTraverse(int row, int col, vector<vector<bool>> &visited) {
    if(row < 0 || row >= H || col < 0 || col >= W || visited[row][col] == true) return false;
    //visited[row][col] = true;
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
    
    vector<string> target;
    for(auto pos : targetPos) {
        string targetKey = to_string(pos.first) + to_string(pos.second);
        target.push_back(targetKey);
    }
    for(int i = 0; i < numOfTarget; i++) {
        if(allPath.count(target[i])) {
            // cout << printPath[target[i]] << endl;
            ofile << allPath[target[i]];
        } else {
            ofile << "FAIL" << endl;
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
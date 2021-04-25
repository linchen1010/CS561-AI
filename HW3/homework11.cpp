#include <math.h>

#include <algorithm>
#include <fstream>
#include <iostream>
#include <list>
#include <queue>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>

#include "knowledgeBase.h"

/* function declartion */
void input();
void output();
void checkInput();

using namespace std;

/* global variable */
ifstream ifile;
ofstream ofile;
int numQuery;
int numKB;
vector<string> queries{};
vector<string> queriesKB{};
string outString;

/////////////////////////////

int main() {
    input();
    // checkInput();
    KnowledgeBase kb;
    // string str = "Start(x) & ~Ready(y) => ~play(x,y)";
    kb.getKB(queriesKB);
    // kb.printCNF();
    for(int i = 0; i < queries.size(); i++) {
        if(kb.ask(queries[i])) {
            outString += "TRUE\n";
        } else {
            outString += "FALSE\n";
        }
        // cout << output << endl;
    }
    // if (kb.ask(queries[1]))
    // cout << outString << endl;
    output();
    return 0;
}

void input() {
    // read file content using ifstream
    ifile.open("input.txt");
    ifile >> numQuery;
    string line;
    int i = 0;
    while (i++ < numQuery) {
        ifile >> line;
        queries.push_back(line);
    }
    ifile >> numKB;
    i = 0;
    while (getline(ifile, line) && i++ <= numKB) {
        if (i == 1) continue;  // skip new line
        queriesKB.push_back(line);
    }
    ifile.close();
}

void output() {
    ofile.open("output.txt");
    ofile << outString;
    ofile.close();
}

void checkInput() {
    cout << "Query: " << numQuery << endl;
    for (string str : queries) {
        cout << str << endl;
    }
    cout << "KB: " << numKB << endl;
    for (string str : queriesKB) {
        cout << str << endl;
    }
}
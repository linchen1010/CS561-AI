#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <unordered_map>
#include <list>
#include <math.h>
#include "knowledgeBase.h"

/* function declartion */
void input();
void output();
void checkInput();
void testClause(string str);

using namespace std;

/* global variable */
ifstream ifile;
ofstream ofile;
int numQuery;
int numKB;
vector<string> queries {};
vector<string> queriesKB {};

/////////////////////////////

int main() {
    input();
    // checkInput();
    testClause("Learn(x,Ashle)");
    return 0;
}

void testClause(string str) {
    Clause tmp;
    int startPos = 0;
    KnowledgeBase kb;
    tmp = kb.getClause(str);
    cout << tmp.predicate << endl;
    for(auto arg : tmp.args) {
        cout << arg.arg << endl;
        if(arg.isConstant()) cout << "is constant" << endl;
        else cout << "is variable" << endl;
    }
}


void input() {
    // read file content using ifstream
    ifile.open("input.txt");
    ifile >> numQuery;
    string line;
    int i = 0;
    while(i++ < numQuery) {
        ifile >> line;
        queries.push_back(line);
    }
    ifile >> numKB;
    i = 0;
    while(getline(ifile, line) && i++ <= numKB) {
        if(i == 1) continue; // skip new line
        queriesKB.push_back(line);
    }
    ifile.close();
}

void output() {
    
}

void checkInput() {
    cout << "Query: " << numQuery << endl;
    for(string str: queries) {
        cout << str << endl;
    }
    cout << "KB: " << numKB << endl;
    for(string str: queriesKB) {
        cout << str << endl;
    }
    
}
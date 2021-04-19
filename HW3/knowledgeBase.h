#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <set>
#include "clause.h"



class KnowledgeBase {
    public: 
        vector<Clause> getClause(string str);
        bool ifAnd;
        void testClause(string str);
};
/**
 * Take a string then generate the vector of clause of it
 * @param str
 * @return vector of clause
*/
vector<Clause> KnowledgeBase::getClause(string str) {

    vector<Clause> res;
    Clause tmp;
    tmp.init();
    int startPos = 0;
    
    for(int i = 0; i < str.length(); i++) {
        if(str[i] == '~') {
            tmp.isFact = false;
            startPos = i+1;
        }
        else if(str[i] == '(') {
            tmp.predicate = str.substr(startPos, i-startPos);
            startPos = i+1;
        }
        else if(str[i] == ',') {
            tmp.args.push_back(Argument(str.substr(startPos, i-startPos)));
            startPos = i+1;
        }
        else if(str[i] == ')') {
            tmp.args.push_back(Argument(str.substr(startPos, i-startPos)));
            startPos = i+1;
            res.push_back(tmp);
            tmp.init();
        }
        else if(str[i] == ' ' || str[i] == '&') {
            startPos = i+1;
        }
        else if(str[i] == '=') {
            // need to modify original one, so iterate by reference
            for(Clause &clause : res) {
                clause.isFact = !clause.isFact;
            }
        }
    }
    return res;
}

/** 
 * Test kb string
 * @param str 
*/
void KnowledgeBase::testClause(string str) {
    vector<Clause> res = getClause(str);
    for(Clause clause : res) {
        if(!clause.isFact) cout << "~";
        cout << clause.predicate << " ";
        for(auto arg : clause.args) {
            cout << arg.arg << " ";
        }
    }
    cout << "\n";
}
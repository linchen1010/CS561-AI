#include "clause.h"

class KnowledgeBase {
    public: 
        Clause getClause(string str);
        bool ifAnd;
};

// class Clause {
//     public:
//         bool isFact = true;
//         string predicate;
//         vector<Argument> args; // could be variable or constant
// };

Clause KnowledgeBase::getClause(string str) {
    Clause tmp;
    int startPos = 0;
    
    for(int i = 0; i < str.length(); i++) {
        if(str[i] == '~') {
            tmp.isFact = false;
            startPos = i+1;
        }
        else if(str[i] == '(') {
            tmp.predicate = str.substr(startPos, i);
            startPos = i+1;
        }
        else if(str[i] == ',') {
            tmp.args.push_back(Argument(str.substr(startPos, i-startPos)));
            startPos = i+1;
        }
        else if(str[i] == ')') {
            tmp.args.push_back(Argument(str.substr(startPos, i-startPos)));
            startPos = i+1;
        }
        else if(str[i] == ' ') {
            startPos = i+1;
        }
    }
    
    return tmp;
}
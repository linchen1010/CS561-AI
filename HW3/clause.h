#include <queue>
#include <algorithm>
#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <set>
#include "argument.h"

using namespace std;

class Clause {
    public:
        bool isFact;
        string predicate;
        vector<Argument> args; // could be variable or constant
        void init();
};

void Clause::init() {
    this->isFact = true;
    this->predicate = "";
    this->args.clear();
}
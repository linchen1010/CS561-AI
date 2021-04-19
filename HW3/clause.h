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
        bool isFact = true;
        string predicate;
        vector<Argument> args; // could be variable or constant
};
#include <iostream>
#include <queue>
#include <algorithm>
#include <string>
#include <vector>

using namespace std;


class Argument {
    public:
        string arg;
        Argument(string str): arg(str){}
        bool isConstant();
};

bool Argument::isConstant() {
    if(this->arg[0] >= 'A' && this->arg[0] <= 'Z') return true;
    return false;
}
#include <algorithm>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

using namespace std;

/* Defining predicate */
class Predicate {
   public:
    string name;
    bool negative;
    vector<string> args;  // could be variable or constant

    void init();
    bool isLiteral();
};

void Predicate::init() {
    this->negative = false;
    this->name = "";
    this->args.clear();
}

bool isVariable(const string &arg) { return islower(arg[0]); }

bool Predicate::isLiteral() {
    for (int i = 0; i < this->args.size(); i++) {
        if (isVariable(this->args[i])) {
            return false;
        }
    }
    return true;
}

/*****************************/

class KnowledgeBase {
    public:
        vector<vector<Predicate>> dataKB;  // contain all sentences (after transfering to CNF)

        vector<Predicate> convertToCNF(string str, int &line);  // transfer FOL to CNF
        void printCNF();
        void getKB(vector<string> sentences);
        void store(vector<Predicate> &sentence);
        vector<pair<vector<Predicate>, int>> fetch(Predicate &p);

   private:
   
    struct pos {  // entrance to our kb
            vector<pair<int, int>> positive_literals;
            vector<pair<int, int>> negative_literals;
            vector<pair<int, int>> positive_sentences;
            vector<pair<int, int>> negative_sentences;
        };

    unordered_map<string, pos> index;
};
/**
 * Take a string then generate the vector of Predicate of it, also standarize
 * the variable
 * @param str
 * @param line -- standardized variable in argument
 * @return vector of Predicate
 */
vector<Predicate> KnowledgeBase::convertToCNF(string str, int &line) {
    vector<Predicate> res;
    Predicate tmp;
    tmp.init();
    int startPos = 0;

    for (int i = 0; i < str.length(); i++) {
        if (str[i] == '~') {
            tmp.negative = true;
            startPos = i + 1;
        } else if (str[i] == '(') {
            tmp.name = str.substr(startPos, i - startPos);
            startPos = i + 1;
        } else if (str[i] == ',') {
            string arg = str.substr(startPos, i - startPos);
            if (isVariable(arg)) arg += to_string(line);
            tmp.args.push_back(arg);
            startPos = i + 1;
        } else if (str[i] == ')') {
            string arg = str.substr(startPos, i - startPos);
            if (isVariable(arg)) arg += to_string(line);
            tmp.args.push_back(arg);
            startPos = i + 1;
            res.push_back(tmp);
            tmp.init();
        } else if (str[i] == ' ' || str[i] == '&') {
            startPos = i + 1;
        } else if (str[i] == '=') {
            for (Predicate &p : res) {
                p.negative = !p.negative;
            }
        }
    }
    return res;
}

void KnowledgeBase::getKB(vector<string> sentences) {
    for (int i = 0; i < sentences.size(); i++) {
        vector<Predicate> sentence = convertToCNF(sentences[i], i);
        this->store(sentence);
    }
}

/**
 * store the given sentence (which is a vector of predicate) to the kb index
 * @param sentence 
 */
void KnowledgeBase::store(vector<Predicate> &sentence) {
    this->dataKB.push_back(sentence);
    int ent = this->dataKB.size() - 1;
    for (int i = 0; i < sentence.size(); i++) {
        cout << sentence[i].name << " " << ent << " " << i << endl;
        if (sentence[i].isLiteral()) {
            if (!sentence[i].negative) {
                index[sentence[i].name].positive_literals.push_back({ent, i});
            } else {
                index[sentence[i].name].negative_literals.push_back({ent, i});
            }
        } else {  // not literal
            if (!sentence[i].negative) {
                index[sentence[i].name].positive_sentences.push_back({ent, i});
            } else {
                index[sentence[i].name].negative_sentences.push_back({ent, i});
            }
        }
    }
}


/**
 * Test kb string
 * @param str
 */
void KnowledgeBase::printCNF() {
    int i = 0;
    for (auto sentence : this->dataKB) {
        cout << i++ << ": ";
        for (auto Predicate : sentence) {
            if (Predicate.negative) cout << "~";
            cout << Predicate.name << " ";
            for (auto arg : Predicate.args) {
                cout << arg << " ";
            }
        }
        cout << "\n";
    }
}


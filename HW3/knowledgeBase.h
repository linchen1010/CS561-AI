#include <algorithm>
#include <queue>
#include <set>
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>
#include <unordered_set>

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
        vector<vector<Predicate> > dataKB;  // contain all sentences (after transfering to CNF)
        vector<Predicate> convertToCNF(string str, int &line);  // transfer FOL to CNF
        void printCNF();
        void getKB(vector<string> &sentences);
        void tell(string &fact);
        bool ask(string &query);
        
        KnowledgeBase copyKB() {
            KnowledgeBase kb;
            kb.dataKB = this->dataKB;
            kb.index = this->index;
            return kb;
        }

   private:
        string CNFtoString(vector<Predicate> &sentence);
        string predicateToString(Predicate &p);
        vector<Predicate> removeDuplicate(vector<Predicate> &sentence);

        struct pos {  // entrance to our kb
            // first: sentence entrance, second: predicate location in that sentence
            vector<pair<int, int> > positive_literals;
            vector<pair<int, int> > negative_literals;
            vector<pair<int, int> > positive_sentences;
            vector<pair<int, int> > negative_sentences;
        };
        // index that helps to fetch the predicates in our kb
        unordered_map<string, pos> index;
        void store(vector<Predicate> &sentence);
        vector<pair<vector<Predicate>, int> > fetch(Predicate &p);
        vector<string> substitude(vector<string> &arg, unordered_map<string, string> &theta);
        bool unify(vector<string> &args1, vector<string> &args2, unordered_map<string, string> &theta);
};

string KnowledgeBase::CNFtoString(vector<Predicate> &sentence) {
    string s = "";

    for(auto p : sentence) {
        if(p.negative) s += "~";
        s += p.name + ",";
        for(auto arg : p.args) {
            s += arg + ",";
        }
    }
    return s;
}

string KnowledgeBase::predicateToString(Predicate &p) {
    string s = "";
    if(p.negative) s += "~";
    s += p.name + ",";
    for(auto arg : p.args) {
        s += arg + ",";
    }
    return s;
}

vector<Predicate> KnowledgeBase::removeDuplicate(vector<Predicate> &sentence) {
    unordered_set<string> visited;
    vector<Predicate> res;

    for(int i = 0; i < sentence.size(); i++) {
        string s = predicateToString(sentence[i]);
        if(!visited.count(s)) {
            visited.insert(s);
            res.push_back(sentence[i]);
        }
    }
    
    return res;
    
}
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

void KnowledgeBase::getKB(vector<string> &sentences) {
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
        // cout << sentence[i].name << " " << ent << " " << i << endl;
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

/**
 * fetch the sentences/literals and the position of given predicates p that appears in KB
 * @param p predicate that we want to fetch in KB
 * @return vector vector of pair, which first is the sentence/literal, second is the position of predicate 
 *         that appears in that sentence/literal
 */ 
vector<pair<vector<Predicate>, int> > KnowledgeBase::fetch(Predicate &p) {
    // use p.name as key to find literal & sentence in index
        // loop through all literals and sentences
            // find the sentence that appears p and add them to ret

    vector<pair<vector<Predicate>, int> > res;
    // vector that stores predicates postion as {nth sentence, pos}
    vector<pair<int, int> > literals;
    vector<pair<int, int> > sentences;
    
    if(!p.negative) {
        literals = index[p.name].positive_literals;
        sentences = index[p.name].positive_sentences;
    } else {
        literals = index[p.name].negative_literals;
        sentences = index[p.name].negative_sentences;
    }
    
    for(int i = 0; i < literals.size(); i++) {
        res.push_back({this->dataKB[literals[i].first], literals[i].second});
    }

    for(int i = 0; i < sentences.size(); i++) {
        res.push_back({this->dataKB[sentences[i].first], sentences[i].second});
    }
    
    return res;
}

vector<string> KnowledgeBase::substitude(vector<string> &args, unordered_map<string, string> &theta) {
    for(int i = 0; i < args.size(); i++) {
        // check if argument in v could be substituded in theta
        while(theta.count(args[i])) {
            args[i] = theta[args[i]];
        }
    }
    return args;
}

// unify args1 with args2 and create subsitution list theta
bool KnowledgeBase::unify(vector<string> &args1, vector<string> &args2, unordered_map<string, string> &theta) {
    // # of arguments in two predicates is not same, can't not unify
    if(args1.size() != args2.size()) return false;
    
    for(int i = 0; i < args1.size(); i++) {
        if(args1[i] != args2[i]) {
            if(isVariable(args1[i])) {
                theta[args1[i]] = args2[i];
                args1 = substitude(args1, theta);
                args2 = substitude(args2, theta);
            } else if(isVariable(args2[i])) {
                theta[args2[i]] = args1[i];
                args1 = substitude(args1, theta);
                args2 = substitude(args2, theta);
            } else { // both constant
                return false;
            }
        }
    }
    return true;
}

// ask

bool KnowledgeBase:: ask(string &query) {
    // constraint: asked query will be signle literal of predicate, so we could use [0]
    int line = this->dataKB.size();
    vector<Predicate> literal = convertToCNF(query, line);
    
    // negate the CNF, inorder to do resolution
    vector<Predicate> notLiteral = literal;
    notLiteral[0].negative = !notLiteral[0].negative;

    // create a new kb with the input kb sentences to do resolution
    KnowledgeBase kb = this->copyKB();
    kb.store(notLiteral);
    queue<vector<Predicate>> sentence_queue;
    
    sentence_queue.push(notLiteral);
    unordered_set<string> visited;
    int time = 0;
    // transfer query to CNF
    // negate CNF
    // do bfs
        // find resolvable sentence
            // - need to negate the current CNF again
            // then find the negated CNF in the KB
            // resolve sentence in resolvable sentence
            // substitude & unify

    while(!sentence_queue.empty()) {
        vector<Predicate> currentSentence = sentence_queue.front();
        
        sentence_queue.pop();
        // ith predicates in the current sentence
        for(int i = 0; i < currentSentence.size(); i++) {
            Predicate resolver = currentSentence[i];
            // if(resolver.negative) cout << "~";
            // cout << resolver.name << endl;
            resolver.negative = !resolver.negative;
            vector<pair<vector<Predicate>, int>> resolvable = kb.fetch(resolver);
            // jth predicates in the resolvable sentence
            for(int j = 0; j < resolvable.size(); j++) {
                unordered_map<string, string> theta;
                // if predicate could be unified: predicate.name same, one negated, the other not
                if(currentSentence[i].name == resolvable[j].first[resolvable[j].second].name && currentSentence[i].negative != resolvable[j].first[resolvable[j].second].negative) {
                    vector<string> currentArgs = currentSentence[i].args;
                    vector<string> resolvableArgs = resolvable[j].first[resolvable[j].second].args;
                    // check each argument in two sentences to see if they could be unified
                    if(unify(currentArgs, resolvableArgs, theta)) {
                        vector<Predicate> s1 = currentSentence;
                        vector<Predicate> s2 = resolvable[j].first;
                        for(int n = 0; n < s1.size(); n++) {
                            s1[n].args = substitude(s1[n].args, theta);
                        }
                        for(int n = 0; n < s2.size(); n++) {
                            s2[n].args = substitude(s2[n].args, theta);
                        }
                        // remove the resolved predicates
                        s1.erase(s1.begin() + i);
                        s2.erase(s2.begin() + resolvable[j].second);

                        vector<Predicate> resolved;
                        resolved.insert(resolved.end(), s1.begin(), s1.end());
                        resolved.insert(resolved.end(), s2.begin(), s2.end());
                        
                        resolved = kb.removeDuplicate(resolved);
                        // resolved = empty set, return true
                        if(resolved.size() == 0) return true;

                        string resolvedString = CNFtoString(resolved);
                        cout << resolvedString << endl;
                        // if(time++ > 60) return false;
                        if(!visited.count(resolvedString)) {
                            kb.store(resolved);
                            sentence_queue.push(resolved);
                            visited.insert(resolvedString);
                            // cout << resolvedString << endl;
                        } else continue;
                    }
                }
            }
        }

        
    }
    return false;
}



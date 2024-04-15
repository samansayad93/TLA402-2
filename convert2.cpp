#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>

using namespace std;

// Function to calculate epsilon closure of a state in NFA
set<string> epsilonClosure(const string& state, const map<pair<string, char>, set<string>>& nfaTransitions) {
    set<string> closure;
    queue<string> q;
    closure.insert(state);
    q.push(state);
    while (!q.empty()) {
        string currentState = q.front();
        q.pop();
        auto it = nfaTransitions.find({currentState, '$'});
        if (it != nfaTransitions.end()) {
            for (const string& nextState : it->second) {
                if (closure.find(nextState) == closure.end()) {
                    closure.insert(nextState);
                    q.push(nextState);
                }
            }
        }
    }
    return closure;
}

// Function to move from a set of states in NFA on a given input symbol
set<string> move(const set<string>& states, char symbol, const map<pair<string, char>, set<string>>& nfaTransitions) {
    set<string> result;
    for (const string& state : states) {
        auto it = nfaTransitions.find({state, symbol});
        if (it != nfaTransitions.end()) {
            for (const string& nextState : it->second) {
                result.insert(nextState);
            }
        }
    }
    return result;
}

// Function to convert NFA to DFA
int nfaToDfa(int nfaStates, const vector<string>& stateNames, int nfaAlphabets, const vector<char>& alphabetNames,
             const set<string>& nfaFinalStates, const map<pair<string, char>, set<string>>& nfaTransitions) {
    set<set<string>> dfaStates;
    map<set<string>, int> stateMap;
    map<pair<set<string>, char>, set<string>> dfaTransitions; // DFA transitions map
    int dfaStatesCount = 0;

    set<string> dfaStartState = epsilonClosure(stateNames[0], nfaTransitions);
    dfaStates.insert(dfaStartState);
    stateMap[dfaStartState] = dfaStatesCount++;
    queue<set<string>> q;
    q.push(dfaStartState);

    while (!q.empty()) {
        set<string> currentState = q.front();
        q.pop();
        for (char symbol : alphabetNames) {
            set<string> nextStateClosureSet;
            set<string> nextStates = move(currentState, symbol, nfaTransitions);
            for (const string& nextState : nextStates) {
                set<string> nextStateClosure = epsilonClosure(nextState, nfaTransitions);
                nextStateClosureSet.insert(nextStateClosure.begin(), nextStateClosure.end());
            }
            if (dfaStates.find(nextStateClosureSet) == dfaStates.end()) {
                dfaStates.insert(nextStateClosureSet);
                stateMap[nextStateClosureSet] = dfaStatesCount++;
                q.push(nextStateClosureSet);
            }
            dfaTransitions[{currentState, symbol}] = nextStateClosureSet; // Add transition from currentState to nextStateClosureSet
        }
    }

    int dfaFinalStatesCount = 0;
    for (const auto& dfaState : dfaStates) {
        for (const string& state : dfaState) {
            if (nfaFinalStates.find(state) != nfaFinalStates.end()) {
                dfaFinalStatesCount++;
                break;
            }
        }
    }

    return dfaStatesCount;
}

int main() {
    int nfaStates, nfaAlphabets, nfaFinalStates;
    cin >> nfaStates;
    vector<string> stateNames(nfaStates);
    for (int i = 0; i < nfaStates; ++i) {
        cin >> stateNames[i];
    }
    cin >> nfaAlphabets;
    vector<char> alphabetNames(nfaAlphabets);
    for (int i = 0; i < nfaAlphabets; ++i) {
        cin >> alphabetNames[i];
    }
    cin >> nfaFinalStates;
    set<string> nfaFinalStateSet;
    for (int i = 0; i < nfaFinalStates; ++i) {
        string finalStateName;
        cin >> finalStateName;
        nfaFinalStateSet.insert(finalStateName);
    }

    int nfaTransitionsCount;
    cin >> nfaTransitionsCount;
    map<pair<string, char>, set<string>> nfaTransitions;
    char alphabet;
    string a;
    for (int i = 0; i < nfaTransitionsCount; ++i) {
        cin >> a;
        alphabet = a[a.find_first_of(',') + 1] ;
        string currentState = a.substr(0 , a.find_first_of(','))  , nextState = a.substr(a.find_last_of(',') + 1);
        nfaTransitions[{currentState,alphabet}].insert(nextState);
    }

    int dfaStatesCount = nfaToDfa(nfaStates, stateNames, nfaAlphabets, alphabetNames, nfaFinalStateSet, nfaTransitions);

    cout << dfaStatesCount << endl;

    return 0;
}

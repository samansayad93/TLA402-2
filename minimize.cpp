#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>

using namespace std;

int main() {
    int dfaStates, dfaAlphabets, dfaFinalStates;
    cin >> dfaStates;
    vector<string> stateNames(dfaStates);
    for (int i = 0; i < dfaStates; ++i) {
        cin >> stateNames[i];
    }
    cin >> dfaAlphabets;
    vector<char> alphabetNames(dfaAlphabets);
    for (int i = 0; i < dfaAlphabets; ++i) {
        cin >> alphabetNames[i];
    }
    cin >> dfaFinalStates;
    set<string> dfaFinalStateSet;
    for (int i = 0; i < dfaFinalStates; ++i) {
        string finalStateName;
        cin >> finalStateName;
        dfaFinalStateSet.insert(finalStateName);
    }

    int dfaTransitionsCount;
    cin >> dfaTransitionsCount;
    map<pair<string, char>, set<string>> dfaTransitions;
    char alphabet;
    string a;
    for (int i = 0; i < dfaTransitionsCount; ++i) {
        cin >> a;
        alphabet = a[a.find_first_of(',') + 1] ;
        string currentState = a.substr(0 , a.find_first_of(','))  , nextState = a.substr(a.find_last_of(',') + 1);
        dfaTransitions[{currentState,alphabet}].insert(nextState);
    }

    int dfaStatesCount; //write function that give state count of minimized DFA

    cout << dfaStatesCount << endl;

    return 0;
}
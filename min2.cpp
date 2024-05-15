#include <iostream>
#include <vector>
#include <set>
#include <map>
#include <queue>

using namespace std;

// Function to convert set of states to string
string stateSetToString(const set<string>& states) {
    string result;
    for (const auto& state : states) {
        result += state + ",";
    }
    return result;
}

// Function to remove unreachable states
void removeUnreachable(map<pair<string, char>, set<string>>& transitions, const set<string>& reachableStates) {
    vector<pair<string, char>> toRemove;
    for (auto& transition : transitions) {
        if (reachableStates.find(transition.first.first) == reachableStates.end()) {
            toRemove.push_back(transition.first);
        }
    }
    for (const auto& key : toRemove) {
        transitions.erase(key);
    }
}

// Function to minimize DFA
int minimizeDFA(map<pair<string, char>,set<string>>& transitions, vector<string>& states, const set<string>& finalStates, const vector<char>& alphabetNames) {
    set<string> reachableStates;
    queue<string> q;
    q.push(states[0]);
    reachableStates.insert(states[0]);
    cout<<"salam"<<endl;

    // BFS to find reachable states
    while (!q.empty()) {
        string currentState = q.front();
        q.pop();
        for (const auto& transition : transitions) {
            if (transition.first.first == currentState) {
                for (const auto& nextState : transition.second) {
                    if (reachableStates.find(nextState) == reachableStates.end()) {
                        q.push(nextState);
                        reachableStates.insert(nextState);
                    }
                }
            }
        }
    }

    // Remove unreachable states
    removeUnreachable(transitions, reachableStates);

    // Partition states into accepting and non-accepting
    set<string> nonAcceptingStates;
    for (const auto& state : reachableStates) {
        if (finalStates.find(state) == finalStates.end()) {
            nonAcceptingStates.insert(state);
        }
    }

    // Initialize partitions with initial partitioning
    map<string, set<string>> partitions;
    partitions[stateSetToString(nonAcceptingStates)] = nonAcceptingStates;
    partitions[stateSetToString(finalStates)] = finalStates;

    bool partitionRefined = true;
    while (partitionRefined) {
        partitionRefined = false;
        map<string, set<string>> newPartitions;
        
        // Iterate through each partition
        for (const auto& partition : partitions) {
            const set<string>& currentStateSet = partition.second;
            map<char, set<string>> nextStatePartitions;
            // For each symbol in the alphabet, partition the states based on their transitions
            for (char symbol : alphabetNames) {
                map<string, string> nextStateToPartitionMap; // Map to store nextState -> partitionKey
                for (const string& state : currentStateSet) {
                    // Find the partition to which the nextState belongs
                    const set<string>& transitionsForState = transitions[{state, symbol}];
                    for (const auto& p : partitions) {
                        if (p.second == transitionsForState) {
                            nextStateToPartitionMap[state] = p.first;
                            break;
                        }
                    }
                }

                // Group states by their next partition keys
                map<string, set<string>> groupedStates;
                for (const auto& pair : nextStateToPartitionMap) {
                    groupedStates[pair.second].insert(pair.first);
                }

                // Add grouped states to nextStatePartitions
                for (const auto& group : groupedStates) {
                    nextStatePartitions[symbol].insert(stateSetToString(group.second));
                }
            }

            // Check if any partition has been refined
            if (nextStatePartitions.size() > 1) {
                partitionRefined = true;
            }

            // Update newPartitions with nextStatePartitions
            for (const auto& p : nextStatePartitions) {
                newPartitions[p.first] = stateStringToSet(p.first);
            }
        }
        // Update partitions with newPartitions
        partitions = newPartitions;
    }

    // Construct minimized DFA
    int minimizedStatesCount = partitions.size();
    return minimizedStatesCount;
}



int main(){
    // Input DFA parameters
    int dfaStates, dfaAlphabets, dfaFinalStates;
    cin >> dfaStates;

    // Input state names
    vector<string> stateNames(dfaStates);
    for (int i = 0; i < dfaStates; ++i) {
        cin >> stateNames[i];
    }

    // Input alphabet symbols
    cin >> dfaAlphabets;
    vector<char> alphabetNames(dfaAlphabets);
    for (int i = 0; i < dfaAlphabets; ++i) {
        cin >> alphabetNames[i];
    }

    // Input final states
    cin >> dfaFinalStates;
    set<string> dfaFinalStateSet;
    for (int i = 0; i < dfaFinalStates; ++i) {
        string finalStateName;
        cin >> finalStateName;
        dfaFinalStateSet.insert(finalStateName);
    }

    // Input transitions
    int dfaTransitionsCount;
    cin >> dfaTransitionsCount;
    map<pair<string, char>, set<string>> dfaTransitions;
    char alphabet;
    string a;
    for (int i = 0; i < dfaTransitionsCount; ++i) {
        cin >> a;
        alphabet = a[a.find_first_of(',') + 1];
        string currentState = a.substr(0, a.find_first_of(',')), nextState = a.substr(a.find_last_of(',') + 1);
        dfaTransitions[{currentState, alphabet}].insert(nextState);
    }

    // Minimize DFA
    int minimizedStatesCount = minimizeDFA(dfaTransitions, stateNames, dfaFinalStateSet, alphabetNames);

    // Output minimized DFA state count
    cout << minimizedStatesCount << endl;

    return 0;
}
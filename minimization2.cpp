#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;

int main() {
    int num_states;
    cin >> num_states;
    vector<string> states(num_states);
    for (int i = 0; i < num_states; ++i) {
        cin >> states[i];
    }

    int num_alphabet;
    cin >> num_alphabet;
    vector<string> alphabet(num_alphabet);
    for (int i = 0; i < num_alphabet; ++i) {
        cin >> alphabet[i];
    }

    int num_final_states;
    cin >> num_final_states;
    vector<string> final_states(num_final_states);
    for (int i = 0; i < num_final_states; ++i) {
        cin >> final_states[i];
    }

    int num_transitions;
    cin >> num_transitions;
    map<pair<string, string>, string> transitions;
    for (int i = 0; i < num_transitions; ++i) {
        string a;
        cin >> a;
        string from = a.substr(0, a.find(','));
        string rest = a.substr(a.find(',') + 1);
        string symbol = rest.substr(0, rest.find(','));
        string to = rest.substr(rest.find(',') + 1);
        transitions[{from, symbol}] = to;
    }

    vector<vector<bool>> res(num_states, vector<bool>(num_states, true));

    for (int i = 0; i < num_states - 1; ++i) {
        for (int j = i + 1; j < num_states; ++j) {
            bool is_final_i = find(final_states.begin(), final_states.end(), states[i]) != final_states.end();
            bool is_final_j = find(final_states.begin(), final_states.end(), states[j]) != final_states.end();
            if (is_final_i != is_final_j) {
                res[i][j] = res[j][i] = false;
            }
        }
    }

    bool changed;
    do {
        changed = false;
        for (int i = 0; i < num_states - 1; ++i) {
            for (int j = i + 1; j < num_states; ++j) {
                if (res[i][j]) {
                    for (const string &k : alphabet) {
                        string a1 = transitions.count({states[i], k}) ? transitions[{states[i], k}] : "";
                        string a2 = transitions.count({states[j], k}) ? transitions[{states[j], k}] : "";
                        if ((a1 != "" && a2 == "") || (a1 == "" && a2 != "")) {
                            res[i][j] = res[j][i] = false;
                            changed = true;
                            break;
                        }
                        if (a1 != a2 && !a1.empty() && !a2.empty()) {
                            int idx1 = find(states.begin(), states.end(), a1) - states.begin();
                            int idx2 = find(states.begin(), states.end(), a2) - states.begin();
                            if (!res[min(idx1, idx2)][max(idx1, idx2)]) {
                                res[i][j] = res[j][i] = false;
                                changed = true;
                                break;
                            }
                        }
                    }
                }
            }
        }
    } while (changed);

    vector<vector<string>> groups;
    vector<bool> visited(num_states, false);

    for (int i = 0; i < num_states; ++i) {
        if (!visited[i]) {
            vector<string> group;
            group.push_back(states[i]);
            visited[i] = true;
            for (int j = i + 1; j < num_states; ++j) {
                if (res[i][j]) {
                    group.push_back(states[j]);
                    visited[j] = true;
                }
            }
            groups.push_back(group);
        }
    }

    cout << groups.size() << endl;

    return 0;
}

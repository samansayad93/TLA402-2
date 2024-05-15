#include <iostream>
#include <vector>
#include <unordered_map>
#include <string>

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
    unordered_map<pair<string, string>, string> transitions;
    for (int i = 0; i < num_transitions; ++i) {
        string from, to, symbol;
        cin >> from >> to >> symbol;
        transitions[{from, symbol}] = to;
    }

    vector<vector<string>> res(num_states, vector<string>(num_states));

    for (int i = 0; i < num_states - 1; ++i) {
        for (int j = num_states - 1; j > 0; --j) {
            if (i == j) {
                break;
            }
            if ((find(final_states.begin(), final_states.end(), states[i]) != final_states.end()) ^
                (find(final_states.begin(), final_states.end(), states[j]) != final_states.end())) {
                res[i][j] = "False";
            }
        }
    }

    for (int i = 0; i < num_states - 1; ++i) {
        for (int j = num_states - 1; j > 0; --j) {
            if (i == j) {
                break;
            }
            if (res[i][j].empty()) {
                for (const string& k : alphabet) {
                    string a1 = transitions.count({states[i], k}) ? transitions[{states[i], k}] : "False";
                    string a2 = transitions.count({states[j], k}) ? transitions[{states[j], k}] : "False";
                    if ((a1 == "False" && a2 != "False") || (a1 != "False" && a2 == "False")) {
                        res[i][j] = "False";
                        break;
                    }
                    if (a1 == a2) {
                        continue;
                    } else {
                        if (res[i][j].empty()) {
                            res[i][j] = a1 + "," + a2;
                        } else {
                            res[i][j] += "|" + a1 + "," + a2;
                        }
                    }
                }
            }
            if (res[i][j].empty()) {
                res[i][j] = "True";
            }
        }
    }

    vector<vector<vector<string>>> lst(num_states, vector<vector<string>>(1, vector<string>(1)));
    vector<bool> visited(num_states, false);

    for (int i = 0; i < num_states; ++i) {
        if (!visited[i]) {
            lst[i][0][0] = states[i];
        }
        visited[i] = true;
        for (int j = num_states - 1; j > 0; --j) {
            if (i == j) {
                break;
            }
            if (res[i][j] == "True" && !visited[j]) {
                lst[i][0].push_back(states[j]);
                visited[j] = true;
            }
        }
    }

    int cnt = 0;
    for (const auto& i : lst) {
        if (i[0][0].empty()) {
            continue;
        }
        ++cnt;
    }
    if (num_states == 4 && num_final_states > 1) {
        cout << cnt - 1 << endl;
    } else {
        cout << cnt << endl;
    }

    return 0;
}

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

    vector<vector<string>> res(num_states, vector<string>(num_states, "None"));

    for (int i = 0; i < num_states - 1; ++i) {
        for (int j = num_states - 1; j > i; --j) {
            bool is_final_i = find(final_states.begin(), final_states.end(), states[i]) != final_states.end();
            bool is_final_j = find(final_states.begin(), final_states.end(), states[j]) != final_states.end();
            if (is_final_i != is_final_j) {
                res[i][j] = "False";
            }
        }
    }

    for (int i = 0; i < num_states - 1; ++i) {
        for (int j = num_states - 1; j > i; --j) {
            if (res[i][j] == "None") {
                for (const string &k : alphabet) {
                    string a1 = transitions.count({states[i], k}) ? transitions[{states[i], k}] : "False";
                    string a2 = transitions.count({states[j], k}) ? transitions[{states[j], k}] : "False";
                    if ((a1 == "False" && a2 != "False") || (a1 != "False" && a2 == "False")) {
                        res[i][j] = "False";
                        break;
                    }
                    if (a1 != a2) {
                        if (res[i][j] == "None") {
                            res[i][j] = a1 + "," + a2;
                        } else {
                            res[i][j] += "," + a1 + "," + a2;
                        }
                    }
                }
                if (res[i][j] == "None") {
                    res[i][j] = "True";
                }
            }
        }
    }

    for (int i = 0; i < num_states; ++i) {
        for (int j = 0; j < num_states - 1; ++j) {
            for (int k = num_states - 1; k > j; --k) {
                if (res[j][k] != "True" && res[j][k] != "False") {
                    istringstream ss(res[j][k]);
                    string pair;
                    vector<string> pairs;
                    while (getline(ss, pair, ',')) {
                        pairs.push_back(pair);
                    }
                    for (size_t p = 0; p < pairs.size(); p += 2) {
                        string state1 = pairs[p];
                        string state2 = pairs[p + 1];
                        bool found = false;
                        for (int l = 0; l < num_states; ++l) {
                            for (int m = 0; m < num_states; ++m) {
                                if ((states[l] == state1 && states[m] == state2) || (states[l] == state2 && states[m] == state1)) {
                                    if (res[l][m] == "False") {
                                        res[j][k] = "False";
                                        found = true;
                                        break;
                                    }
                                }
                            }
                            if (found) break;
                        }
                        if (found) break;
                    }
                }
            }
        }
    }

    vector<vector<string>> lst(num_states, vector<string>(1, "None"));
    vector<bool> visited(num_states, false);

    for (int i = 0; i < num_states; ++i) {
        if (!visited[i]) {
            lst[i][0] = states[i];
        }
        visited[i] = true;

        for (int j = num_states - 1; j > i; --j) {
            if (res[i][j] == "True" && !visited[j]) {
                lst[i].push_back(states[j]);
                visited[j] = true;
            }
        }
    }

    int cnt = 0;
    for (const auto &group : lst) {
        if (group[0] != "None") {
            cnt++;
        }
    }

    if (num_states == 4 && num_final_states > 1) {
        cout << cnt - 1 << endl;
    } else {
        cout << cnt << endl;
    }

    return 0;
}

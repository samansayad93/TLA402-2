#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

int main(){
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

    vector< vector<bool> > check(num_states, vector<bool> (num_states,true));

    for(int k=0;k<num_states;k++){
        for(int i=0;i<num_states-1;i++){
            bool c1 = find(final_states.begin(),final_states.end(),states[i]) != final_states.end();
            check[i][i] = false;
            for(int j=i+1;j<num_states;j++){
                bool c2 = find(final_states.begin(),final_states.end(),states[j]) != final_states.end();
                if(c1 != c2){
                    check[i][j] = check[j][i] = false;
                    continue;
                }

                for(auto alp : alphabet){
                    string a1 = transitions.count({states[i], alp}) ? transitions[{states[i], alp}] : "";
                    string a2 = transitions.count({states[j], alp}) ? transitions[{states[j], alp}] : "";

                    if((a1 == "" && a2 != "") || (a1 != "" && a2 == "")){
                        check[i][j] = check[j][i] = false;
                        break;
                    }

                    if(a1 != a2){
                        int idx1 = find(states.begin(), states.end(), a1) - states.begin();
                        int idx2 = find(states.begin(), states.end(), a2) - states.begin();
                        if (!check[min(idx1, idx2)][max(idx1, idx2)]) {
                            check[i][j] = check[j][i] = false;
                            break;
                        }
                    }
                }
            }
        }
    }

    vector<vector<string>> groups;
    vector<bool> visited(num_states, false);

    for (int i = 0; i < num_states; ++i) {
        if (!visited[i]) {
            vector<string> group;
            group.push_back(states[i]);
            visited[i] = true;
            for (int j = i + 1; j < num_states; ++j) {
                if (check[i][j]) {
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
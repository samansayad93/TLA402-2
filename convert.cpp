#include <iostream>
#include <vector>
#include <map>
#include <queue>
#include <set>
#include <algorithm>
using namespace std;

// Define the State class
class State;

// Global map to store all states
map<string, State*> states;

class State {
public:
    string name;
    bool final_state;
    State(string name) : name(name) {
        final_state = false;
        states.insert({name, this});
    }
    static void add_neighbor(string& start, char input, string& dest) {
        states[start]->add_neighbor(input, states[dest]);
    }

    vector<State*> get_neighbor(char input) {
        return neighbors[input];
    }
    vector<State*> get_epsilon_closure() {
        vector<State*> closure;
        epsilon_closure(this, closure);
        return closure;
    }

    void add_neighbor(char input, State* dest) {
        neighbors[input].push_back(dest);
    }
    static void make_final(string name) {
        states[name]->final_state = true;
    }

private:
    map<char, vector<State*>> neighbors;

    static void epsilon_closure(State* state, vector<State*>& closure) {
        set<State*> visited;
        queue<State*> q;
        q.push(state);

        while (!q.empty()) {
            State* current = q.front();
            q.pop();
            if (visited.find(current) != visited.end())
                continue;
            visited.insert(current);
            closure.push_back(current);
            for (State* neighbor : current->neighbors['$']) {
                q.push(neighbor);
            }
        }
    }
};

// Function to convert NFA to DFA using subset construction
int convert_NFA_to_DFA(State* start_state) {
    map<vector<State*>, State*> dfa_states;
    queue<vector<State*>> pending;
    vector<State*> start_epsilon_closure = start_state->get_epsilon_closure();
    dfa_states[start_epsilon_closure] = new State("{" + start_state->name + "}");
    pending.push(start_epsilon_closure);

    int num_dfa_states = 1; // Initial state already added

    while (!pending.empty()) {
        vector<State*> current_states = pending.front();
        pending.pop();
        State* current_dfa_state = dfa_states[current_states];

        for (char c = 'a'; c <= 'z'; ++c) {
            vector<State*> next_states;
            for (State* nfa_state : current_states) {
                vector<State*> neighbors = nfa_state->get_neighbor(c);
                for (State* neighbor : neighbors) {
                    vector<State*> epsilon_closure = neighbor->get_epsilon_closure();
                    next_states.insert(next_states.end(), epsilon_closure.begin(), epsilon_closure.end());
                }
            }

            if (!next_states.empty()) {
                sort(next_states.begin(), next_states.end());
                if (dfa_states.find(next_states) == dfa_states.end()) {
                    State* new_dfa_state = new State("{");
                    for (State* state : next_states) {
                        new_dfa_state->name += state->name + ",";
                    }
                    new_dfa_state->name.pop_back(); // Remove the trailing comma
                    new_dfa_state->name += "}";
                    dfa_states[next_states] = new_dfa_state;
                    pending.push(next_states);
                    num_dfa_states++; // Increment the number of DFA states
                }
                current_dfa_state->add_neighbor(c, dfa_states[next_states]);
            }
        }
    }

    return num_dfa_states;
}

int main() {
     State * start;
    int n;
    cin >> n;
    string temp;
    cin >> temp;
    start = new State(temp);

    for(int  i = 1 ; i < n ; i ++)
    {
        cin >> temp;
        new State(temp);
    }
    cin >> n;
    vector <char> alphabet(n);
    for(int  i = 0 ; i < n ; i ++) 
    {
        cin >> alphabet[i];
    }
    cin >> n;
    for(int  i = 0 ; i < n; i++)
    {
        cin >> temp;
        State::make_final(temp);
    }
    cin >> n;
    string a , b;
    char c;
    for(int i = 0 ; i  < n; i ++)
    {
        cin >> a;
        c = a[a.find_first_of(',') + 1] ;
        string k = a.substr(0 , a.find_first_of(','))  , kk = a.substr(a.find_last_of(',') + 1);
        State::add_neighbor(k, c ,kk );
    }

    cout<<convert_NFA_to_DFA(start)<<endl;
}

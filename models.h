#include <bits/stdc++.h>
using std::vector;
using std::map;
// using std::multimap;
using std::string;



class State
{
public:
    static map<string , State*> states;
    string name;
    bool final_state;
    State(string name) : name(name)
    {
        final_state = false;
        State::states.insert({name , this});
    }
    static void add_neighbor (string start , char input , string dest )
    {
        State::states[start]->add_neighbor(input , State::states[dest]);
    }
    vector<State*> get_neighbor(char input)
    {
        vector <State*> ans;
        for(auto haha : neighbors[input])
        {
            ans.push_back(haha);
        }
        for(auto haha : neighbors['$'])
        {
            ans.push_back(haha);
        }
        return ans;
    }
    void add_neighbor(char input , State* dest)
    {
        neighbors[input].push_back(dest);
    }
private:
    map <char , vector <State*>> neighbors;

};
#include <bits/stdc++.h>
// #include <models.h>
using namespace std;
class State;

map<string , State*> states;
class State
{
public:
    string name;
    bool final_state;
    State(string name) : name(name)
    {
        // State.states["hi"] = nullptr;
        final_state = false;
        states.insert({name , this});
    }
    static void add_neighbor (string &start , char input , string &dest )
    {
        states[start]->add_neighbor(input , states[dest]);
        // if (input == '')states[dest]->stalkers.insert(states[start]);
    }

    vector<State*> get_neighbor(char input)
    {
        // cout << "asd";
        // vector <State*> ans;
        // for(auto haha : neighbors[input])
        // {
        //     ans.push_back(haha);
        // }
        // return ans;
        return neighbors[input];
    }
    vector<State*> get_sami()
    {
        // cout << "asd";
        
        // vector <State*> ans;
        // for(auto haha : neighbors['$'])
        // {
        //     ans.push_back(haha);
        // }
        // return ans;
        return neighbors['$'];
    }
    
    void add_neighbor(char input , State* dest)
    {
        neighbors[input].push_back(dest);
    }
    static void make_finale(string name)
    {
        states[name]->final_state = true;
    }
    map <char , vector <State*>> neighbors;
    set<State*> stalkers;
private:

};


bool NFA(string pattern , State * curr , int i)
{
    // cout << curr->name << endl;
        // cout <<"joke"; 
    if(pattern.size() == i)
    {
        // cout << curr->final_state;
    return curr->final_state;
        
    }
     
    vector <State *> neighbor = curr->get_neighbor(pattern[i]);

    for (auto haha : neighbor)
    {
        if (NFA(pattern , haha , i + 1))
            return true;
    }
    for (auto haha : curr->get_sami())
    {
        if (NFA(pattern , haha , i))
            return true;
    }
    return false;

}

int main()
{
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
        State::make_finale(temp);
    }
    cin >> n;
    string a , b;
    char c;
    for(int i = 0 ; i  < n; i ++)
    {
        cin >> a;

        // cout << a;
        // c= a.substr(a.find_first_of(',') ,a.las )[0];
        c = a[a.find_first_of(',') + 1] ;
        string k = a.substr(0 , a.find_first_of(','))  , kk = a.substr(a.find_last_of(',') + 1);
        State::add_neighbor(k, c ,kk );
    }
    // cout << states["q3"]->final_state;
    string pattern;
    cin >> pattern;
    // cout << start->neighbors.size();
    cout << (NFA(pattern , start , 0) ? "Accepted" : "Rejected");
}
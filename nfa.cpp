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
        final_state = false;
        states.insert({name , this});
    }
    static void add_neighbor (string &start , char input , string &dest )
    {
        states[start]->add_neighbor(input , states[dest]);
    }

    vector<State*> get_neighbor(char input)
    {
        return neighbors[input];
    }
    vector<State*> get_sami()
    {
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
private:
    map <char , vector <State*>> neighbors;

};


bool NFA(string pattern , State * curr , int i)
{
    if(pattern.size() == i) return curr->final_state;
     
    for (auto haha : curr->get_neighbor(pattern[i]))
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
        c = a[a.find_first_of(',') + 1] ;
        string k = a.substr(0 , a.find_first_of(','))  , kk = a.substr(a.find_last_of(',') + 1);
        State::add_neighbor(k, c ,kk );
    }
    string pattern;
    cin >> pattern;
    cout << (NFA(pattern , start , 0) ? "Accepted" : "Rejected");
}
#include <bits/stdc++.h>
// #include <models.h>
using namespace std;
class State;

map<string , State*> states;
class State
{
public:
    string name;
    bool final_state , visited;
    State(string name) : name(name)
    {
        final_state = false;
        visited = false;
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
    map <char , vector <State*>> neighbors;
private:

};
struct NFAS
{
    State * start , * end;
};


bool NFA(string &pattern , State * curr , int i)
{
    // cout << curr->name << " " << i <<endl;
    if(pattern.size() == i) 
    {
        // cout <<"asd";
        if( curr->final_state) return true;
        for (auto haha : curr->get_sami())
        {
            if (NFA(pattern , haha , i))
                return true;
        }
    }
     
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

string create_name()
{
    static int counter = -1;
    counter ++;
    return ("q" + std::to_string(counter));
}

bool is_operator(char c)
{
    return c=='|' || c=='+' || c=='*' || c=='.';
    // return c=='|' || c=='+' ;
}

int get_priority(char oprtr)
{
    if (oprtr== '*') return 2;
    if (oprtr== '+') return 2;
    if (oprtr== '.') return 1;
    if (oprtr== '|') return 0;
    return -1;
}

string infix_to_postfix(string &reg )
{
    stack<char> operators;
    string postfix;
    for(int i = 0 ; i < reg.size() ; i++)
    {
        // cout << postfix << endl;
        if(is_operator(reg[i]))
        {
            while (!operators.empty() && get_priority( operators.top()) >= get_priority(reg[i]))
            {
                postfix += operators.top();
                operators.pop();
            }
            // cout << "added to stack " << reg[i] << endl;
            operators.push(reg[i]);
        }
        else if (reg[i] == '(') operators.push(reg[i]);
        else if (reg[i] == ')')
        {
            while (operators.top()!='(')
            {
                postfix += operators.top();
                operators.pop();
            }
            operators.pop();

        }
        else
        {
            postfix += reg[i];  
        }
    }
    while (!operators.empty())
    {
        postfix += operators.top();
        operators.pop();
    }
    // cout << "asd";
    return postfix;
}


NFAS postfix_to_nfa(string &postfix)
{
    stack <NFAS> nfas;
    State * start = new State(create_name());
    nfas.push({start , start});
    NFAS nfa1 , nfa2;
    for(int i = 0 ; i < postfix.size() ; i ++)
    {
        if (is_operator(postfix[i]))
        {
            if (postfix[i] == '+')
            {
                nfa1 = nfas.top(); nfas.pop();
                // State * temp = new State(create_name());
                // temp->add_neighbor('$' , nfa1.start);
                // nfa1.start = temp;
                nfa1.end->add_neighbor('$' , nfa1.start);
                nfas.push(nfa1);
            }
            else if (postfix[i] == '*')
            {
                nfa1 = nfas.top(); nfas.pop();
                // State * pre_start = nfa1.start;
                // State * temp = new State(create_name());
                // temp->add_neighbor('$' , nfa1.start);
                // nfa1.start = temp;
                // nfa1.end->add_neighbor('$' , nfa1.start); 
            
                // temp = new State(create_name());
                // nfa1.end->add_neighbor('$' , temp);
                // temp->add_neighbor( '$' , pre_start);
                // nfa1.end = temp;
                // nfa1.start->add_neighbor( '$' , nfa1.end); // for one

                nfa1.end->add_neighbor('$' , nfa1.start);
                State *temp = new State(create_name());
                nfa1.start->add_neighbor('$' , temp);
                nfa1.end = temp;

                nfas.push(nfa1);
            }
            else if (postfix[i] == '|')
            {
                nfa1 = nfas.top(); nfas.pop();
                nfa2 = nfas.top(); nfas.pop();
                State * temp = new State(create_name());
                temp->add_neighbor('$' , nfa1.start);
                temp->add_neighbor('$' , nfa2.start);
                nfa1.start = temp;
                temp = new State(create_name());
                nfa1.end->add_neighbor('$' , temp);
                nfa2.end->add_neighbor('$' , temp);
                nfa1.end = temp;
                nfas.push(nfa1);
            }
            else if (postfix[i] == '.')
            {
                nfa1 = nfas.top(); nfas.pop();
                nfa2 = nfas.top(); nfas.pop();
                nfa2.end->add_neighbor('$' , nfa1.start);
                nfas.push({nfa2.start , nfa1.end});
            }
        }
        else
        {
            if (postfix[i] =='\\')
            {
                i++;
            }
            // nfa1 = nfas.top(); nfas.pop();
            // State * temp = new State(create_name());
            // nfa1.end->add_neighbor(postfix[i] , temp);
            // nfa1.end = temp;
            nfa1.start = new State(create_name());
            nfa1.end = new State(create_name());
            nfa1.start->add_neighbor(postfix[i] , nfa1.end);
            nfas.push(nfa1);
        }
    }
    // cout << nfas.size() << endl;
    while (!nfas.empty())
    {
        if (nfas.size() == 1) break;
        nfa1 = nfas.top(); nfas.pop();
        nfa2 = nfas.top(); nfas.pop();
        nfa2.end->add_neighbor('$' , nfa1.start);
        nfas.push({nfa2.start , nfa1.end});
    }
    // if(nfas.size()!=1) cout << "asd";
    return nfas.top();
}

void print_nfa(State * curr)
{
    if (curr->visited) return;
    curr->visited = true;
    for( auto ha : curr->neighbors)
    {
        for(State * he : ha.second)
        {
            cout << curr->name << " -> " << he->name << " with " << ha.first << " is finale ? " << he->final_state <<endl;
            print_nfa(he);
        }
    }
}

string add_concat(string reg)
{
    bool flag = false;
    for(int i = 0 ; i < reg.size() ; i++)
    {
        if (!is_operator(reg[i]) && (reg[i] != '(') && !flag) { if(reg[i] == '\\') i++; flag = true; continue;}
        if(
            ((!is_operator(reg[i])) || reg[i] == '(') && 
            (reg[i] != ')') && 
            ( i==0 || reg[i - 1] !='|') &&
            ( i==0 || reg[i - 1] !='(') && flag)
        {
            // if (!flag) {if (reg[i] != '(') flag = true; continue;}
            // cout << "wow";
            reg = reg.substr(0 , i) + '.' + reg.substr(i);
            i++;
            if(reg[i] == '\\') i++;
        }
        // else if (reg[i] == '\\' && flag)
        // {
        //     if ( ( i!=0 && reg[i - 1] =='|')) continue;
        //     reg = reg.substr(0 , i) + '.' + reg.substr(i);
        //     i+=2;
        // }
    }
    return reg;
}

int main()
{
    
    State * start;
    int n;
    cin >> n;
    vector <char> alphabet(n);
    for(int  i = 0 ; i < n ; i ++) 
    {
        cin >> alphabet[i];
    }
    string reg , pattern , postfix;
    cin >> reg;

    reg = add_concat(reg);
    // cout << reg << endl;
    postfix = infix_to_postfix(reg);
    // cout << "asd";
    // cout << postfix << endl;
    NFAS res = postfix_to_nfa (postfix);
    // start = new State(create_name());
    // cout << res.start->name << endl;
    res.end->final_state = true;
    // print_nfa(res.start);
    cin >> pattern;
    
    cout << (NFA(pattern , res.start , 0) ? "Accepted" : "Rejected");
}
// C++ program to evaluate value of a postfix expression
#include <bits/stdc++.h>
using namespace std;

// The main function that returns value 
// of a given postfix expression
int evaluatePostfix(string exp)
{
	// Create a stack of capacity equal to expression size
	stack<int> st;

	// Scan all characters one by one
	for (int i = 0; i < exp.size(); ++i) {
		
		// If the scanned character is an operand 
		// (number here), push it to the stack.
		if (isdigit(exp[i]))
			st.push(exp[i] - '0');

		// If the scanned character is an operator, 
		// pop two elements from stack apply the operator
		else {
			int val1 = st.top();
			st.pop();
			int val2 = st.top();
			st.pop();
			switch (exp[i]) {
			case '+':
				st.push(val2 + val1);
				break;
			case '-':
				st.push(val2 - val1);
				break;
			case '*':
				st.push(val2 * val1);
				break;
			case '/':
				st.push(val2 / val1);
				break;
			}
		}
	}
	return st.top();
}

// Driver code
int main()
{
	string exp = "++1/*2345";

	// Function call
	cout << "postfix evaluation: " << evaluatePostfix(exp);
	return 0;
}

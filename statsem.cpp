#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctype.h>
//#include <deque>
#include <stack>
#include "parser.h"
#include "tree.h"
#include "statsem.h"

using namespace std;

vector<string> globals;
stack<string> myStack;

void statSemantics(node* root) {
	cout << "Made it to static semantics" << endl;
	
	/*myStack.push("This");
	myStack.push("is");
	myStack.push("an");
	myStack.push("example");
	myStack.push("test");
	
	cout << "Size: " << myStack.size() << endl;
	cout << find("an") << endl;
	cout << find("example") << endl;
	cout << "Size: " << myStack.size() << endl;*/

	traverse(root);
}

int find(string myStr) {
	stack<string> tempStack = myStack;
	int i = 0;
	int firstOccurrence = -1;
	while (!tempStack.empty()) {
		if (tempStack.top() == myStr) {
			firstOccurrence = i;
			break;
		}
		tempStack.pop();
		i++;
	}
	return firstOccurrence;
}

void traverse(node* myNode) {
	if (myNode == NULL) return;
}

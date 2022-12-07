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
	int found;
	int varsCount = 0;
	if (myNode->tk->instance == "<vars>") {
		if (myNode->first->tk->instance != "Empty") {
			found = find(myNode->first->tk->instance);
			if (found != -1) {
				statSemanticsError("Duplicate variable name", myNode->first->tk->instance, myNode->first->tk->lineNum);
			} else {
				varsCount++;
				myStack.push(myNode->first->tk->instance);
			}
			if (myNode->third->tk->instance != "Empty") {
				traverse(myNode->third);
			}
		}
	} else if (myNode->tk->instance == "<R>") {
		if (myNode->first->tk->tokenType == "identifier") {
			found = find(myNode->first->tk->instance);
			if (found == -1) {
				statSemanticsError("Unknown variable", myNode->first->tk->instance, myNode->first->tk->lineNum);
			}
		} else {
			traverse(myNode->first);
		}
	} else if (myNode->tk->instance == "<assign>") {
		found = find(myNode->first->tk->instance);
		if (found == -1) {
			statSemanticsError("Assigning unknown variable", myNode->first->tk->instance, myNode->first->tk->lineNum);
		}
		traverse(myNode->second);
	} else {
		traverse(myNode->first);
		traverse(myNode->second);
		traverse(myNode->third);
		traverse(myNode->fourth);
		traverse(myNode->fifth);
	}
	if (varsCount != 0) {
		while (varsCount > 0) {
			myStack.pop();
			varsCount--;
		}
	}
}

void statSemanticsError(string error, string variable, int lineNumber) {
	/***** Possible Errors *****/
	// "Duplicate variable name"
	// "Unknown variable"
	// "Assigning unknown variable"
	
}

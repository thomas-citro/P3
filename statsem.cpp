#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctype.h>
#include <stack>
#include "parser.h"
#include "tree.h"
#include "statsem.h"

using namespace std;

stack<string> myStack;
string fileName2;
bool readingGlobals;

void statSemantics(node* root, string file) {
	fileName2 = file;
	
	readingGlobals = true;
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
				if (readingGlobals) varsCount--; // To keep globals on the stack
			}
			if (myNode->third->tk->instance != "Empty") {
				traverse(myNode->third);
			}
		}
		if (readingGlobals) {
			readingGlobals = false;
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
	
	// Get error line
	ifstream file;
	file.open(fileName2);
	if (file.fail()) {
		cout << "File failed to open." << endl;
		exit(0);
	}
	int currentLineNumber = 0;
	string currentLine;
	while (!file.eof()) {
		currentLineNumber++;
		getline(file, currentLine);
		if (currentLineNumber == lineNumber) break;
	}
	if (currentLineNumber < lineNumber) {
		cout << "Line not found!" << endl;
		exit(0);
	}
	file.close();
	
	// Output error
	string message;
	if (error == "Duplicate variable name") {
		message = "Variable with the name '" + variable + "' already defined in this scope.";
	} else if (error == "Unknown variable") {
		message = "Variable with the name '" + variable + "' is not defined in this scope.";
	} else if (error == "Assigning unknown variable") {
		message = "Attempting to assign undeclared variable with the name '" + variable + "'";
	}
	cout << "Static Semantics Error: " << message << endl;
	cout << "   ^ On line number " << lineNumber << ": " << currentLine << endl;
	exit(0);
}

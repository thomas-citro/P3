#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <ctype.h>
#include <deque>
#include "parser.h"
#include "tree.h"
#include "statsem.h"

using namespace std;

vector<string> globals;
deque<string> stack;

void statSemantics(node* root) {
	cout << "Made it to static semantics" << endl;
	
	stack.push_front("This");
	stack.push_front("is");
	stack.push_front("an");
	stack.push_front("example");
	stack.push_front("test");
	
	cout << find("an") << endl;
	cout << find("example") << endl;
}

int find(string myStr) {
	deque<string>::iterator it;
	int i = 0;
	int firstOccurrence = -1;
	for (it = stack.begin(); it != stack.end(); ++it) {
		if (*it == myStr) {
			firstOccurrence = i;
			break;
		}
		i++;
	}
	return firstOccurrence;	
}

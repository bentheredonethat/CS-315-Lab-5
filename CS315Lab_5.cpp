#pragma warning(disable: 4996)
#include <string>
#include <iostream>
#include <ostream>
#include <vector>
#include <stack>
#include <cstdlib>
#include <iomanip>
#include <fstream>
using namespace std; 

class String{
private:
	enum { SZ = 80 };
	char str[SZ];
public:
	String(){ strcpy(str, ""); }
	String(char s[]){ strcpy(str, s); }
	void display() const{ cout << str; }
	void getstr(){ cin.get(str, SZ); }
	bool operator == (String ss) const{
		return (strcmp(str, ss.str) == 0) ? true : false;
	}
};


class Node{
public:
	int tag; // if tag is 0 then key node (i.e. number node), if 1 then list node
	int key;
	Node *Down;
	Node *Next;


	Node(string token); // key node constructor
	Node(Node * newnode); //list node constructor
};



Node::Node(string token) { // key node constructor
	Next = NULL;
	Down = NULL;
	tag = 0;// set tag to 0
	key = stoi(token); // key is value of token
}


Node::Node(Node *newnode) { // list node constructor
	Next = NULL;
	Down = newnode;
	tag = 1;
	key = NULL; // key is value of token
}

Node *  append(Node *listnode, Node *newnode) { // adds newnode to end of list and returns list 
	if (listnode) {
		Node * last = listnode;	 // pointer to list node	
		while (last->Next != NULL) last = last->Next; // find the last 'topmost' list node via its next pointing null
		last->Next = newnode; // set the last list node to now point to newnode
		return listnode; // return type is pointer to node so have to return pointer to a node
	}
	else {
		return newnode;
	}
}

// acts as a 'proper' pop function 
// removes and returns top of stack
Node *pop(vector <Node*> &v){
	Node * finalelement = v[v.size() - 1];
	v.pop_back();
	return finalelement;
}

// this function takes in array of tokens and returns corresponding nested list structure 
Node * Convert(string listoftokens[]){
	vector <Node*> stackofnodes; // stack of nested lists; 
	// * Node = 'list' in our lexicon
	for (int CurrentToken = 0; listoftokens[CurrentToken].length(); CurrentToken++)  { // as long as the current string in the list exists continue
		if (listoftokens[CurrentToken].compare("[") == 0) stackofnodes.push_back(NULL);  // when see a "[", push an empty list(NULL) on the stack

		else if (listoftokens[CurrentToken].compare("]") == 0) { // if see closing bracket then first pop the top of stack into temp variable
			// if size of vector is one, then stop return top of stack
			//if (stackofnodes.size == 1) return pop(stackofnodes);
			Node * temp = pop(stackofnodes); // list on top of stack
			if (stackofnodes.size() == 0) return temp;
			stackofnodes.push_back(append(pop(stackofnodes), new Node(temp)));
		}
		else// current token is number 
			// the second parameter in append function above should be address to list node pointing to number node with value of current token
			// append newly constructed listnode pointing to newly constructed number node to list that is now on top stack


			// high level comment
			//  add number to end of list on top of stack; handles special case when an empty list is on top of the stack
			stackofnodes.push_back(append(pop(stackofnodes), new Node(new Node(listoftokens[CurrentToken]))));
		 
	}
}

// this function read a list and print all the keys within the structure with each key's corresponding depth
// parameters: list and depth. depth is Zero because in main the function hs not yet gone 'down' once
void printdepth(Node * L, int depth) { // depth should be 0 when called from main
	// while there is a current node, either print key value on that node or go down
	for (Node * currentnode = L; currentnode; currentnode = currentnode->Next) {
		// if currentnode is a number node then print and advance through list of listnodes
		if (currentnode->key) cout << "(" << currentnode->key << "," << depth << ") ";
		// if currently on listnode then recurse on current list and depth increase by one since going 'down' in list
		else if (currentnode->Down) printdepth(currentnode->Down, depth + 1);
	}
}


//function size(input: node N) {
int size(Node * N) {
//	if (N is empty) return 0;
	if (!N) return 0;
//	else if (N contains a key) return 1;
	else if (N->key) return 1;
//	else return size(N->down) + size(N->next);
	else return size(N->Down) + size(N->Next);

}

void update1(Node * L, int & position, int value) {
	if (!L || position == 0) return; // if node is null or already changed number node return
	else if (L->tag == 0) { // number node
		if (--position == 0) L->key = value; 	
	}
	else {  // list nodes
		update1(L->Down, position, value);
		update1(L->Next, position, value);
	}
}


// this function will replace jth key and replace with value k
void update(Node * L, int  position, int value) {
	update1(L, position, value);
	
}


 // end update 




// OBSERVATIONS
// use tags to determine type of node
// get rid of current and decrement position
// what's the mechanism by which all calls share the value of "position"
	// can do it via a global
    // another way is ....   &    
    // little problem:  int i = 3; update(list,i,5);  cout << i  
    // what's the solution?  
	//   update(Node *l, int position,int value)  { int interenalposition = position; update1(l,internalposition,value); }
// using recursion to iterate over a list
    // on a list, iterate over it 
    // clean up confusion over next on number nodes
// how do you get out once you're done?
// get rid of returning a value



// this function will take ina nested list and make all the numbernodes have a depth of one
void flatten(Node * L) {
	for (Node * currentnode = L; currentnode->Next; currentnode = currentnode->Next) { // iterate through all the least depth nodes
		if (L->tag == 1) { // if current node points to a list of lists 
			// point next of the corresponding last node that current starts at
			Node * lastnode = currentnode->Down;
			while (lastnode->Next) {
				lastnode = lastnode->Next;
				if (lastnode->tag == 1) flatten(currentnode);
				// point the next of previous node to where down points
				L->Next = currentnode;
				// set L->Down to null and check for any other L->Down's in current list
				currentnode->Down = NULL;
			}
			lastnode->Next = currentnode->Next;

		}
	}

}
int main(){
	//string input = "[2, 3]";
	string input = " [4, [12, 4, 3], [[5, 3, -1], 8], [12, 8]]  ";
	// here is where string is tokenized
	string listoftokens[50];
	int numberoftokens=0;
	for (int i = 0; i < input.size(); i++) {

		if (input[i] == '[') listoftokens[numberoftokens++] = input[i]; 
		else if (input[i] == ']') listoftokens[numberoftokens++] = input[i];
		else if (isdigit(input[i]) || input[i] == '-') { // if current char in the input string is a digit or a unary minus then  
			listoftokens[numberoftokens] = input[i];
			while (isdigit(input[i+1])) // check if next char in input string is digit
			{
				listoftokens[numberoftokens] += input[++i];// append digit to end of string 
			}
			numberoftokens++;
		}

	}
		
	Node * Listoflists = Convert(listoftokens);
	cout << "did not crash on convert\n";
	printdepth(Listoflists, 0);
	cout << endl;
	cout << "testing update function\n" << endl;
	// first paramter is the list to update, second is position and third is value to replace with
	update(Listoflists, 5, 99);
	cout << "did not crash on update\n" << endl;
	printdepth(Listoflists, 0);
	flatten(Listoflists);
	cout << "did not crash on flatten\n" << endl;
	printdepth(Listoflists, 0);
	cout << endl;
	return 0;
}

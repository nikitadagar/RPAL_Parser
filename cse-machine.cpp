#include "header.h"

int env_count = 0;


void run_machine (Node* root) {

	stack<cseNode*> c_control;		//control stack
	stack<cseNode*> s_stack;		//program stack
	
	int lambdas = countLambda(root); 
	
	queue<cseNode*>* deltas[lambdas+1];		//create an array of queues for the deltas
	buildControls(root, deltas);			// populate the control structures

	unordered_map<string, string>* envs[lambdas+1];	//array of pointers to maps of environments

	cseNode* en = createNextEnv(envs);
	c_control.push(en);

	load_control(1, c_control, deltas);
	printControlStack(c_control);


}

//populates the array of all the control structures (deltas)
void buildControls (Node* root, queue<cseNode*>* deltas[]) {
	
	int lambdas = countLambda(root); 	//count number of lambdas in the tree
	Node* subtreeRoots[lambdas+1];
	subtreeRoots[0] = root;

	int count = 0;	//count of number of lambdas encountered

	for (int i = 0; i <= lambdas; i++) {
		deltas[i] = new queue<cseNode*>; 
		createControlStructure(subtreeRoots[i], *deltas[i], count, subtreeRoots);
	}

	for(int j = 0; j < lambdas + 1; j++) {
		queue<cseNode*> q = *deltas[j];
		cout << "D" << j << ":\n";
		while(!q.empty()) {
			cout << q.front()->name << "\n";
			q.pop();
		}
		cout << "\n";
	}
}

//root is either the root of the whole tree or lambda
//funtion creates a control structure starting at the given node
void createControlStructure (Node* root, queue<cseNode*> &q, int &count, Node* subtrees[]) {

	cseNode* n = new cseNode;

	if (root->name == "lambda") {
		//create new lambda node
		n->name = "lambda";
		n->i = ++count;
		n->x = root->child[0]->name;

		q.push(n);

		subtrees[count] = root->child[1];
		// return;
	}
	else {
		n->name = root->name;
		q.push(n);
		
		if(root->children > 0) {
			createControlStructure(root->child[0], q, count, subtrees);
			createControlStructure(root->child[1], q, count, subtrees);
		}
	}
}

//counts number of lambdas in the whole AST
int countLambda (Node* root) {
	preOrder(root);
	int count = 0;
	while (!q.empty()) {
		if (q.front()->name == "lambda") {
			count++;
		}
		q.pop();
	}
	return count;
}

//Adds the nodes of a tree to the global queue in a pre-order manner
void preOrder(Node* root) {

	q.push(root);
	
	if (root->children > 0){
		preOrder(root->child[0]);
		preOrder(root->child[1]);
	}
}

//prints the global queue
void printQueue () {
	while(!q.empty()) {
		cout <<"\n" << q.front()->name;
		q.pop();
	}
}

//prints the queue at the provided root
void printQueue (queue<cseNode*> q) {
	while (!q.empty()) {
		cout << q.front()->name << "\n";
		q.pop();
	}
}

cseNode* newCSENode (string name, string type) {
	cseNode* node = new cseNode;
	node->name = name;
	node->type = type;
	return node;
}

//create next environment
cseNode* createNextEnv (unordered_map<string, string>* envs[]) {

	cseNode* env = newCSENode ("e" + to_string(env_count),"env");
	envs[env_count] = new unordered_map<string, string>;
	env_count++;
	return env;
}

void load_control (int d, stack<cseNode*> &c_control, queue<cseNode*>* deltas[]) {
	
	queue<cseNode*> del = *deltas[d];
	while (!del.empty()) {
		c_control.push(del.front());
		del.pop();
	}
}
//helper function to print stack
//can be deleted after completion
void printControlStack(stack<cseNode*> controlStack) {
	cseNode* node;
	cout << "\ncontrol stack:" << "\n"; 
	while(!controlStack.empty()) {
		node = controlStack.top();
		cout << node->name << " ";
		controlStack.pop();
	}
	cout << "\n\n";
}

















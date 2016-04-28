#include "header.h"

int env_count = 0;
stack<int> current_env;

void run_machine (Node* root) {

	stack<cseNode*> c_control;		//control stack
	stack<cseNode*> s_stack;		//program stack
	
	int lambdas = countLambda(root); 
	
	queue<cseNode*>* deltas[lambdas+1];		//create an array of queues for the deltas
	buildControls(root, deltas);			// populate the control structures

	unordered_map<string, cseNode>* envs[lambdas+1];	//array of pointers to maps of environments

	// initializing the control and stack 
	// adding e0 to both
	cseNode* en = createNextEnv(envs, -1);
	c_control.push(en);
	s_stack.push(en);
	load_control(0, c_control, deltas);

	// printDeltas (lambdas, deltas);
	start_machine(envs, c_control, s_stack, deltas);

	
	//printControlStack(c_control);
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
}

//root is either the root of the whole tree or lambda
//funtion creates a control structure starting at the given node
void createControlStructure (Node* root, queue<cseNode*> &qu, int &count, Node* subtrees[]) {

	cseNode* n = new cseNode;

	if (root->name == "lambda") {
		//create new lambda node
		n->name = "lambda";
		n->i = ++count;
		
		if (root->child[0]->name == ",") {
			//create the array superscript of lambda of the form x,y
			string temp = "";
			for (int i=0; i < root->child[0]->children; i++) {
				if (temp == "") {
					temp = temp + root->child[0]->child[i]->name;
				}
				else {
					temp = temp + "," + root->child[0]->child[i]->name;
				}
			}
			n->type = "n-ary";
			n->x = temp;
		}
		else {
			n->x = root->child[0]->name;
		}

		qu.push(n);

		subtrees[count] = root->child[1];
	}
	else if (root->name == "->") {

		subtrees[++count] = root->child[1];
		cseNode* del1 = new cseNode;	//push delta node for true
		del1->name = "delta";
		del1->type = "true";
		del1->i = count;
		qu.push(del1);

		subtrees[++count] = root->child[2];	//add to subtree array to generate control structures
		cseNode* del2 = new cseNode;	//push delta node for flase
		del2->name = "delta";
		del2->type = "false";
		del2->i = count;
		qu.push(del2);

		n->name = "beta";			//push the beta node
		qu.push(n);

		createControlStructure(root->child[0], qu, count, subtrees);
	}

	else if (root->name == "tau") {
		
		n->name = "tau";		//add the tau node to the current control structure
		n->i = root->children;
		qu.push(n);

		//add each of tau's children to the subtree node
		for (int i = 0; i < root->children; i++) {
			subtrees[++count] = root->child[i];
			cseNode* del = new cseNode;
			del->name = "delta";
			del->type = "delta";
			del->i = count;
			qu.push(del);
		}
	}

	else {
		n->name = root->name;
		if (root->name == "<ID:Print>" || root->name == "<ID:Stem>") {
			n->type = "func";
		}
		qu.push(n);
		for(int i = 0; i < root->children; i++) {
			createControlStructure(root->child[i], qu, count, subtrees);
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
		else if (q.front()->name == "->") {
			count = count+2;
		}
		else if (q.front()->name == "tau") {
			count = count+root->children;
		}
		q.pop();
	}
	return count;
}

//Adds the nodes of a tree to the global queue in a pre-order manner
void preOrder(Node* root) {

	q.push(root);
	
	for(int i = 0; i < root->children; i++) {
		preOrder(root->child[i]);
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
void printQueue (queue<cseNode*> qu) {
	while (!qu.empty()) {
		cout << qu.front()->name << "\n";
		qu.pop();
	}
}

cseNode* newCSENode (string name, string type) {
	cseNode* node = new cseNode;
	node->name = name;
	node->type = type;
	return node;
}

//create next environment
cseNode* createNextEnv (unordered_map<string, cseNode>* envs[], int k) {

	cseNode* env = newCSENode ("e" + to_string(env_count),"env");	//new CSE Node of type env
	envs[env_count] = new unordered_map<string, cseNode>;	//add empty env map to array of envs

	if (k >= 0){		//for all non primitive envs
		
		int parentEnvInd = k;
		unordered_map<string,cseNode> parentEnv = *envs[parentEnvInd];
		unordered_map<string,cseNode> *currentEnv = envs[env_count];
		(*currentEnv).insert(parentEnv.begin(), parentEnv.end()); //copy parent environment 

	}

	current_env.push(env_count);	//stack to keep track of current environment
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
void printStack(stack<cseNode*> controlStack) {
	cseNode* node;
	while(!controlStack.empty()) {
		node = controlStack.top();
		cout << node->name << " ";
		controlStack.pop();
	}
	cout << "\n\n";
}

//helper funciton to print environments
void printMap (unordered_map<string, string> e0) {
	typedef unordered_map<string, string>::iterator iter;
	for(iter iterator = e0.begin(); iterator != e0.end(); iterator++) {
    	cout<< "\nKey : " << iterator->first << " value : " << iterator->second << "\n"; 
	}

}

void printDeltas (int lambdas, queue<cseNode*>* deltas[]) {

	cout << "printing deltas : \n";
	for(int j = 0; j < lambdas + 1; j++) {
		queue<cseNode*> qu = *deltas[j];
		cout << "D" << j << ":\n";
		while(!qu.empty()) {
			cout << qu.front()->name << "\n";
			qu.pop();
		}
		cout << "\n";
	}
}















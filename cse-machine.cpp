#include "header.h"



void preOrder(Node* root) {

	q.push(root);
	
	if (root->children > 0){
		preOrder(root->child[0]);
		preOrder(root->child[1]);
	}
}

void printQueue () {
	while(!q.empty()) {
		cout <<"\n" << q.front()->name;
		q.pop();
	}
}

void printQueue (queue<cseNode*> q) {
	while (!q.empty()) {
		cout << q.front()->name << "\n";
		q.pop();
	}
}

//buidControl func
void buildControl (Node* root) {
	
	int lambdas = countLambda(root); 	//count number of lambdas in the tree
	Node* subtreeRoots[lambdas+1];
	subtreeRoots[0] = root;

	int count = 0;	//count of number of lambdas encountered
	queue<cseNode*>* deltas[lambdas+1];		//array of all deltas (control structures)


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




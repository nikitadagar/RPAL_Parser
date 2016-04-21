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

// //buidControl func
// void buildControl (Node* root) {
// 	int lambdas = countLambda(root);
// 	Node* deltas[lambdas];

// 	int count = 0;

// 	queue<cseNode*> d0; 
// 	createControlStructure(root, d0, count, deltas);


// }

// //root is either the root of the whole tree or lambda
// //funtion creates a control structure starting at the given node
// void createControlStructure (Node* root, queue<cseNode*> &q, int &count, Node* deltas[]) {

// 	cseNode* n = new cseNode;

// 	if (root->name == "lambda") {
// 		//create new lambda node
// 		n->name = "lambda";
// 		n->i = ++count;
// 		n->x = root->child[0]->name;

// 		q.push(n);

// 		deltas[count] = root->child[1];
// 		// return;
// 	}
// 	else {
// 		n->name = root->name;
// 		q.push(n);
		
// 		if(root->children > 0) {
// 			createControlStructure(root->child[0], q, count, deltas);
// 			createControlStructure(root->child[1], q, count, deltas);
// 		}
// 	}
// }

// int countLambda (Node* root) {
// 	preOrder(root);
// 	int count = 0;
// 	while (!q.empty()) {
// 		if (q.front()->name == "lambda") {
// 			count++;
// 		}
// 		q.pop();
// 	}
// 	return count;
// }




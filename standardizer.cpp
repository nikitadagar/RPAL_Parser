#include "header.h"

Node* standardize(Node* root) {
	Node* result;
	
	if (root->name == "let") {
		result = standardizeLet(root);
	}
	else if (root->name == "where") {
		result = standardizeWhere(root);
	}
	else if (root->name == "within") {
		result = standardizeWithin(root);
	}
	else if (root->name == "function_form") {
		result = standardizeFuncForm(root);
	}
	else if (root->name == "and") {
		result = standardizeAnd(root);
	}
	else if (root->name == "@") {
		result = standardizeAt(root);
	}
	else if (root->name == "rec") {
		result = standardizeRec(root);
	}
	else if(root->children > 0) { 
		for(int i = 0; i < root->children; i++) {
			root->child[i] = standardize(root->child[i]);
		}
		result = root;
	}
	else {
		result = root;
	}
	return result;
}

Node* standardizeLet (Node* root) {
	
	//standardize children
	root->child[0] = standardize(root->child[0]);
	root->child[1] = standardize(root->child[1]);


	//change names of nodes
	root->name = "gamma";
	root->child[0]->name = "lambda";
	
	//swap pointers and standarize child nodes
	Node* P = root->child[1];
	root->child[1] = root->child[0]->child[1];
	root->child[0]->child[1] = P;

	return root;
}

Node* standardizeWhere (Node* root) {

	//standardize children
	root->child[0] = standardize(root->child[0]);
	root->child[1] = standardize(root->child[1]);


	//change names of nodes
	root->name = "gamma";
	Node* lambda = newNode("lambda", 2);

	lambda->child[0] = root->child[1]->child[0];
	lambda->child[1] = root->child[0];

	root->child[1] = root->child[1]->child[1];
	root->child[0] = lambda;

	return root;
}

Node* standardizeWithin (Node* root) {

	root->child[0] = standardize(root->child[0]);
	root->child[1] = standardize(root->child[1]);

	Node* lambda = newNode("lambda", 2);
	lambda->child[0] = root->child[0]->child[0];
	lambda->child[1] = root->child[1]->child[1];

	Node* gamma = newNode("gamma", 2);
	gamma->child[0] = lambda;
	gamma->child[1] = root->child[0]->child[1];

	root->name = "=";
	root->child[0] = root->child[1]->child[0];
	root->child[1] = gamma;

	return root;
}

Node* standardizeFuncForm (Node* root) {

	root->child[0] = standardize(root->child[0]);
	root->child[root->children-1] = standardize(root->child[root->children-1]);

	Node* P = root->child[0];

	Node* temp;

	for(int i = 0; i < root->children-2; i++) {
		
		Node* lambda = newNode("lambda",2);
		lambda->child[0] = root->child[root->children -i-2];
		
		if (i == 0) {
			lambda->child[1] = root->child[root->children-1];
		}
		else {
			lambda->child[1] = temp;
		}
		temp = lambda;
	}

	root->name = "=";
	root->children = 2;
	root->child = new Node*[2];
	root->child[1] = temp;
	root->child[0] = P;

	

	return root;

}

Node* standardizeAnd(Node *node) {
	//standardize all immediate children
	for(int i = 0; i < node->children; i++) {
		node->child[i] = standardize(node->child[i]);
	}

	node->name = "=";
	Node* tau = newNode("tau", node->children);
	Node* comma = newNode(",", node->children);
	Node* X;
	Node* E;

	//connect up the children for comma and tau
	for(int j = 0; j < node->children; j++) {
		X = node->child[j]->child[0];
		E = node->child[j]->child[1];
		comma->child[j] = X;
		tau->child[j] = E;
	}

	//connect the new comma and tau nodes as the children for the =
	node->children = 2;
	node->child = new Node*[node->children];
	node->child[0] = comma;
	node->child[1] = tau;

	return node;
}

Node* standardizeAt(Node* root) {

	Node* E1 = standardize(root->child[0]);
	Node* N = standardize(root->child[1]);
	Node* E2 = standardize(root->child[2]);

	Node* gamma = newNode("gamma",2);
	gamma->child[0] = N;
	gamma->child[1] = E1;

	root->name = "gamma";
	root->child = new Node*[2];
	root->children = 2;
	root->child[0] = gamma;
	root->child[1] = E2;

	return root;
}

Node* standardizeRec (Node* root) {

	root->child[0] = standardize(root->child[0]);

	Node* X = root->child[0]->child[0];
	Node* E = root->child[0]->child[1];

	Node* lambda = newNode("lambda", 2);
	lambda->child[0] = X;
	lambda->child[1] = E;

	Node* gamma = newNode("gamma", 2);
	gamma->child[0] = newNode("<Y*>",0);
	gamma->child[1] = lambda;

	root->name = "=";
	root->children = 2;
	root->child = new Node*[2];
	root->child[0] = X;
	root->child[1] = gamma;

	return root;
}


Node* newNode (string name, int children) {
	Node* node = new Node;
	node->name = name;
	node->children = children;
	node->child = new Node*[children];
	return node;
}

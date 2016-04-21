#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <stack>
#include <queue>

using namespace std;

// Node structure
typedef struct nodeType{
    string name;
    struct nodeType** child;
    int children;
} Node;

typedef struct cseNode {
	string name;
	int i;	//subscript
	string x;	//superscript
	int k;	//pre-superscript
} cseNode;

extern ifstream in_stream;
queue<Node*> q;

//Building and displaying AST funcitons
void display_tree(Node*, int);
void build_tree(string, int);

//SCANNER FUNCTIONS

string next_token(std::string);

string is_identifier(std::string);
string is_any_identifier();
string is_punctuation(std::string);
string is_digit();
string is_operator(std::string);
string is_string();

void read_identifier();
void read_string();
void read_integer();
void read(std::string);

//HELPER FUNCTIONS FOR SCANNER
void remove_spaces();
bool is_comment();
void remove_comment();
void my_putback(std::string);
void poison_pill();

// PARSER FUNCTIONS FOR NON TERMINALS

void E();
void Ew();
void T();
void Ta();
void Tc();
void Bt();
void Bs();
void Bp();
void B();
void A();
void At();
void Af();
void Ap();
void R();
void Rn();
void D();
void Da();
void Dr();
void Db();
void Vl();
void Vb();

//STANDARDIZE FUNCTIONS

Node* standardize(Node*);
Node* standardizeLet(Node*);
Node* standardizeWhere(Node*);
Node* standardizeWithin(Node*);
Node* standardizeFuncForm(Node*);
Node* standardizeAnd(Node*);
Node* standardizeAt(Node*);
Node* standardizeRec(Node*);
Node* newNode(string, int);

//CSE MACHINE 

void preOrder(Node*);
void printQueue();
void printQueue(queue<cseNode>);
void buildControl (Node*);
void createControlStructure (Node*, queue<cseNode*>&, int&, Node*[]);
int countLambda (Node*); 

#endif
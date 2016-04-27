#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <unordered_set>
#include <stack>
#include <queue>
#include <unordered_map>
#include <string>
#include <sstream>
#include <vector>
#include <algorithm>
#include <cmath>

using namespace std;

// Node structure
typedef struct nodeType{
    string name;
    struct nodeType** child;
    int children;
} Node;

typedef struct cseNode {
	string name;
	string type;
	int i;	//subscript
	string x;	//superscript
	int k;	//pre-superscript
} cseNode;

extern ifstream in_stream;
queue<Node*> q;		//queue which has the preorder of the whole standardized AST
extern stack<int> current_env;
extern int env_count;

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
void buildControls (Node*, queue<cseNode*>*[]);
void createControlStructure (Node*, queue<cseNode*>&, int&, Node*[]);
int countLambda (Node*); 
cseNode* newCSENode (string, string);
cseNode* createNextEnv (unordered_map<string, cseNode>*[]);
void load_control (int, stack<cseNode*>&, queue<cseNode*>*[]);
void printStack(stack<cseNode*>);
void printMap (unordered_map<string, string>);
void printDeltas (int, queue<cseNode*>*[]);

//CSE MACHINE CONTROLLER 
void start_machine (unordered_map<string, cseNode>*[], stack<cseNode*>, stack<cseNode*>, queue<cseNode*>*[]);
bool isInt(string);
bool isID (string);
cseNode* bi_operation (string, string, string);
int extractInt (string);
string extractID (string);
cseNode* un_operation (string, string); 
string augToTuple (string tuple, string val);
void myPrint (string);

#endif





#include "header.h"
#include "scanner.cpp"
#include "standardizer.cpp"
#include "cse-machine.cpp"
#include "cse-machine-controller.cpp"

ifstream in_stream;
stack<Node*> s;

//procedure for Non terminal E
void E () {

	if (next_token("let") == "let"){
		read("let");
		D();
		read("in");
		E();
		build_tree("let",2);
	}
	else if (next_token("fn") == "fn") {
		read("fn");
		int n=1;
		Vb();
		//while we don't see . keep calling Vb
		while (next_token(".") != ".") {
			n++;
			Vb();
		}
		read(".");
		E();
		build_tree("lambda",n+1);
	}
	else {
		Ew();
	}
}

void Ew() {
	T();

	if (next_token("where") == "where"){
		read("where");
		Dr();
		build_tree("where",2);
	}
}

void T () {
	Ta();
	
	int n=1;
	if (next_token(",") == ","){
		while (next_token(",") == ","){
			read(",");
			n++;
			Ta();
		}
		build_tree("tau",n);
	}
}

void Ta() {
	Tc();

	while (next_token("aug") == "aug"){
		read("aug");
		Tc();
		build_tree("aug",2);
	}
}

void Tc() {
	B();
	if (next_token("->") == "->" ){
		read("->");
		Tc();
		read("|");
		Tc();
		build_tree("->",3);
	}
}

void B () {
	Bt ();
	while (next_token("or") == "or"){
		read("or");
		Bt();
		build_tree("or",2);
	}
}

void Bt () {
	Bs ();
	while (next_token("&") == "&"){
		read("&");
		Bs();
		build_tree("&",2);
	}
}

void Bs () {

	if (next_token("not") == "not") {
		read("not");
		Bp();
		build_tree("not",1);
	} else{
		Bp();
	}
}

void Bp () {

	A();

		if (next_token("gr") == "gr") {
			read("gr");
			A();
			build_tree("gr",2);
		}
		else if (next_token(">") == ">") {
			read(">");
			A();
			build_tree(">",2);
		}
		else if (next_token("ge") == "ge") {
			read("ge");
			A();
			build_tree("ge",2);
		}
		else if (next_token(">=") == ">=") {
			read(">=");
			A();
			build_tree(">=",2);
		}
		else if (next_token("ls") == "ls") {
			read("ls");
			A();
			build_tree("ls",2);
		}
		else if (next_token("<") == "<") {
			read("<");
			A();
			build_tree("<",2);
		}
		else if (next_token("le") == "le") {
			read("le");
			A();
			build_tree("le",2);
		}
		else if (next_token("<=") == "<=") {
			read("<=");
			A();
			build_tree("<=",2);
		}
		else if (next_token("eq") == "eq") {
			read("eq");
			A();
			build_tree("eq",2);
		}
		else if (next_token("ne") == "ne") {
			read("ne");
			A();
			build_tree("ne",2);
		}
}

void A () {

	if (next_token("+") == "+"){
		read("+");
		At();
	}
	else if (next_token("-") == "-"){
		read("-");
		At();
		build_tree("neg",1);
	}
	else {
		At();
	}

	while ((next_token("+") == "+") || next_token("-") == "-") {

		if(next_token("+") == "+"){
			read("+");
			At();
			build_tree("+",2);
		}
		else {
			read("-");
			At();
			build_tree("-",2);
		}
	}
}

void At () {

	Af ();

	while (next_token("*") == "*" || next_token("/") == "/") {

		if (next_token("*") == "*") {
			read("*");
			Af();
			build_tree("*", 2);
		}
		else if (next_token("/") == "/") {
			read("/");
			Af();
			build_tree("/",2);
		}
	}
}

void Af () {

	Ap();
	if (next_token("**") == "**"){
		read("**");
		Af();
		build_tree("**", 2);
	}
}

void Ap () {

	R();

	while (next_token("@") == "@"){
		read("@");

		read_identifier();

		R ();
		build_tree("@",3);
	}
}

void R () {

	Rn ();
	
	while ((next_token(_IDENTIFIER) != "") 
		|| (next_token(_DIGIT) != "") 
		|| (next_token(_STRING) != "")
		|| (next_token("true") == "true")
		|| (next_token("false") == "false")
		|| (next_token("nil") == "nil")
		|| (next_token("dummy") == "dummy")
		|| (next_token("(") == "(")) {
		
		Rn();
		build_tree("gamma",2);
	}
}

void Rn () {

	if (next_token(_IDENTIFIER) != "") {
		read_identifier();
	}
	else if (next_token(_DIGIT) != "") {
		read_digit();
	}
	else if (next_token(_STRING) != ""){
		read_string();
	}
	else if (next_token("true") == "true") {
		read("true");
		build_tree("<true>", 0);
	}
	else if (next_token("false") == "false") {
		read("false");
		build_tree("<false>", 0);
	}
	else if (next_token("nil") == "nil") {
		read("nil");
		build_tree("<nil>", 0);
	}
	else if (next_token("dummy") == "dummy") {
		read("dummy");
		build_tree("<dummy>", 0);
	}
	else if (next_token("(") == "(") {
		read("(");
		E ();
		read(")");
	}
	//should never reach this else because of the select sets
	else if (in_stream.peek() != EOF) {
		cout << "ERROR : expecting Rn \n";
	}
}

void D () {
	
	Da();
	if (next_token("within") == "within"){
		read("within");
		D();
		build_tree("within",2);
	}
}

void Da () {

	Dr ();
	int n=1;
	if(next_token("and") == "and"){
		while (next_token("and") == "and"){
			read("and");
			n++;
			Dr ();
		}
		build_tree("and",n);
	}
}

void Dr () {
	if (next_token("rec") == "rec") {
		read("rec");
		Db();
		build_tree("rec",1);
	} else {
		Db();
	}
}

void Db () {

	if (next_token("(") == "("){
		read("(");
		D();
		read(")");
	}
	else {
		//need to store identifier to be able to revert back later in case the next token is a comma
		string id = next_token(_IDENTIFIER);
		read_identifier();
		if ((next_token(_IDENTIFIER) != "") || (next_token("(") == "(")) {
			int n=0;
			while ((next_token(_IDENTIFIER) != "") || (next_token("(") == "(")){
				Vb();
				n++;
			}
			read("=");
			E();
			build_tree("function_form", n + 2);
		}
		else {
			if (next_token(",") == ","){
				my_putback(id);
				Vl();
			}
			read("=");
			E();
			build_tree("=", 2);
		}
	}
}

void Vb () {

	if (next_token(_IDENTIFIER) != "") {
		read_identifier();
	}
	else if (next_token("(") != "") {
		read ("(");
		
		if (next_token(")") != "") {
			read (")");
			build_tree("()",0);
			//TODO: build_tree
		}
		else {
			Vl();
			read (")");
		}
	}
	else {
		cout << "ERROR : not a valid variable definition (Vb)\n";
	}
}

void Vl () {

	read_identifier();

	int n=1;

	if (next_token(",") == ","){
		while (next_token(",") == "," ){
			read(",");
			read_identifier();
			n++;
		}
		build_tree(",", n);
	}
}

void build_tree(string name, int children){
    
    Node* n = new Node;
    n->name = name;
    n->children = children;
    n->child = new Node*[children];
    
    for(int i = children-1; i >= 0; i--)
    {
        Node* temp = s.top();
        s.pop();
        n->child[i] = temp;
    }

    s.push(n);
}

void display_tree(Node *node, int level){
    
    if(node == NULL) return;

    //print the dots
    for(int i = 1; i <= level; i++)
        cout<<".";
    
    cout << node->name << " " << endl;

    if(node->child == NULL) return;
    
    for(int i = 0; i < node->children; i++){
        display_tree(node->child[i], level+1);
    }
}

int main (int argc, char** argv){
	bool ast = false;
	bool st = false;
	string file;

	for(int i = 1; i < argc; i++) {
		
		if(string(argv[i]) == "-ast") {
			ast = true;
		}
		else if (string(argv[i]) == "-st") {
			st = true;
		}
		else if(argv[i][0] != '-') {
			file = string(argv[i]);
			in_stream.open(file.c_str());
		}
	}

	E();
	
	// printQueue();
	
	if (ast) {
		display_tree(s.top(), 0);
	}
	
	standardize(s.top());
	
	if (st) {
		display_tree(s.top(), 0);
	}

	run_machine(s.top());

  	in_stream.close();
  }
                                                        
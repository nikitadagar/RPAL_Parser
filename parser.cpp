#include "header.h"
#include "scanner.cpp"

ifstream in_stream;

//procedure for Non terminal E
void E () {

	if (next_token("let") == "let"){
		read("let");
		D();
		read("in");
		E();
		//build tree
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
		//build tree
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
		//build tree
	}
}

void T () {
	Ta();

	while (next_token(",") == ","){
		read(",");
		Ta();
		//build tree here
	}
}

void Ta() {
	Tc();

	while (next_token("aug") == "aug"){
		read("aug");
		Tc();
	}
}

void Tc() {
	B();
	if (next_token("->") == "->" ){
		read("->");
		Tc();
		read("|");
		Tc();
	}
}

void B () {
	Bt ();
	while (next_token("or") == "or"){
		read("or");
		Bt();
	}
}

void Bt () {
	Bs ();
	while (next_token("&") == "&"){
		read("&");
		Bs();
	}
}

void Bs () {

	if (next_token("not") == "not") {
		read("not");
	}
	Bp();
}

void Bp () {

	A();

		if (next_token("gr") == "gr") {
			read("gr");
			A();
		}
		else if (next_token(">") == ">") {
			read(">");
			A();
		}
		else if (next_token("ge") == "ge") {
			read("ge");
			A();
		}
		else if (next_token(">=") == ">=") {
			read(">=");
			A();
		}
		else if (next_token("ls") == "ls") {
			read("ls");
			A();
		}
		else if (next_token("<") == "<") {
			read("<");
			A();
		}
		else if (next_token("le") == "le") {
			read("le");
			A();
		}
		else if (next_token("<=") == "<=") {
			read("<=");
			A();
		}
		else if (next_token("eq") == "eq") {
			read("eq");
			A();
		}
		else if (next_token("ne") == "ne") {
			read("ne");
			A();
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
		//build tree
	}
	else {
		At();
	}

	while ((next_token("+") == "+") || next_token("-") == "-") {

		if(next_token("+") == "+"){
			read("+");
			At();
			//build tree
		}
		else {
			read("-");
			At();
			//built tree
		}
	}

}

void At () {

	Af ();

	if (next_token("*") == "*") {
		read("*");
		At();
	}
	else if (next_token("/") == "/") {
		read("/");
		At();
	}
}

void Af () {

	Ap();
	if (next_token("**") == "**"){
		read("**");
		Af();
	}
}

void Ap () {

	R();

	while (next_token("@") == "@"){
		read("@");

		read_identifier();

		R ();
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
	}
	else if (next_token("false") == "false") {
		read("false");
	}
	else if (next_token("nil") == "nil") {
		read("nil");
	}
	else if (next_token("dummy") == "dummy") {
		read("dummy");
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
	}
}

void Da () {

	Dr ();

	while (next_token("and") == "and"){
		read("and");
		Dr ();
	}
}

void Dr () {
	if (next_token("rec") == "rec") {
		read("rec");
	}
	Db();
}

void Db () {

	if (next_token("(") == "("){
		read("(");
		D();
		read(")");
	}
	else {
		read_identifier();
		if ((next_token(_IDENTIFIER) != "") || (next_token("(") == "(")){
			while ((next_token(_IDENTIFIER) != "") || (next_token("(") == "(")){
				Vb();
			}
			read("=");
			E();
		}
		else {
			if (next_token(",") == ","){
				Vl();
			}
			read("=");
			E();
		}
	}
}

void Vb () {

	if (next_token(_IDENTIFIER) != "") {
		read (next_token(_IDENTIFIER));
	}
	else if (next_token("(") != "") {
		read ("(");
		
		if (next_token(")") != "") {
			read (")");
			//TODO: build_tree
		}
		else {
			Vl();
			if (next_token(")") == "") {
				cout << "ERROR : ) expected in Variable list\n";
			}
			else {
				read (")");
			}
		}
	}
	else {
		cout << "ERROR : not a valid variable definition (Vb)\n";
	}
}

void Vl () {

	read_identifier();

	if (next_token(",") == ","){
		while (next_token(",") == "," ){
			read(",");
			read_identifier();
		}
	}
	else {
		cout << "ERROR : expecting , in variable list\n";
	}
}


int main (int argc, char** argv){

	string file = argv[1];
	helper(file);
    in_stream.open(file.c_str());

  	E();
    // cout << is_string();

  	in_stream.close();
  }
  
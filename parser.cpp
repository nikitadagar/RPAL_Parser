#include "header.h"
#include "scanner.cpp"

ifstream in_stream;

//procedure for Non terminal E
void E () {

// 	if (is_next_token("let")) {
// 		cout << "pYES";
// 	}
// 	else if (is_next_token("fn")) {

// 	}
// 	else {
// 		Ew ();
// 	}
	
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

	// if (next_token("+") == "+") {
	// 	read("+");
	// }
	// else if (next_token("-") == "-") {
	// 	read("-");
	// }
	// At();
	
	// while ()

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
	read("**");
	Af();

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
	while(in_stream.peek() == ' '){
		in_stream.get();
		Rn();
	}
}

void Rn () {

	if (next_token("IDENTIFIER") != "") {
		read(next_token("IDENTIFIER"));
	}
	else if (next_token("INTEGER") != "") {
		read(next_token("INTEGER"));
	}
	else if (next_token("STRING") != ""){
		read (next_token("STRING"));
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
	else if (next_token("(") != "") {
		read("(");
		E ();
		if (next_token(")") == ")") {
			read(")");
		}
		else {
			cout << "ERROR : Expecting )";
		}
	}
	else {
		cout << "ERROR in Rators and Rands";
	}
}

void D () {
	
	Da();
	read("within");
	D();
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

	if (next_token("(") != ""){
		read ("(");

		D ();

		read(")");
	}
	else if (next_token("IDENTIFIER") != ""){
		read (next_token("IDENTIFIER"));

		Vb();

		while (next_token("=") != "=") {
			Vb();
		}
		read("=");
		E();
	}
	else {
		Vl();
		read("=");
		E();
	}
}

void Vb () {

	if (next_token("IDENTIFIER") != "") {
		read (next_token("IDENTIFIER"));
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

	while (next_token(",") == "," ){
		read(",");
		read_identifier();
	}
}


int main (int argc, char** argv){

	string file = argv[1];
    in_stream.open(file.c_str());

  	// if (is_next_token("let") == true){
  	// 	cout << " YES " << "\n";
  	// } else {
  	// 	cout << " NO " << "\n";
  	// }

  	Vb();

  	in_stream.close();
  }
  
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



// void Ap () {

// 	R();

// 	while (next_token("@") == "@"){
// 		read("@");
		
// 		if (next_token("IDENTIFIER") != ""){
// 			read()
// 		}
// 	}
// }


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

	if (next_token("within") == "within"){
		read("within");
		D();
	}
	else if (next_token(";") == ";"){
		read (";");
	}
	else {
		cout << "ERROR : Expecting ;";
	}
}

void Da () {

	Dr ();

	while (next_token("and") == "and"){
		read("and");
		Dr ();
	}
	if (next_token(";") == ";") {
		read(";");
	}
	else {
		cout << "ERROR : Expecting ; in definition";
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

		if (next_token(")") == ")"){ 	//if next token is )
			read (")");

			if (next_token(";") == ";"){ 	//if next token is ;
				read(";");
			}
			else {
				cout << "ERROR : Expecting ; in definition\n";
			}
		}
		else {
			cout << "ERROR : Expecting ) in definition\n";
		}
	}
	else if (next_token("IDENTIFIER") != ""){
		read (next_token("IDENTIFIER"));

		Vb();

		while (next_token("=") != "=") {
			Vb();
		}
		if (next_token("=") == "="){
			read ("=");
			E ();
		}
		else {
			cout << "ERROR : Expecting = but not found in definition";
		}
	}
	else {
		Vl();
		 if (next_token("=") == "=") {
		 	read ("=");
		 	E();
		 }
		 else {
		 	cout << "ERROR : Expecting = but not found in definition";
		 }
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
  
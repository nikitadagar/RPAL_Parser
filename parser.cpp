#include "header.h"
#include "scanner.cpp"

ifstream in_stream;

//procedure for Non terminal E
// void E () {

// 	if (is_next_token("let")) {
// 		cout << "pYES";
// 	}
// 	else if (is_next_token("fn")) {

// 	}
// 	else {
// 		Ew ();
// 	}
	
// }

// void Db () {

// 	if (next_token("(") != ""){
// 		read ("(");

// 		D ();

// 		if (next_token(")") != ""){ 	//if next token is )
// 			read (")");

// 			if (next_token(";") != ""){ 	//if next token is ;
// 				read(";");
// 			}
// 			else {
// 				cout << "ERROR : Expecting ; in definition\n";
// 			}
// 		}
// 		else {
// 			cout << "ERROR : Expecting ) in definition\n";
// 		}
// 	}
// 	else if (next_token("IDENTIFIER")){
// 		read (next_token("IDENTIFIER"));

// 		Vb();

// 		while (next_token("=") == "" || in_stream.peek() )
// 	}


// }

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
	
	string nt;
	nt = next_token("IDENTIFIER");

	if (nt != "") {
		read (nt);
		
		nt = next_token(",");

		while (nt == ",") { 	//if the token returned is the one we want

			read (nt);

			if (next_token("IDENTIFIER") == "") {
				cout << "ERROR : Identifier expected after comma in variable list\n";
				break;
			}
			else {
				read (next_token("IDENTIFIER"));
			}
			nt = next_token(",");
		}	//while
	// build_tree (",", 0);
	}
	else {
		cout << "ERROR : Identifier expected in variable list\n";
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
  
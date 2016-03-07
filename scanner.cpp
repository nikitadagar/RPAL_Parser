#include <iostream>
#include <fstream>
#include <unordered_set>
#include "scanner.h"

using namespace std;

ifstream in_stream;

bool is_next_token (string s) {

	//creating a hashset of all token types

	string const DIGIT = "DIGIT";
	
	string const identifier_list[] = {"let", "in", "fn", "where", "aug", "not", "or", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec", "list"};
	unordered_set<string> identifiers(identifier_list, identifier_list + sizeof(identifier_list) / sizeof(identifier_list[0]));

	string const punctuation_list[] = {"(", ")", ";", ","};
	unordered_set<string> punctuations(punctuation_list, punctuation_list + sizeof(punctuation_list) / sizeof(punctuation_list[0]));


	string const operator_symbol_list[] = {"+", "-", "*", "<", ">", "&", ".", "@", "/", ":", "=", "~", "|", "$", "!", "#", "^", "_", "?"};
	unordered_set<string> operator_symbols(punctuation_list, punctuation_list + sizeof(punctuation_list) / sizeof(punctuation_list[0]));

	if (identifiers.count(s)) {
		cout << "Identifier found ";
		return is_identifier(s);
	} 
	else if (punctuations.count(s)) {
		cout << "punctuation found";
		return is_punctuation(s);
	}
	else if (s == DIGIT){
		cout << "DIGIT found";
		return is_digit(s);
	}
	else {
		cout << "Next token not found";
	}

	return false;
}

//checks if the given string is an identifier
bool is_identifier (string s){

	remove_spaces();

	string token;
	char c;

	if (isalpha(in_stream.peek())) {
		
		while (isalpha(in_stream.peek()) || isdigit(in_stream.peek()) || (in_stream.peek()) == '_'){
			token += in_stream.get();
		}
		if (token == s) {
			return true;
		} else {
			my_putback(token);
		}
	}
	return false;
}

//checks if the next token is the given punctuation 
bool is_punctuation (string s) {
	
	remove_spaces();
	
	if (in_stream.peek() == s[0]) {
		return true;
	} else {
		return false;
	}
}

//checks if the next token is an integer
bool is_digit (string s) {

	remove_spaces();

	if (isdigit(in_stream.peek())) {
		while (isdigit(in_stream.peek())) {
			in_stream.get();
		}
		return true;
	} else {
		return false;
	}
}

int main (int argc, char** argv){

	string file = argv[1];
    in_stream.open(file.c_str());

  	// cout << in_stream.get();
  	
  	// char c = in_stream.get();
  	// cout << c << "\n";

  	if (is_next_token("DIGIT") == true){
  		cout << "yes" << "\n";
  	} else {
  		cout << "no" << "\n";
  	}

  	// if (is_next_token("(") == true){
  	// 	cout << "yes" << "\n";
  	// } else {
  	// 	cout << "no" << "\n";
  	// }

  	in_stream.close();
  	return 0;
}

// 	--------- HELPER FUNCTIONS ----------

void remove_spaces () {
	while (isspace(in_stream.peek())){
		in_stream.get();
	}
}

//adds the string back to the beginning of the file
void my_putback (string s) {
    int size = s.size();
    const char* ch = s.c_str();
    for(int i = size-1; i >= 0; i--){
        in_stream.putback(ch[i]);
    }
}
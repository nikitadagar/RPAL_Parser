#include <iostream>
#include <fstream>
#include <unordered_set>
#include "scanner.h"

using namespace std;

ifstream in_stream;

string const _DIGIT = "DIGIT";
string const _STRING = "STRING";

//creating a hashset of all token types
	
string const identifier_list[] = {"let", "in", "fn", "where", "aug", "not", "or", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec", "list"};
unordered_set<string> identifiers(identifier_list, identifier_list + sizeof(identifier_list) / sizeof(identifier_list[0]));

string const punctuation_list[] = {"(", ")", ";", ","};
unordered_set<string> punctuations(punctuation_list, punctuation_list + sizeof(punctuation_list) / sizeof(punctuation_list[0]));

string const operator_symbol_list[] = {"+", "-", "*", "<", ">", "&", ".", "@", "/", ":", "=", "~", "|", "$", "!", "#", "^", "_", "?"};
unordered_set<string> operator_symbols(operator_symbol_list, operator_symbol_list + sizeof(operator_symbol_list) / sizeof(operator_symbol_list[0]));


bool is_next_token (string s) {

	remove_comment();
	remove_spaces();

	if (identifiers.count(s)) {
		cout << "Identifier found ";
		return is_identifier(s);
	} 
	else if (punctuations.count(s)) {
		cout << "punctuation found";
		return is_punctuation(s);
	}
	else if (s == _DIGIT){
		cout << "DIGIT found";
		return is_digit();
	}
	else if (s == _STRING){
		cout << "string found";
		return is_string();
	}
	else if (operator_symbols.count(string(1,s[0]))) {
		cout << "operator_symbol found";
		return is_operator(s);
	}
	else {
		cout << "Next token not found";
		return false;
	}
}

//checks if the given string is an identifier
bool is_identifier (string s){

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
	
	if (in_stream.peek() == s[0]) {
		return true;
	} else {
		return false;
	}
}

//checks if the next token is an integer
bool is_digit () {

	if (isdigit(in_stream.peek())) {
		while (isdigit(in_stream.peek())) {
			in_stream.get();
		}
		return true;
	} else {
		return false;
	}
}

//checks if the next token is an identifier 
bool is_operator (string s) {

	string c;
	string token;

	c = in_stream.peek();

	while (operator_symbols.count(c)) {
		token += in_stream.get();
		c = in_stream.peek();
	}

	if (token == s) {
		return true;
	}
	else {
		my_putback(token);
		return false; 
	}
}

bool is_string () {

	string token;
	int flag;   	// flag used to check if the quotes were closed before EOF

	if (in_stream.peek() == '\"') {
		
		flag = 1;
		in_stream.get();

		QUOTE: while (in_stream.peek() != '\"') {

			if (in_stream.eof()) {
				return false;
			}

			token += in_stream.get();
		}

		in_stream.get();
		flag = 0;

		if (in_stream.peek() == '\"') {
			in_stream.get();
			flag = 1;
			goto QUOTE;
		}

		if (flag == 0) return true;
		else return false;
	}
	else {
		return false;
	}
}

int main (int argc, char** argv){

	string file = argv[1];
    in_stream.open(file.c_str());

  	if (is_next_token("let") == true){
  		cout << " YES " << "\n";
  	} else {
  		cout << " NO " << "\n";
  	}

  	if (is_next_token("STRING") == true){
  		cout << " YES " << "\n";
  	} else {
  		cout << " NO " << "\n";
  	}
  	if (is_next_token("=") == true){
  		cout << " YES " << "\n";
  	} else {
  		cout << " NO " << "\n";
  	}

  	in_stream.close();
  	return 0;
}

// 	--------- HELPER FUNCTIONS ----------

void remove_spaces () {
	while (isspace(in_stream.peek())){
		in_stream.get();
	}
}

//checks if the next token is the beginning of a comment
bool is_comment () {

	remove_spaces();

	string s;

	s = in_stream.get();
	s += in_stream.get();

	if (s == "//") {
		cout << "\n saw //";
		return true;
	}
	else {
		my_putback(s);
		return false;
	}
}

//removes the comment if next token is //
void remove_comment () {

	string s;

	while (is_comment()) {
		getline(in_stream, s);
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
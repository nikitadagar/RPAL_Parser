#include "header.h"

string const _DIGIT = "DIGIT";
string const _STRING = "STRING";
string const _IDENTIFIER = "IDENTIFIER";

//creating a hashset of all token types
	
string const identifier_list[] = {"let", "in", "fn", "where", "aug", "not", "or", "gr", "ge", "ls", "le", "eq", "ne", "true", "false", "nil", "dummy", "within", "and", "rec", "list"};
unordered_set<string> identifiers(identifier_list, identifier_list + sizeof(identifier_list) / sizeof(identifier_list[0]));

string const punctuation_list[] = {"(", ")", ";", ","};
unordered_set<string> punctuations(punctuation_list, punctuation_list + sizeof(punctuation_list) / sizeof(punctuation_list[0]));

string const operator_symbol_list[] = {"+", "-", "*", "<", ">", "&", ".", "@", "/", ":", "=", "~", "|", "$", "!", "#", "^", "_", "?"};
unordered_set<string> operator_symbols(operator_symbol_list, operator_symbol_list + sizeof(operator_symbol_list) / sizeof(operator_symbol_list[0]));


//takes in the token that we're looking for
//returns the next token found
string next_token (string s) {

	remove_comment();
	remove_spaces();

	if (identifiers.count(s)) {
		// cout << "Identifier found \n";
		return is_identifier(s);
	}
	else if (s == _IDENTIFIER) {
		// cout << "Looking for any Identifier \n";
		return is_any_identifier();
	} 
	else if (punctuations.count(s)) {
		// cout << "punctuation found \n";
		return is_punctuation(s);
	}
	else if (s == _DIGIT){
		// cout << "DIGIT found\n";
		return is_digit();
	}
	else if (s == _STRING){
		// cout << "string found\n";
		return is_string();
	}
	else if (operator_symbols.count(string(1,s[0]))) {
		// cout << "operator_symbol found\n";
		return is_operator(s);
	}
	else {
		cout << "Next token not found\n";
		return "";
	}
}

//checks if the given string is the given identifier
string is_identifier (string s){
	// cout << "\nCalled is_identifier asking for " << s;
	string token;
	char c;

	if (isalpha(in_stream.peek())) {
		while (isalpha(in_stream.peek()) || isdigit(in_stream.peek()) || (in_stream.peek()) == '_'){
			token += in_stream.get();
		}
		if (token == s) {	//we found the given identifier
			// TODO: build_tree
			my_putback(token);
			// cout << "\nfound "<< token;
			return token;
		} else {	//if the next token is not the given identifier, we put it back
			my_putback(token);
			return "";
		}
	}
	return "";
}

//checks if the next token is any of the identifiers
//returns the length of the identifier found
string is_any_identifier () {

	string token = "";

	if (isalpha(in_stream.peek())) {
		
		while (isalpha(in_stream.peek()) || isdigit(in_stream.peek()) || (in_stream.peek()) == '_'){
			token += in_stream.get();
		}
		my_putback (token);
		if (identifiers.count(token)){
			return "";
		} else {
			return token;
		}
	}
	else {
		if (token != ""){
			my_putback (token);
		}
		return "";
	}
}

//checks if the next token is the given punctuation 
string is_punctuation (string s) {

	char c = in_stream.peek();
	
	if (c == s[0]) {
		return s;
	} else {
		return "";
	}
}

//checks if the next token is an integer
string is_digit () {
	string token;

	if (isdigit(in_stream.peek())) {

		while (isdigit(in_stream.peek())) {
			token += in_stream.get();
		}
		// TODO: build_tree
		my_putback(token);
		return token;
	} else {
		if (token != ""){
			my_putback (token);
		}
		return "";
	}
}

//checks if the next token is an operator
string is_operator (string s) {

	string c;
	string token;

	c = in_stream.peek();

	while (operator_symbols.count(c)) {
		token += in_stream.get();
		c = in_stream.peek();
	}

	if (token == s) {
		my_putback(token);
		return token;
	}
	else {
		my_putback(token);
		return ""; 
	}
}

//checks if the next token is any string
string is_string () {

	string token;
	int flag;   	// flag used to check if the quotes were closed before EOF

	if (in_stream.peek() == '\"' || in_stream.peek() == '\'') {

		flag = 1;
		char quote_type = in_stream.peek();
		token += in_stream.get();

		QUOTE: while (in_stream.peek() != quote_type) {
			if (in_stream.eof()) {
				return "";
			}

			token += in_stream.get();
		}

		token += in_stream.get();
		flag = 0;

		if (in_stream.peek() == '\"' || in_stream.peek() == '\'') {
			token += in_stream.get();
			flag = 1;
			goto QUOTE;
		}

		if (flag == 0) {
			// TODO: build_tree
			// cout << "\nString  : " << token;
			my_putback(token);
			return token;
		}
		else return "";
	}
	else {
		return "";
	}
}

void read_identifier () {

	remove_comment();
	remove_spaces();

	string s = is_any_identifier();

	if (s == "") {
		cout << "ERROR : Expecting an Identifier\n";
		poison_pill();
	}
	else {
		build_tree("<ID:" + s + ">", 0);
		read(s);
	}
}

void read_string () {

	remove_comment();
	remove_spaces();

	string s = is_string();

	if (s == "") {
		cout << "ERROR : Expecting an string\n";
	}
	else {
		// cout << "\nString received : " << s;
		build_tree("<STR:" + s + ">", 0);
		read(s);
	}
}

void read_digit () {

	remove_comment();
	remove_spaces();

	string s = is_digit();

	if (s == "") {
		cout << "ERROR : Expecting an integer\n";
	}
	else {
		build_tree("<INT:" + s + ">", 0);
		read(s);
	}
}

//read function moves the instream pointer ahead to consume the given token
void read (string s) {

	string token;

	remove_comment();
	remove_spaces();

	int len = s.length();

	for (int i=0; i<len; i++) {
		token += in_stream.get();
	}

	if (token != s) {
		// cout << "READ ERROR : Expected " << s << " but found " << token << "\n";
	}
	else {
		// cout << "Reading " << s << "\n";
	}
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

	string s = "";

	if (in_stream.peek() == '/') {
		s += in_stream.get();
	}

	if (in_stream.peek() == '/') {
		s += in_stream.get();
	}

	if (s == "//") {
		return true;
	}
	else if(s != "") {
		my_putback(s);
		return false;
	}
	return false;
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
	// cout <<"putback : "<< s << "\n";
    int size = s.length();
    const char* ch = s.c_str();
    
    //for older versions of C++
    if (in_stream.fail()) {
    	in_stream.clear();
    }

    for(int i = size-1; i >= 0; i--){
        in_stream.putback(ch[i]);
    }
}

ofstream out_stream;

void helper (string filename) {
	out_stream.open(filename, ios::app);
	out_stream << " ";
	out_stream.close();
}

void poison_pill (){
	in_stream.close();
	exit(0);
}
#ifndef HEADER_H
#define HEADER_H

#include <iostream>
#include <fstream>
#include <unordered_set>

using namespace std;

extern ifstream in_stream;

string next_token(std::string);

string is_identifier(std::string);
string is_any_identifier();
string is_punctuation(std::string);
string is_digit();
string is_operator(std::string);
string is_string();

//HELPER FUNCTIONS FOR SCANNER
void remove_spaces();
bool is_comment();
void remove_comment();
void my_putback(std::string);

// PARSER FUNCTIONS FOR NON TERMINALS


void D();
void Da();
void Dr();
void Db();
void Vl();
void Vb();

#endif
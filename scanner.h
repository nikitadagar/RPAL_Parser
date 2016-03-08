

bool is_next_token(std::string);

bool is_identifier(std::string);
bool is_punctuation(std::string);
bool is_digit();
bool is_operator(std::string);
bool is_string();

//helper functions
void remove_spaces();
bool is_comment();
void remove_comment();
void my_putback(std::string);
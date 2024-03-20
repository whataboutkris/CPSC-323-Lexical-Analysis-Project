#include <iostream>
#include <string>
#include <vector>

struct Token {
  std::string type;
  std::string lexeme;
};

std::vector<Token> lexer(const std::string &input_string);
#include "main.h"
#include <fstream>
#include <iostream>
#include <regex>
#include <string>
#include <vector>

std::vector<Token> lexer(const std::string &input_string) {
  // Define token patterns using regular expressions
  std::vector<std::pair<std::string, std::string>> token_patterns = {
      {"REAL",
       "\\d+(\\.\\d+)?"}, // Pattern for numbers (including floating-point)
      {"OPERATOR", "[\\+\\-\\*\\/\\>\\<\\=]"},
      {"SEPARATOR", "\\("},
      {"SEPARATOR", "\\)"},
      {"SEPARATOR", ";"},
      {"IDENTIFIER", "[a-zA-Z_][a-zA-Z0-9_]*"}, // Pattern for identifiers
      {"KEYWORD",
       "\\b(if|else|while|for|return|int|float|char|void)\\b"} // Pattern for
                                                               // keywords
  };

  std::vector<Token> tokens;
  // Combine all token patterns into a single regular expression
  std::string pattern_string;
  for (const auto &pattern : token_patterns) {
    if (!pattern_string.empty()) {
      pattern_string += "|"; // Add alternation if it's not the first pattern
    }
    pattern_string += "(" + pattern.second + ")";
  }

  // Compile the combined regular expression
  std::regex regex(pattern_string);

  // Tokenize the input string using the regular expression
  std::sregex_iterator iter(input_string.begin(), input_string.end(), regex);
  std::sregex_iterator end;
  while (iter != end) {
    for (size_t i = 0; i < iter->size(); ++i) {
      if (!iter->str(i).empty()) {
        std::string matched = iter->str(i);
        // Check which pattern matched
        for (const auto &pattern : token_patterns) {
          if (std::regex_match(matched, std::regex(pattern.second))) {
            if (pattern.first == "IDENTIFIER") {
              // Checking here if it's a keyword otherwise it would be labelled
              // as identifier
              if (std::regex_match(matched,
                                   std::regex(token_patterns.back().second))) {
                tokens.push_back({"KEYWORD", matched});
              } else {
                tokens.push_back({pattern.first, matched});
              }
            } else {
              tokens.push_back({pattern.first, matched});
            }
            break;
          }
        }
        break; // Exit the loop after adding the token
      }
    }
    ++iter;
  }

  return tokens;
}

int main() {

  std::ifstream inputFile("input_scode.txt");
  if (!inputFile.is_open()) {
    std::cerr << "Failed to open the input file." << std::endl;
    return 1;
  }

  std::string input_string;
  std::string line;
  while (std::getline(inputFile, line)) {
    input_string += line;
  }
  inputFile.close();

  std::vector<Token> tokens = lexer(input_string);
  std::ofstream outputFile("output.txt");
  if (!outputFile.is_open()) {
    std::cerr << "Failed to open the output file." << std::endl;
    return 1;
  }

  outputFile << "[Token  ,  Lexeme]" << std::endl; // formatting

  for (const auto &token : tokens) {
    outputFile << "[" << token.type << ", " << token.lexeme << "]" << std::endl;
  }

  outputFile.close();

  return 0;
}
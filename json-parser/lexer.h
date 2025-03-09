#pragma once
#include <fstream>
#include <string>
#include <vector>

#include "token.h"

class Lexer {
   public:
    Lexer(const std::string& filePath);
    std::vector<Token> tokenize();

   private:
    std::ifstream file;
    int line;
    int column;
    char advance();
    void throwError(const std::string& message);

    Token tokenizeString();
    char handleEscape();

    Token tokenizeDigit(char& c);
    Token tokenizeTrue();
    Token tokenizeFalse();
    Token tokenizeNull();
};

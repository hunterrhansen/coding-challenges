#ifndef LEXER_H
#define LEXER_H

#include <string>
#include <vector>
#include "token.h"

class Lexer
{
public:
    Lexer(const std::string &input);
    std::vector<Token> tokenize();

private:
    std::string input;
    size_t pos;
};

#endif // !LEXER_H

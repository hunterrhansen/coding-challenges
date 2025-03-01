#include "lexer.h"
#include "token.h"
#include <iostream>
#include <string>
#include <vector>

Lexer::Lexer(const std::string &input)
{
    this->input = input;
    this->pos = 0;
}

std::vector<Token> Lexer::tokenize()
{
    std::cout << this->input << std::endl;
    return std::vector<Token>();
}
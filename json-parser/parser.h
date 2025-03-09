#pragma once
#include <vector>

#include "token.h"

class Parser {
   public:
    Parser(std::vector<Token> tokens);
    bool parse();

   private:
    std::vector<Token> tokens;
    size_t current;

    void parseValue();
    void parseObject();
    void parseArray();
    Token peek();
    void consume(TokenType type);
};
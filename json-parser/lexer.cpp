#include "lexer.h"
#include "token.h"
#include <iostream>
#include <string>
#include <vector>
#include <stack>

Lexer::Lexer(const std::string &filePath)
{
    file.open(filePath);
    line = 1;
    column = 1;
}

std::vector<Token> Lexer::tokenize()
{
    char firstChar = file.peek();
    if (firstChar == EOF)
    {
        throwError("Invalid JSON: empty input");
    }

    std::vector<Token> tokens;
    char c;

    while (file.get(c))
    {
        // Handle single-character tokens
        if (c == '{')
            tokens.push_back(Token(TokenType::LEFT_BRACE, "{"));
        else if (c == '}')
            tokens.push_back(Token(TokenType::RIGHT_BRACE, "}"));
        else if (c == '[')
            tokens.push_back(Token(TokenType::LEFT_BRACKET, "["));
        else if (c == ']')
            tokens.push_back(Token(TokenType::RIGHT_BRACKET, "]"));
        else if (c == ':')
            tokens.push_back(Token(TokenType::COLON, ":"));
        else if (c == ',')
            tokens.push_back(Token(TokenType::COMMA, ","));
        // Handle strings
        else if (c == '"')
        {
            tokens.push_back(tokenizeString());
        }
        // Handle numbers
        else if (isdigit(c) || c == '-')
        {
            // TODO: Implement number parsing
        }
        // Handle true/false/null
        else if (c == 't' || c == 'f' || c == 'n')
        {
            // TODO: Implement literal parsing
        }
        // Skip whitespace
        else if (isspace(c))
        {
            continue;
        }
        // Invalid character
        else
        {
            throwError("Invalid character: " + std::string(1, c));
        }
    }

    return tokens;
}

Token Lexer::tokenizeString()
{
    char c;
    std::string str;
    while (file.get(c))
    {
        if (c == '"')
        {
            return Token(TokenType::STRING, str);
        }
        str += c;
    }
    if (file.eof())
    {
        throwError("Unterminated string - missing closing quote");
    }
}

char Lexer::advance()
{
    char c;
    file.get(c);

    if (c == '\n')
    {
        line++;
        column = 1;
    }
    else
    {
        column++;
    }

    return c;
}

void Lexer::throwError(const std::string &message)
{
    throw std::runtime_error(
        "Error at line " + std::to_string(line) +
        ", column " + std::to_string(column) +
        ": " + message);
}
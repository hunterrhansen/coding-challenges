#pragma once

enum class TokenType {
    // Literals
    STRING,
    NUMBER,

    // Keywords
    TRUE,
    FALSE,
    NULL_TOKEN,

    // Structural
    LEFT_BRACE,     // {
    RIGHT_BRACE,    // }
    LEFT_BRACKET,   // [
    RIGHT_BRACKET,  // ]
    COMMA,          // ,
    COLON,          // :
};

struct Token {
    TokenType type;
    std::string lexeme;

    Token(TokenType t, std::string l) : type(t), lexeme(std::move(l)) {}
};
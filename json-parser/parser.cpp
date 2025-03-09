#include "parser.h"

#include <iostream>
#include <string>
#include <vector>

#include "token.h"

Parser::Parser(std::vector<Token> tokens) {
    this->tokens = tokens;
    current = 0;
}

bool Parser::parse() {
    if (tokens.empty()) {
        return false;
    }

    try {
        parseValue();
        return current == tokens.size();  // Ensure we consumed all tokens
    } catch (const std::runtime_error& e) {
        return false;
    }
}

void Parser::parseValue() {
    if (current >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }

    Token token = tokens[current];

    switch (token.type) {
        case TokenType::LEFT_BRACE:
            parseObject();
            break;
        case TokenType::LEFT_BRACKET:
            parseArray();
            break;
        case TokenType::STRING:
        case TokenType::NUMBER:
        case TokenType::TRUE:
        case TokenType::FALSE:
        case TokenType::NULL_TOKEN:
            current++;  // Consume the token
            break;
        default:
            throw std::runtime_error("Unexpected token");
    }
}

void Parser::parseObject() {
    consume(TokenType::LEFT_BRACE);

    if (peek().type == TokenType::RIGHT_BRACE) {
        current++;  // Empty object
        return;
    }

    while (true) {
        // Parse key (must be string)
        if (peek().type != TokenType::STRING) {
            throw std::runtime_error("Expected string key in object");
        }
        current++;  // Consume key

        // Parse colon
        consume(TokenType::COLON);

        // Parse value
        parseValue();

        // Check if we're done or need to parse more key-value pairs
        if (peek().type == TokenType::RIGHT_BRACE) {
            current++;
            break;
        }

        consume(TokenType::COMMA);
    }
}

void Parser::parseArray() {
    consume(TokenType::LEFT_BRACKET);

    if (peek().type == TokenType::RIGHT_BRACKET) {
        current++;  // Empty array
        return;
    }

    while (true) {
        parseValue();

        if (peek().type == TokenType::RIGHT_BRACKET) {
            current++;
            break;
        }

        consume(TokenType::COMMA);
    }
}

Token Parser::peek() {
    if (current >= tokens.size()) {
        throw std::runtime_error("Unexpected end of input");
    }
    return tokens[current];
}

void Parser::consume(TokenType type) {
    if (peek().type != type) {
        throw std::runtime_error("Expected different token type");
    }
    current++;
}
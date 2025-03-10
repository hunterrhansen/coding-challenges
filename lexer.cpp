#include "lexer.h"

#include <iostream>
#include <stack>
#include <string>
#include <vector>

#include "token.h"

Lexer::Lexer(const std::string &filePath) {
    file.open(filePath);
    line = 1;
    column = 1;
}

std::vector<Token> Lexer::tokenize() {
    char firstChar = file.peek();
    if (firstChar == EOF) {
        throwError("Invalid JSON: empty input");
    }

    std::vector<Token> tokens;
    char c;

    while (file.get(c)) {
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
        else if (c == '"') {
            tokens.push_back(tokenizeString());
        }
        // Handle numbers
        else if (isdigit(c) || c == '-') {
            tokens.push_back(tokenizeDigit(c));
        }
        // Handle true/false/null
        else if (c == 't') {
            tokens.push_back(tokenizeTrue());
        } else if (c == 'f') {
            tokens.push_back(tokenizeFalse());
        } else if (c == 'n') {
            tokens.push_back(tokenizeNull());
        }
        // Skip whitespace
        else if (isspace(c)) {
            continue;
        }
        // Invalid character
        else {
            throwError("Invalid character: " + std::string(1, c));
        }
    }

    return tokens;
}

Token Lexer::tokenizeString() {
    char c;
    std::string str;
    while (file.get(c)) {
        if (c == '\\') {
            char escape = handleEscape();
            str += escape;
            continue;
        }
        if (c == '"') {
            return Token(TokenType::STRING, str);
        }
        str += c;
    }
    if (file.eof()) {
        throwError("Unterminated string - missing closing quote");
    }

    // Add a return here to satisfy compiler, though throwError will end
    // execution
    return Token(TokenType::STRING, "");
}

char Lexer::handleEscape() {
    char c;
    file.get(c);

    switch (c) {
        case '\\':
            return '\\';
        case '\"':
            return '\"';
        case '/':
            return '/';
        case 'b':
            return '\b';
        case 'f':
            return '\f';
        case 'n':
            return '\n';
        case 'r':
            return '\r';
        case 't':
            return '\t';
        case 'u':
            // Handle Unicode sequences (this needs additional implementation)
            throwError("Unicode sequences not yet implemented");
        default:
            throwError("Invalid escape sequence: \\" + std::string(1, c));
    }
    // Add a return here to satisfy compiler, though throwError will end
    // execution
    return '\0';
}

enum class NumberState {
    INTEGER,   // Parsing the integer part (before decimal)
    FRACTION,  // Parsing decimal places
    EXPONENT,  // Parsing the exponent
};

Token Lexer::tokenizeDigit(char &c) {
    std::string number;

    NumberState state = NumberState::INTEGER;

    number += c;

    while (file.peek() != EOF) {
        c = file.peek();

        switch (state) {
            case NumberState::INTEGER:
                if (c == '.') {
                    // Move to fraction state
                    state = NumberState::FRACTION;
                    number += file.get();  // Consume the '.'
                } else if (c == 'e' || c == 'E') {
                    // Move to exponent state
                    state = NumberState::EXPONENT;
                    number += file.get();  // Consume the 'e' or 'E'
                } else if (isdigit(c)) {
                    // Stay in integer state
                    number += file.get();  // Consume the digit
                } else {
                    // End of number reached
                    return Token(TokenType::NUMBER, number);
                }
                break;

            case NumberState::FRACTION:
                if (!isdigit(c)) {  // First check after decimal point
                    throwError(
                        "invalid number - expected digit after decimal point");
                }
                number += file.get();  // Consume first digit after decimal

                while (file.peek() != EOF) {
                    c = file.peek();
                    if (c == '.') {
                        throwError("invalid number - multiple decimal points");
                    } else if (c == 'e' || c == 'E') {
                        state = NumberState::EXPONENT;
                        number += file.get();
                        break;
                    } else if (isdigit(c)) {
                        number += file.get();
                    } else {
                        return Token(TokenType::NUMBER, number);
                    }
                }
                break;

            case NumberState::EXPONENT:
                // First character after 'e'/'E' can be +/- or must be digit
                c = file.peek();
                if (c == '+' || c == '-') {
                    number += file.get();  // Consume the sign
                    c = file.peek();
                }

                // Must have at least one digit after e/E (+/-)
                if (!isdigit(c)) {
                    throwError("invalid number - expected digit in exponent");
                }

                // Consume first digit
                number += file.get();

                // Continue consuming digits until non-digit
                while (file.peek() != EOF) {
                    c = file.peek();
                    if (isdigit(c)) {
                        number += file.get();
                    } else {
                        return Token(TokenType::NUMBER, number);
                    }
                }
                break;
        }
    }

    // After the switch statement, handle EOF
    return Token(TokenType::NUMBER, number);
}

Token Lexer::tokenizeTrue() {
    char c;
    std::string trueStr = "rue";  // We already consumed 't'

    for (char expected : trueStr) {
        if (file.eof()) {
            throwError("Unexpected EOF while parsing 'true'");
        }

        file.get(c);
        if (c != expected) {
            throwError("Invalid literal: expected 'true'");
        }
    }

    // Peek next character to ensure it's a valid delimiter
    c = file.peek();
    if (!isspace(c) && c != ',' && c != '}' && c != ']' && c != EOF) {
        throwError("Invalid character after 'true' literal");
    }

    return Token(TokenType::TRUE, "true");
}

Token Lexer::tokenizeFalse() {
    char c;
    std::string falseStr = "alse";  // We already consumed 'f'

    for (char expected : falseStr) {
        if (file.eof()) {
            throwError("Unexpected EOF while parsing 'false'");
        }

        file.get(c);
        if (c != expected) {
            throwError("Invalid literal: expected 'false'");
        }
    }

    // Peek next character to ensure it's a valid delimiter
    c = file.peek();
    if (!isspace(c) && c != ',' && c != '}' && c != ']' && c != EOF) {
        throwError("Invalid character after 'false' literal");
    }

    return Token(TokenType::FALSE, "false");
}

Token Lexer::tokenizeNull() {
    char c;
    std::string nullStr = "ull";  // We already consumed 'n'

    for (char expected : nullStr) {
        if (file.eof()) {
            throwError("Unexpected EOF while parsing 'null'");
        }

        file.get(c);
        if (c != expected) {
            throwError("Invalid literal: expected 'null'");
        }
    }

    // Peek next character to ensure it's a valid delimiter
    c = file.peek();
    if (!isspace(c) && c != ',' && c != '}' && c != ']' && c != EOF) {
        throwError("Invalid character after 'null' literal");
    }

    return Token(TokenType::NULL_TOKEN, "null");
}

char Lexer::advance() {
    char c;
    file.get(c);

    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }

    return c;
}

void Lexer::throwError(const std::string &message) {
    throw std::runtime_error("Error at line " + std::to_string(line) +
                             ", column " + std::to_string(column) + ": " +
                             message);
}
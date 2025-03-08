#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "lexer.h"

std::string getTestFilePath(const std::string& filename) {
    return "tests/temp/" + filename;
}

void test_string_tokenization() {
    // Test case 1: Basic string
    {
        std::ofstream testFile(getTestFilePath("test1.json"));
        testFile << R"("Hello, World!")";
        testFile.close();

        Lexer lexer(getTestFilePath("test1.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::STRING);
        assert(tokens[0].lexeme == "Hello, World!");
    }

    // Test case 2: Empty string
    {
        std::ofstream testFile(getTestFilePath("test2.json"));
        testFile << R"("")";
        testFile.close();

        Lexer lexer(getTestFilePath("test2.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::STRING);
        assert(tokens[0].lexeme == "");
    }

    // Test case 3: String with escaped quotes
    {
        std::ofstream testFile(getTestFilePath("test3.json"));
        testFile << R"("Hello \"World\"")";
        testFile.close();

        Lexer lexer(getTestFilePath("test3.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::STRING);
        assert(tokens[0].lexeme == R"(Hello "World")");
    }

    // Test case 4: Unterminated string (should throw)
    {
        std::ofstream testFile(getTestFilePath("test4.json"));
        testFile << R"("Hello)";  // Missing closing quote
        testFile.close();

        Lexer lexer(getTestFilePath("test4.json"));
        bool caught_exception = false;
        try {
            auto tokens = lexer.tokenize();
        } catch (const std::runtime_error& e) {
            caught_exception = true;
        }
        assert(caught_exception);
    }

    // Test case: All escape sequences
    {
        std::ofstream testFile(getTestFilePath("test_escape.json"));
        testFile << R"("\\\/\"\b\f\n\r\t")";
        testFile.close();

        Lexer lexer(getTestFilePath("test_escape.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::STRING);
        assert(tokens[0].lexeme == "\\/\"\b\f\n\r\t");
    }

    // Test case: Invalid escape sequence
    {
        std::ofstream testFile(getTestFilePath("test_invalid_escape.json"));
        testFile << R"("Hello\a")";  // \a is not valid in JSON
        testFile.close();

        Lexer lexer(getTestFilePath("test_invalid_escape.json"));
        bool caught_exception = false;
        try {
            auto tokens = lexer.tokenize();
        } catch (const std::runtime_error& e) {
            caught_exception = true;
            // Optionally verify error message
            assert(std::string(e.what()).find("Invalid escape sequence") !=
                   std::string::npos);
        }
        assert(caught_exception);
    }

    // Test case: Backslash at end of input
    {
        std::ofstream testFile(getTestFilePath("test_backslash_eof.json"));
        testFile << R"("Hello\)";  // Backslash at end
        testFile.close();

        Lexer lexer(getTestFilePath("test_backslash_eof.json"));
        bool caught_exception = false;
        try {
            auto tokens = lexer.tokenize();
        } catch (const std::runtime_error& e) {
            caught_exception = true;
        }
        assert(caught_exception);
    }

    std::cout << "All string tokenization tests passed!" << std::endl;
}

void test_number_tokenization() {
    // Test case 1: Integer
    {
        std::ofstream testFile(getTestFilePath("test_num1.json"));
        testFile << "123";
        testFile.close();

        Lexer lexer(getTestFilePath("test_num1.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "123");
    }

    // Test case 2: Floating point
    {
        std::ofstream testFile(getTestFilePath("test_num2.json"));
        testFile << "123.456";
        testFile.close();

        Lexer lexer(getTestFilePath("test_num2.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "123.456");
    }

    // Test case 3: Negative numbers
    {
        std::ofstream testFile(getTestFilePath("test_num3.json"));
        testFile << "-123.456";
        testFile.close();

        Lexer lexer(getTestFilePath("test_num3.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "-123.456");
    }

    std::cout << "All number tokenization tests passed!" << std::endl;
}

void test_special_tokens() {
    // Test case 1: Basic literals
    {
        std::ofstream testFile(getTestFilePath("test_special1.json"));
        testFile << "true false null";
        testFile.close();

        Lexer lexer(getTestFilePath("test_special1.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 3);
        assert(tokens[0].type == TokenType::TRUE);
        assert(tokens[1].type == TokenType::FALSE);
        assert(tokens[2].type == TokenType::NULL_TOKEN);
    }

    // Test case 2: Invalid true literal
    {
        std::ofstream testFile(getTestFilePath("test_special2.json"));
        testFile << "truex";  // Invalid - extra character
        testFile.close();

        Lexer lexer(getTestFilePath("test_special2.json"));
        bool caught_exception = false;
        try {
            auto tokens = lexer.tokenize();
        } catch (const std::runtime_error& e) {
            caught_exception = true;
            assert(std::string(e.what()).find(
                       "Invalid character after 'true'") != std::string::npos);
        }
        assert(caught_exception);
    }

    // Test case 3: Invalid false literal
    {
        std::ofstream testFile(getTestFilePath("test_special3.json"));
        testFile << "fals";  // Invalid - incomplete
        testFile.close();

        Lexer lexer(getTestFilePath("test_special3.json"));
        bool caught_exception = false;
        try {
            auto tokens = lexer.tokenize();
        } catch (const std::runtime_error& e) {
            caught_exception = true;
            assert(std::string(e.what()).find(
                       "Invalid literal: expected 'false'") !=
                   std::string::npos);
        }
        assert(caught_exception);
    }

    // Test case 4: Invalid null literal
    {
        std::ofstream testFile(getTestFilePath("test_special4.json"));
        testFile << "nul";  // Invalid - incomplete
        testFile.close();

        Lexer lexer(getTestFilePath("test_special4.json"));
        bool caught_exception = false;
        try {
            auto tokens = lexer.tokenize();
        } catch (const std::runtime_error& e) {
            caught_exception = true;
            assert(std::string(e.what()).find(
                       "Invalid literal: expected 'null'") !=
                   std::string::npos);
        }
        assert(caught_exception);
    }

    // Test case 5: Literals in context
    {
        std::ofstream testFile(getTestFilePath("test_special5.json"));
        testFile << R"({"key1": true, "key2": false, "key3": null})";
        testFile.close();

        Lexer lexer(getTestFilePath("test_special5.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 11);
        assert(tokens[3].type == TokenType::TRUE);
        assert(tokens[7].type == TokenType::FALSE);
        assert(tokens[11].type == TokenType::NULL_TOKEN);
    }

    // Test case 6: EOF during literal
    {
        std::ofstream testFile(getTestFilePath("test_special6.json"));
        testFile << "tr";  // Incomplete true
        testFile.close();

        Lexer lexer(getTestFilePath("test_special6.json"));
        bool caught_exception = false;
        try {
            auto tokens = lexer.tokenize();
        } catch (const std::runtime_error& e) {
            caught_exception = true;
            assert(std::string(e.what()).find(
                       "Unexpected EOF while parsing 'true'") !=
                   std::string::npos);
        }
        assert(caught_exception);
    }

    std::cout << "All special token tests passed!" << std::endl;
}

void test_structural_tokens() {
    // Test case 1: Object
    {
        std::ofstream testFile(getTestFilePath("test_struct1.json"));
        testFile << R"({"key": "value"})";
        testFile.close();

        Lexer lexer(getTestFilePath("test_struct1.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 5);
        assert(tokens[0].type == TokenType::LEFT_BRACE);
        assert(tokens[1].type == TokenType::STRING);
        assert(tokens[2].type == TokenType::COLON);
        assert(tokens[3].type == TokenType::STRING);
        assert(tokens[4].type == TokenType::RIGHT_BRACE);
    }

    // Test case 2: Array
    {
        std::ofstream testFile(getTestFilePath("test_struct2.json"));
        testFile << R"([1, 2, 3])";
        testFile.close();

        Lexer lexer(getTestFilePath("test_struct2.json"));
        auto tokens = lexer.tokenize();

        assert(tokens.size() == 7);
        assert(tokens[0].type == TokenType::LEFT_BRACKET);
        assert(tokens[1].type == TokenType::NUMBER);
        assert(tokens[2].type == TokenType::COMMA);
        assert(tokens[3].type == TokenType::NUMBER);
        assert(tokens[4].type == TokenType::COMMA);
        assert(tokens[5].type == TokenType::NUMBER);
        assert(tokens[6].type == TokenType::RIGHT_BRACKET);
    }

    std::cout << "All structural token tests passed!" << std::endl;
}

int main() {
    test_string_tokenization();
    test_number_tokenization();
    test_special_tokens();
    test_structural_tokens();
    std::cout << "All tests passed successfully!" << std::endl;
    return 0;
}

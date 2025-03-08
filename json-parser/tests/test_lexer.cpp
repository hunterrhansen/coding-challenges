#include "lexer.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <fstream>

void test_string_tokenization() {
    // Test case 1: Basic string
    {
        std::ofstream testFile("test1.json");
        testFile << R"("Hello, World!")";
        testFile.close();
        
        Lexer lexer("test1.json");
        auto tokens = lexer.tokenize();
        
        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::STRING);
        assert(tokens[0].lexeme == "Hello, World!");
    }

    // Test case 2: Empty string
    {
        std::ofstream testFile("test2.json");
        testFile << R"("")";
        testFile.close();
        
        Lexer lexer("test2.json");
        auto tokens = lexer.tokenize();
        
        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::STRING);
        assert(tokens[0].lexeme == "");
    }

    // Test case 3: String with escaped quotes
    {
        std::ofstream testFile("test3.json");
        testFile << R"("Hello \"World\"")";
        testFile.close();
        
        Lexer lexer("test3.json");
        auto tokens = lexer.tokenize();
        
        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::STRING);
        assert(tokens[0].lexeme == R"(Hello "World")");
    }

    // Test case 4: Unterminated string (should throw)
    {
        std::ofstream testFile("test4.json");
        testFile << R"("Hello)";  // Missing closing quote
        testFile.close();
        
        Lexer lexer("test4.json");
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
        std::ofstream testFile("test_num1.json");
        testFile << "123";
        testFile.close();
        
        Lexer lexer("test_num1.json");
        auto tokens = lexer.tokenize();
        
        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "123");
    }

    // Test case 2: Floating point
    {
        std::ofstream testFile("test_num2.json");
        testFile << "123.456";
        testFile.close();
        
        Lexer lexer("test_num2.json");
        auto tokens = lexer.tokenize();
        
        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "123.456");
    }

    // Test case 3: Negative numbers
    {
        std::ofstream testFile("test_num3.json");
        testFile << "-123.456";
        testFile.close();
        
        Lexer lexer("test_num3.json");
        auto tokens = lexer.tokenize();
        
        assert(tokens.size() == 1);
        assert(tokens[0].type == TokenType::NUMBER);
        assert(tokens[0].lexeme == "-123.456");
    }

    std::cout << "All number tokenization tests passed!" << std::endl;
}

void test_special_tokens() {
    // Test case 1: true, false, null
    {
        std::ofstream testFile("test_special1.json");
        testFile << "true false null";
        testFile.close();
        
        Lexer lexer("test_special1.json");
        auto tokens = lexer.tokenize();
        
        assert(tokens.size() == 3);
        assert(tokens[0].type == TokenType::TRUE);
        assert(tokens[1].type == TokenType::FALSE);
        assert(tokens[2].type == TokenType::NULL_TOKEN);
    }

    std::cout << "All special token tests passed!" << std::endl;
}

void test_structural_tokens() {
    // Test case 1: Object
    {
        std::ofstream testFile("test_struct1.json");
        testFile << R"({"key": "value"})";
        testFile.close();
        
        Lexer lexer("test_struct1.json");
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
        std::ofstream testFile("test_struct2.json");
        testFile << R"([1, 2, 3])";
        testFile.close();
        
        Lexer lexer("test_struct2.json");
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

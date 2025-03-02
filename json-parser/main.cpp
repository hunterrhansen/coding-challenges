#include <iostream>
#include "lexer.h"
#include <fstream>
#include <sstream>

int main()
{
    // Test valid JSON
    try
    {
        std::cout << "Testing valid.json" << std::endl;
        Lexer *lexer = new Lexer("./tests/step1/valid.json");
        std::vector<Token> tokens = lexer->tokenize();

        // Verify expected tokens
        if (tokens.size() != 2 || 
            tokens[0].lexeme != "{" || 
            tokens[1].lexeme != "}")
        {
            std::cerr << "Test failed: Expected tokens [\"{\", \"}\"]" << std::endl;
            return 1;
        }
        
        std::cout << "Valid test passed" << std::endl;
        delete lexer;

        // Test invalid JSON
        std::cout << "\nTesting invalid.json" << std::endl;
        lexer = new Lexer("./tests/step1/invalid.json");
        tokens = lexer->tokenize();  // Should throw an exception
        
        std::cerr << "Test failed: Expected exception for invalid.json" << std::endl;
        delete lexer;
        return 1;
    }
    catch (const std::exception &e)
    {
        std::cout << "Expected error caught: " << e.what() << std::endl;
    }

    return 0;
}
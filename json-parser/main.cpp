#include <iostream>
#include "lexer.h"
#include <fstream>
#include <sstream>

std::string readFile(const std::string &filename)
{
    std::ifstream file(filename);
    if (!file.is_open())
    {
        throw std::runtime_error("Could not open file " + filename);
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

int main()
{
    try
    {
        // read json file
        std::string json_content = readFile("./tests/step1/valid.json");
        Lexer *lexer = new Lexer(json_content);
        std::vector<Token> tokens = lexer->tokenize();

        std::cout << "Tokenization complete" << std::endl;

        // clean up
        delete lexer;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
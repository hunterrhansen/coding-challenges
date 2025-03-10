#include <cassert>
#include <fstream>
#include <iostream>
#include <sstream>

#include "lexer.h"
#include "parser.h"

std::string getTestFilePath(const std::string& filename) {
    return "tests/temp/" + filename;
}

void test_empty_json() {
    // Test case 1: Empty object
    {
        std::ofstream testFile(getTestFilePath("parser_test1.json"));
        testFile << "{}";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test1.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    // Test case 2: Empty array
    {
        std::ofstream testFile(getTestFilePath("parser_test2.json"));
        testFile << "[]";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test2.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    std::cout << "Empty JSON tests passed!" << std::endl;
}

void test_simple_values() {
    // Test case 1: Simple string
    {
        std::ofstream testFile(getTestFilePath("parser_test3.json"));
        testFile << R"("test")";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test3.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    // Test case 2: Simple number
    {
        std::ofstream testFile(getTestFilePath("parser_test4.json"));
        testFile << "42";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test4.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    // Test case 3: Boolean values
    {
        std::ofstream testFile(getTestFilePath("parser_test5.json"));
        testFile << "true";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test5.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    std::cout << "Simple value tests passed!" << std::endl;
}

void test_simple_objects() {
    // Test case 1: Object with one key-value pair
    {
        std::ofstream testFile(getTestFilePath("parser_test6.json"));
        testFile << R"({"key": "value"})";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test6.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    // Test case 2: Object with multiple key-value pairs
    {
        std::ofstream testFile(getTestFilePath("parser_test7.json"));
        testFile << R"({"name": "John", "age": 30, "city": "New York"})";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test7.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    std::cout << "Simple object tests passed!" << std::endl;
}

void test_simple_arrays() {
    // Test case 1: Array with simple values
    {
        std::ofstream testFile(getTestFilePath("parser_test8.json"));
        testFile << R"([1, 2, 3, 4, 5])";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test8.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    // Test case 2: Array with mixed types
    {
        std::ofstream testFile(getTestFilePath("parser_test9.json"));
        testFile << R"([1, "two", true, null])";
        testFile.close();

        Lexer lexer(getTestFilePath("parser_test9.json"));
        auto tokens = lexer.tokenize();
        Parser parser(tokens);

        assert(parser.parse() == true);
    }

    std::cout << "Simple array tests passed!" << std::endl;
}

int main() {
    test_empty_json();
    test_simple_values();
    test_simple_objects();
    test_simple_arrays();
    std::cout << "All parser tests passed successfully!" << std::endl;
    return 0;
}
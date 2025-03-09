#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

#include "lexer.h"
#include "parser.h"

// Helper function to test a valid JSON file
bool testValidFile(const std::string& filepath) {
    std::cout << "Testing valid file: " << filepath << std::endl;
    try {
        // Test lexing
        Lexer lexer(filepath);
        std::vector<Token> tokens = lexer.tokenize();

        // Test parsing
        Parser parser(tokens);
        bool parseResult = parser.parse();

        if (!parseResult) {
            std::cerr << "Parse failed for valid file: " << filepath
                      << std::endl;
            return false;
        }

        std::cout << "✓ Successfully parsed: " << filepath << std::endl;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "✗ Error processing valid file " << filepath << ": "
                  << e.what() << std::endl;
        return false;
    }
}

// Helper function to test an invalid JSON file
bool testInvalidFile(const std::string& filepath) {
    std::cout << "Testing invalid file: " << filepath << std::endl;
    try {
        // Test lexing
        Lexer lexer(filepath);
        std::vector<Token> tokens = lexer.tokenize();

        // Test parsing
        Parser parser(tokens);
        bool parseResult = parser.parse();

        // If we get here without an exception, that's a problem
        std::cerr << "✗ Failed: Expected error for invalid file: " << filepath
                  << std::endl;
        return false;
    } catch (const std::exception& e) {
        std::cout << "✓ Expected error caught for " << filepath << ": "
                  << e.what() << std::endl;
        return true;
    }
}

// Helper function to run tests for a specific step
bool runStepTests(int step) {
    bool allPassed = true;
    std::string baseDir = "./tests/step" + std::to_string(step) + "/";

    // Test valid files
    std::vector<std::string> validFiles = {baseDir + "valid.json"};

    // Add valid2.json only for step 2
    if (step == 2) {
        validFiles.push_back(baseDir + "valid2.json");
    }

    // Test all valid files
    for (const auto& file : validFiles) {
        if (!testValidFile(file)) {
            allPassed = false;
        }
    }

    // Test invalid file
    if (!testInvalidFile(baseDir + "invalid.json")) {
        allPassed = false;
    }

    // Add invalid2.json for step 2
    if (step == 2) {
        if (!testInvalidFile(baseDir + "invalid2.json")) {
            allPassed = false;
        }
    }

    return allPassed;
}

int main() {
    bool allTestsPassed = true;

    // Run tests for each step
    for (int step = 1; step <= 4; step++) {
        std::cout << "\n=== Running Step " << step << " Tests ===\n"
                  << std::endl;
        if (!runStepTests(step)) {
            allTestsPassed = false;
        }
    }

    // Print final results
    if (allTestsPassed) {
        std::cout << "\n✅ All tests passed successfully!" << std::endl;
        return 0;
    } else {
        std::cerr << "\n❌ Some tests failed!" << std::endl;
        return 1;
    }
}
# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -pedantic -I. -g

# Target executable names
TARGET = lexer
TEST_LEXER = test_lexer
TEST_PARSER = test_parser

# Source directories
SRC_DIR = .
TEST_DIR = tests
TEST_TEMP_DIR = $(TEST_DIR)/temp

# Source files
SOURCES = $(SRC_DIR)/lexer.cpp $(SRC_DIR)/parser.cpp
TEST_LEXER_SOURCES = $(TEST_DIR)/test_lexer.cpp
TEST_PARSER_SOURCES = $(TEST_DIR)/test_parser.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_LEXER_OBJECTS = $(TEST_LEXER_SOURCES:$(TEST_DIR)/%.cpp=$(TEST_DIR)/%.o) lexer.o
TEST_PARSER_OBJECTS = $(TEST_PARSER_SOURCES:$(TEST_DIR)/%.cpp=$(TEST_DIR)/%.o) lexer.o parser.o

# Define build directory
BUILD_DIR = build

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(TEST_TEMP_DIR))

# Main targets
all: $(TARGET) build_tests

# Build the main executable
$(TARGET): main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) main.o $(OBJECTS) -o $(BUILD_DIR)/$(TARGET)

# Build the test executables
build_tests: build_test_lexer build_test_parser

build_test_lexer: $(TEST_LEXER_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_LEXER_OBJECTS) -o $(BUILD_DIR)/$(TEST_LEXER)

build_test_parser: $(TEST_PARSER_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_PARSER_OBJECTS) -o $(BUILD_DIR)/$(TEST_PARSER)

# Pattern rules for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
	rm -f *.o $(TEST_DIR)/*.o $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(TEST_LEXER) $(BUILD_DIR)/$(TEST_PARSER)
	rm -rf $(TEST_TEMP_DIR)/*

# Test Rules
test: build_tests run_tests

run_tests: run_test_lexer run_test_parser

run_test_lexer:
	./$(BUILD_DIR)/$(TEST_LEXER)

run_test_parser:
	./$(BUILD_DIR)/$(TEST_PARSER)
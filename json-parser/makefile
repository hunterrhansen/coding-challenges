# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -pedantic -I.

# Target executable names
TARGET = lexer
TEST_TARGET = test_lexer

# Source directories
SRC_DIR = .
TEST_DIR = tests
TEST_TEMP_DIR = $(TEST_DIR)/temp

# Source files
SOURCES = $(SRC_DIR)/lexer.cpp
TEST_SOURCES = $(TEST_DIR)/test_lexer.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:$(TEST_DIR)/%.cpp=$(TEST_DIR)/%.o) lexer.o

# Define build directory
BUILD_DIR = build

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))
$(shell mkdir -p $(TEST_TEMP_DIR))

# Main targets
all: $(TARGET) $(TEST_TARGET)

$(TARGET): main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) main.o $(OBJECTS) -o $(BUILD_DIR)/$(TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(BUILD_DIR)/$(TEST_TARGET)

# Pattern rules for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_DIR)/%.o: $(TEST_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
	rm -f *.o $(TEST_DIR)/*.o $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(TEST_TARGET)
	rm -rf $(TEST_TEMP_DIR)/*

# Test Rule
test: $(TEST_TARGET)
	./$(BUILD_DIR)/$(TEST_TARGET)
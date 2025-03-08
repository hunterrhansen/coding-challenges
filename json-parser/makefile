# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11 -pedantic

# Target executable names
TARGET = lexer
TEST_TARGET = test_lexer

# Source files
SOURCES = lexer.cpp
TEST_SOURCES = test_lexer.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)
TEST_OBJECTS = $(TEST_SOURCES:.cpp=.o) lexer.o

# Define build directory
BUILD_DIR = build

# Ensure build directory exists
$(shell mkdir -p $(BUILD_DIR))

# Main targets
all: $(TARGET) $(TEST_TARGET)

$(TARGET): main.o $(OBJECTS)
	$(CXX) $(CXXFLAGS) main.o $(OBJECTS) -o $(BUILD_DIR)/$(TARGET)

$(TEST_TARGET): $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $(TEST_OBJECTS) -o $(BUILD_DIR)/$(TEST_TARGET)

# Pattern rule for object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean Rule
clean:
	rm -f *.o $(BUILD_DIR)/$(TARGET) $(BUILD_DIR)/$(TEST_TARGET)

# Test Rule
test: $(TEST_TARGET)
	./$(BUILD_DIR)/$(TEST_TARGET)
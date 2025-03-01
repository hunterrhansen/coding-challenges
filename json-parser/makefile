# Compiler to user
CXX = g++

# Compiler flags
CXXFlags = -Wall -std=c++11

# Target executable name
TARGET = lexer

# Source files
SOURCES = main.cpp lexer.cpp

# Object files (automatically generated from sources)
OBJECTS = $(SOURCES:.cpp=.o)

# Main target
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFlags) $(OBJECTS) -o $(TARGET)

# Clean Rule
clean:
	rm -f $(OBJECTS) $(TARGET)
# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++23 -I$(OPENSSL_DIR)/include  # Include OpenSSL headers
LDFLAGS = -L$(OPENSSL_DIR)/lib -lssl -lcrypto  # Link OpenSSL libraries

# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
OPENSSL_DIR = openssl

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Output executable
TARGET = $(BIN_DIR)/papy

# Default target to build
all: directories $(TARGET)

# Create required directories if they don't exist
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)  # Link OpenSSL libraries

# Rule to compile .cpp to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Rebuild the project
rebuild: clean all

# Install (optional, you can customize)
install:
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean rebuild install directories
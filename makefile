# Directories
SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin
OPENSSL_DIR = $(SRC_DIR)/dependencies/openssl/include  # Local OpenSSL directory
ZLIB_DIR = $(SRC_DIR)/dependencies/gzip/include

# Compiler and flags
CXX = g++
CXXFLAGS_COMMON = -I$(OPENSSL_DIR) -I$(ZLIB_DIR) -MMD -MP -O3 -g -fno-omit-frame-pointer  # Include dependency tracking
ifeq ($(shell uname), Darwin)
	CXXFLAGS = -std=c++2b $(CXXFLAGS_COMMON)  # macOS-specific flags
else
	CXXFLAGS = -std=c++23 $(CXXFLAGS_COMMON)  # Default flags
endif

# Linker flags
LDFLAGS = -L$(OPENSSL_DIR) -L$(ZLIB_DIR) -lssl -lcrypto -lz # Link local OpenSSL libraries

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Track build dependencies
DEP_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.d)

# Output executable
TARGET = $(BIN_DIR)/papy

# Default target to build
all: directories $(TARGET)

# Create required directories if they don't exist
directories:
	@mkdir -p $(OBJ_DIR) $(BIN_DIR)

# Link object files to create the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(OBJ_FILES) -o $@ $(LDFLAGS)

# Rule to compile .cpp to .o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS) -c $< -o $@

-include $(DEP_FILES)

# Clean up object files and executable
clean:
	rm -rf $(OBJ_DIR)/*.o $(TARGET)

# Rebuild the project
rebuild: clean all

# Install (optional, you can customize)
install:
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean rebuild install directories

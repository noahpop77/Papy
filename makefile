# Directories
SRC_DIR = src

OBJ_DIR = obj
OBJ_DIR_REL = $(OBJ_DIR)/rel
OBJ_DIR_DEV = $(OBJ_DIR)/dev

BIN_DIR = bin
BIN_DIR_REL = $(BIN_DIR)/rel
BIN_DIR_DEV = $(BIN_DIR)/dev

OPENSSL_DIR = $(SRC_DIR)/dependencies/openssl/include  # Local OpenSSL directory
ZLIB_DIR = $(SRC_DIR)/dependencies/gzip/include

# Compiler and flags
CXX = g++
# Link Time Optimization + high level of optimization + strip
CXX_REL_OPT_FLAGS = -flto -O3 -s $(STATIC_FLAG) # set STATIC_FLAG=-static in make invocation from terminal to get static linking 
CXXFLAGS_COMMON = -I$(OPENSSL_DIR) -I$(ZLIB_DIR) -MMD -MP # Include dependency tracking
ifeq ($(shell uname), Darwin) # macOS-specific flags
	CXXFLAGS_REL = $(CXX_REL_OPT_FLAGS) -std=c++2b $(CXXFLAGS_COMMON)
	CXXFLAGS_DEV = -std=c++2b $(CXXFLAGS_COMMON)
else # Default flags
	CXXFLAGS_REL = $(CXX_REL_OPT_FLAGS) -std=c++23 $(CXXFLAGS_COMMON)
	CXXFLAGS_DEV = -std=c++23 $(CXXFLAGS_COMMON)
endif

# Linker flags. Actually, just g++ flags
# Links with local OpenSSL libraries and some more on windows (TODO: other platforms)
LDFLAGS_DEV = -L$(OPENSSL_DIR) -L$(ZLIB_DIR) -lssl -lcrypto -lz 
# On windows these libs are needed (otherwise you end up using funs you do not link with)
ifeq ($(OS),Windows_NT)
	LDFLAGS_DEV += -lws2_32 -lcrypt32
endif
LDFLAGS_REL = $(CXX_REL_OPT_FLAGS) $(LDFLAGS_DEV)

# Source files and object files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
# object files for rel/dev are stored separately to allow independent incremental building of both
OBJ_FILES_REL = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR_REL)/%.o)
OBJ_FILES_DEV = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR_DEV)/%.o)

# Output executable
TARGET_REL = $(BIN_DIR_REL)/papy
TARGET_DEV = $(BIN_DIR_DEV)/papy

# Default target to build
all: rel

dev: directories $(TARGET_DEV)
rel: directories $(TARGET_REL)

# Create required directories if they don't exist
directories:
	@mkdir -p $(OBJ_DIR)/dev $(OBJ_DIR)/rel
	@mkdir -p $(BIN_DIR)/dev $(BIN_DIR)/rel

# Link object files to create the executable
$(TARGET_REL): $(OBJ_FILES_REL)
	$(CXX) $(OBJ_FILES_REL) -o $@ $(LDFLAGS_REL)
$(TARGET_DEV): $(OBJ_FILES_DEV)
	$(CXX) $(OBJ_FILES_DEV) -o $@ $(LDFLAGS_DEV)

# Rule to compile .cpp to .o
$(OBJ_DIR_REL)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS_REL) -c $< -o $@
$(OBJ_DIR_DEV)/%.o: $(SRC_DIR)/%.cpp | directories
	$(CXX) $(CXXFLAGS_DEV) -c $< -o $@

# Track build dependencies
DEP_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR_REL)/%.d)
-include $(DEP_FILES)
DEP_FILES = $(SRC_FILES:$(SRC_DIR)/%.cpp=$(OBJ_DIR_DEV)/%.d)
-include $(DEP_FILES)

# Clean up object files and executable (dep files too for safity, they take nothing to create but can break build if something is messed up)
clean:
	rm -rf $(OBJ_DIR)/*
	rm -rf $(BIN_DIR)/*

# Rebuild the project
rebuild: clean all

# Install (optional, you can customize)
install:
	cp $(TARGET) /usr/local/bin/

.PHONY: all clean rebuild install directories

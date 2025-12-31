# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra

# Directories
INCLUDE_DIR = include
CACHE_INCLUDE_DIR = include/cache

SRC_ALLOCATOR = src/allocator
SRC_CACHE = src/cache
TESTS = tests

# Targets
.PHONY: all clean

all: allocator test_allocator test_cache

# Allocator CLI
allocator:
	$(CXX) src/main.cpp $(SRC_ALLOCATOR)/*.cpp -I$(INCLUDE_DIR) -o memory-simulator

# Allocator tests
test_allocator:
	$(CXX) $(TESTS)/test_allocator.cpp $(SRC_ALLOCATOR)/*.cpp -I$(INCLUDE_DIR) -o test_allocator

# Cache tests
test_cache:
	$(CXX) $(TESTS)/test_cache.cpp $(SRC_CACHE)/*.cpp -I$(CACHE_INCLUDE_DIR) -o test_cache

# Cleanup
clean:
	rm -f memory-simulator test_allocator test_cache

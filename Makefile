CPP_FLAGS := -std=c++17 -Wall
CC := g++

trie: trie.cpp
	$(CC) $(CPP_FLAGS) $^ -o $@

INC := -I./ext/tinytest
override CXXFLAGS := -std=c++17 -Werror $(INC) $(CXXFLAGS)
CC = g++

TT_OBJ := $(addprefix ext/tinytest/,tinytest.o)
TRIE_OBJ := test_trie.o trie.o
OBJ := $(TT_OBJ) $(TRIE_OBJ)

BIN := test_trie

all: $(BIN)

test_trie: $(TT_OBJ) $(TRIE_OBJ)

clean:
	rm -f $(OBJ) $(BIN)

test: $(BIN)
	./test_trie

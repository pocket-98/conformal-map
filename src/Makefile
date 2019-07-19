# Makefile for a.out
# usage: make [FLAGS=<flags>]

BIN     		= ../bin/
MAIN    		= main.cpp
CSOURCES		=
SOURCES 		= tree.cpp
HEADERS 		= tree.hpp
TARGET  		= a.out

TEST    		= test/main.cpp
TSOURCES		= test/test_tree.cpp
THEADERS		= test/test.hpp test/test_tree.hpp
TRGTST  		= a.out-test

CC      		= gcc
CXX     		= g++
GCCOPTS 		= -Wall -Wextra -pedantic -ansi -std=c99
GXXOPTS 		= -Wall -Wextra -pedantic -ansi -std=c++11 -g
LDLIBS  		=
CCOPTS  		= $(GCCOPTS) $(FLAGS)
CXXOPTS 		= $(GXXOPTS) $(FLAGS)
RM      		= rm -f
MKDIR   		= mkdir -p

COBJ    		= $(patsubst %.c,$(BIN)%.o,$(CSOURCES))
OBJ     		= $(patsubst %.cpp,$(BIN)%.o,$(SOURCES))
TOBJ    		= $(patsubst %.cpp,$(BIN)%.o,$(TSOURCES))
MAINOBJ 		= $(patsubst %.cpp,$(BIN)%.o,$(MAIN))
TMAINOBJ		= $(patsubst %.cpp,$(BIN)%.o,$(TEST))

all:    		$(TARGET)

# compile .c source files to objects (.o)
$(BIN)%.o: 		%.c $(HEADERS) $(THEADERS)
				@$(MKDIR) $(@D)
				$(CC) $(CCOPTS) -o $@ -c $<

# compile .cpp source files to objects (.o)
$(BIN)%.o: 		%.cpp $(HEADERS) $(THEADERS)
				@$(MKDIR) $(@D)
				$(CXX) $(CXXOPTS) -o $@ -c $<

# link executable
$(TARGET):		$(OBJ) $(COBJ) $(MAINOBJ)
				$(CXX) $(CXXOPTS) -o $@ $^ $(LDLIBS)
#				$(CC) $(CCOPTS) -o $@ $^ $(LDLIBS)
				@echo "binary compiled: '$(TARGET)'"

# link test executable
$(TRGTST):		$(OBJ) $(COBJ) $(TOBJ) $(TMAINOBJ)
				$(CXX) $(CXXOPTS) -o $@ $^ $(LDLIBS)
#				$(CC) $(CCOPTS) -o $@ $^ $(LDLIBS)
				@echo "binary compiled: '$(TRGTST)'"

# run test executable
test:   		$(TRGTST)
				@echo "testing '$(TEST)':"
				./$(TRGTST)

# remove objects and executable
clean:
				$(RM) $(TARGET) $(OBJ) $(COBJ) $(MAINOBJ)
				$(RM) $(TRGTST) $(TOBJ) $(TMAINOBJ)


# Makefile for a.out
# usage: make [FLAGS=<flags>]

BIN     		= ../bin/
MAIN    		= main.cpp
SOURCES 		= util.cpp
CSOURCES		=
HEADERS 		= util.hpp
TARGET  		= a.out

TEST    		= test/test_util.cpp test/main.cpp
THEADERS		= test/test_util.hpp test/test.hpp
TRGTST  		= a.out-test

CC      		= gcc
CXX     		= g++
GCCOPTS 		= -Wall -Wextra -pedantic -ansi -std=c99
GXXOPTS 		= -Wall -Wextra -pedantic -ansi -std=c++11
LDLIBS  		=
CCOPTS  		= $(GCCOPTS) $(FLAGS)
CXXOPTS 		= $(GXXOPTS) $(FLAGS)
RM      		= rm -f
MKDIR   		= mkdir -p

COBJ    		= $(patsubst %.c,$(BIN)%.o,$(CSOURCES))
OBJ     		= $(patsubst %.cpp,$(BIN)%.o,$(SOURCES))
MOBJ    		= $(patsubst %.cpp,$(BIN)%.o,$(MAIN))
TOBJ    		= $(patsubst %.cpp,$(BIN)%.o,$(TEST))

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
$(TARGET):		$(OBJ) $(COBJ) $(MOBJ)
				$(CXX) $(CXXOPTS) -o $@ $^ $(LDLIBS)
#				$(CC) $(CCOPTS) -o $@ $^ $(LDLIBS)
				@echo "binary compiled: '$(TARGET)'"

# link test executable
$(TRGTST):		$(OBJ) $(COBJ) $(TOBJ)
				$(CXX) $(CXXOPTS) -o $@ $^ $(LDLIBS)
#				$(CC) $(CCOPTS) -o $@ $^ $(LDLIBS)
				@echo "binary compiled: '$(TRGTST)'"

# run test executable
test:   		$(TRGTST)
				@echo "testing '$(TEST)':"
				./$(TRGTST)

# remove objects and executable
clean:
				$(RM) $(TARGET) $(OBJ) $(COBJ) $(MOBJ)
				$(RM) $(TRGTST) $(TOBJ)


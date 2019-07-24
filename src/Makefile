# Makefile for a.out
# usage: make [FLAGS=<flags>]
# make test: compares expected to actual output
# make cov:  rebuilds project, runs tests and checks line coverage

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

TRGCOV  		= a.out-cov
GCOV    		= gcov
COVOPTS 		= -fprofile-arcs -ftest-coverage
GCOVOPTS		= -b -n
GCOVPRNT		= grep -A 4 "\.cpp" | grep -v "/usr/include" \
				| sed -re '/^No.*/d' | sed -re 's/(^Lin|^Bra|^Tak|^Cal)/    \1/'

COVDA   		= $(patsubst %.c,$(BIN)%.gcda,$(CSOURCES)) \
				$(patsubst %.cpp,$(BIN)%.gcda,$(SOURCES)) \
				$(patsubst %.cpp,$(BIN)%.gcda,$(TSOURCES)) \
				$(patsubst %.cpp,$(BIN)%.gcda,$(TEST))

COVNO   		= $(patsubst %.c,$(BIN)%.gcno,$(CSOURCES)) \
				$(patsubst %.cpp,$(BIN)%.gcno,$(SOURCES)) \
				$(patsubst %.cpp,$(BIN)%.gcno,$(TSOURCES)) \
				$(patsubst %.cpp,$(BIN)%.gcno,$(TEST)) \

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
				@echo "binary compiled: '$(TRGTST)'"

# run test executable
test:   		$(TRGTST)
				@echo "testing:"
				./$(TRGTST)

# compile .c source files to objects (.o) with line coverage on
$(BIN)%.gcda:	%.c
				@$(MKDIR) $(@D)
				$(CC) $(CCOPTS) $(COVOPTS) -o $(patsubst %.gcda,%.o,$@) -c $<

# compile .cpp source files to objects (.o) with line coverage on
$(BIN)%.gcda:	%.cpp
				@$(MKDIR) $(@D)
				$(CXX) $(CXXOPTS) $(COVOPTS) -o $(patsubst %.gcda,%.o,$@) -c $<

# link executable with line coverage on
$(TRGCOV):		$(COVDA) $(COVNO)
				$(CXX) $(CXXOPTS) $(COVOPTS) -o $@ $(OBJ) $(COBJ) $(TOBJ) $(TMAINOBJ) $(LDLIBS)
				@echo "binary compiled: '$(TRGCOV)'"

cov:    		$(TRGCOV)
				@echo "testing:"
				./$(TRGCOV)
				@echo -e "\nchecking coverage '$(TRGCOV)':"
				@$(GCOV) $(GCOVOPTS) $(COVDA) | $(GCOVPRNT)

# remove objects and executable
.PHONY: clean
clean:
				$(RM) $(TARGET) $(OBJ) $(COBJ) $(MAINOBJ)
				$(RM) $(TRGTST) $(TOBJ) $(TMAINOBJ)
				$(RM) $(TRGCOV) $(COVDA) $(COVNO)


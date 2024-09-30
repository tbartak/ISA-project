COMPILER = g++
FLAGS = -std=c++14 -lpcap -Wall -Wextra -Werror -pedantic
SRC = main.cpp argument_parser.cpp utility.cpp
EXEC = isa-top

all:
	$(COMPILER) $(FLAGS) -o $(EXEC) $(SRC)

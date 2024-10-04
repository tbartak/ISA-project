COMPILER = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic
SRC = main.cpp argument_parser.cpp utility.cpp
EXEC = isa-top

all:
	$(COMPILER) $(FLAGS) -o $(EXEC) $(SRC) -lpcap -lncurses

run:
	sudo ./$(EXEC) -i lo

run eth0:
	sudo ./$(EXEC) -i eth0
COMPILER = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic -g
SRC = main.cpp config.cpp utility.cpp packet_info.cpp network_interface.cpp packet_config.cpp sorting.cpp globals.cpp packet_handling.cpp
EXEC = isa-top

all:
	$(COMPILER) $(FLAGS) -o $(EXEC) $(SRC) -lpcap -lncurses

run lo:
	sudo ./$(EXEC) -i lo

run:
	sudo ./$(EXEC) -i eth0

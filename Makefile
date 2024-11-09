COMPILER = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic
DEBUG = -g
SRC = main.cpp config.cpp utility.cpp packet_info.cpp network_interface.cpp sorting.cpp packet_config.cpp globals.cpp packet_handling.cpp display.cpp
EXEC = isa-top

all:
	$(COMPILER) $(FLAGS) -o $(EXEC) $(SRC) -lpcap -lncurses

run lo:
	sudo ./$(EXEC) -i lo

run:
	sudo ./$(EXEC) -i eth0

run 10s:
	sudo ./$(EXEC) -i eth0 -t 10

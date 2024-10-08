COMPILER = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic
SRC = main.cpp config.cpp utility.cpp packet_info.cpp network_interface.cpp packet_config.cpp
EXEC = isa-top

all:
	$(COMPILER) $(FLAGS) -o $(EXEC) $(SRC) -lpcap -lncurses

run:
	sudo ./$(EXEC) -i lo

run eth0:
	sudo ./$(EXEC) -i eth0
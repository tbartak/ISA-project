COMPILER = g++
FLAGS = -std=c++14 -Wall -Wextra -Werror -pedantic -Wunused -Wunused-parameter
DEBUG = -g

SRC = main.cpp config.cpp utility.cpp packet_info.cpp network_interface.cpp sorting.cpp packet_config.cpp globals.cpp packet_handling.cpp display.cpp
EXEC = isa-top

all:
	$(COMPILER) $(FLAGS) -o $(EXEC) $(SRC) -lpcap -lncurses

run:
	sudo ./$(EXEC) -i eth0

tar:
	tar -czf xbarta51.tar.gz \
		--exclude='.vscode' \
		--exclude='*.o' \
		--exclude='.git' \
		--exclude='.gitignore' \
		--exclude='*.tar.gz' \
		--exclude='$(EXEC)' \
		*.cpp *.h Makefile manual.pdf isa-top.1 LICENSE scapy/
CC = g++
CXX_FLAGS += `mysql_config --cflags` -I./src -std=c++11
LD_FLAGS += `mysql_config --libs`

SRC_DIR = ./src

all: nwu

nwu: main.o nudb.o
	$(CC) -o nwu $^ $(LD_FLAGS)

main.o : $(SRC_DIR)/main.cpp
	$(CC) -c $(CXX_FLAGS) $<

nudb.o : $(SRC_DIR)/nudb.cpp
	$(CC) -c $(CXX_FLAGS) $<

.PHONY: clean
clean:
	rm -rf *.o nwu

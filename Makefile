CC = g++
CXX_FLAGS += `mysql_config --cflags` -I./src -I./3rdparty/mongoose -std=c++11
LD_FLAGS += `mysql_config --libs`

SRC_DIR = ./src

all: nwu

nwu: main.o nudb.o mongoose.o
	$(CC) -o nwu $^ $(LD_FLAGS)

main.o : $(SRC_DIR)/main.cpp
	$(CC) -c $(CXX_FLAGS) $<

nudb.o : $(SRC_DIR)/nudb.cpp
	$(CC) -c $(CXX_FLAGS) $<

mongoose.o : ./3rdparty/mongoose/mongoose.c
	$(CC) -c $(CXX_FLAGS) $<

.PHONY: clean
clean:
	rm -rf *.o nwu

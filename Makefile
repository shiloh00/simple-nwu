CC = g++
CXX_FLAGS += `mysql_config --cflags`
LD_FLAGS += `mysql_config --libs`

SRC_DIR = ./src

all: main.o
	$(CC) -o nwu $^ $(LD_FLAGS)

main.o : $(SRC_DIR)/main.cpp
	$(CC) -c $(CXX_FLAGS) $<


.PHONY: clean
clean:
	rm -rf *.o nwu

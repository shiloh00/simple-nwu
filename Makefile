CC = g++
CXX_FLAGS += `mysql_config --cflags` -I./src -I./3rdparty/mongoose -std=c++11 -DMG_ENABLE_THREADS
LD_FLAGS += `mysql_config --libs` -pthread
INSTALL_DIR = /usr/lib/mysql/plugin

SRC_DIR = ./src

all: nwu lib_mysqludf_sys.so

nwu: main.o nudb.o mongoose.o
	$(CC) -o nwu $^ $(LD_FLAGS)

main.o : $(SRC_DIR)/main.cpp
	$(CC) -c $(CXX_FLAGS) $<

nudb.o : $(SRC_DIR)/nudb.cpp
	$(CC) -c $(CXX_FLAGS) $<

mongoose.o : ./3rdparty/mongoose/mongoose.c
	$(CC) -c $(CXX_FLAGS) $<

lib_mysqludf_sys.so: ./3rdparty/lib_mysqludf_sys/lib_mysqludf_sys.c
	$(CC) -m64 -fPIC -Wall -DMYSQL_DYNAMIC_PLUGIN `mysql_config --cflags` -I. -shared $^ -o lib_mysqludf_sys.so $(LD_FLAGS)

.PHONY: clean install
clean:
	-rm -rf *.o nwu *.so 
	-rm -rf $(INSTALL_DIR)/lib_mysqludf_sys.so

install: lib_mysqludf_sys.so
	cp lib_mysqludf_sys.so $(INSTALL_DIR)/

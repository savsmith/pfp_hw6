# the compiler: gcc for C program, define as g++ for C++
CC = g++

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -std=c++14 -g
LINKS = -lm
DEPS = graph.h
REMOVE_TARGETS = 

all: graph \
	 bf_a \
	 bf_b \
	 bf_c \
	 bf_d \
	 bf_e

graph: graph.cpp graph.h
	$(CC) graph.cpp -O3 -std=c++11 -g -c -o graph.o -I/opt/intel/vtune_amplifier/include

bf_a: graph.h graph.cpp
	$(CC) graph.o bf_a.cpp /opt/intel/vtune_amplifier/lib64/libittnotify.a -O3 -std=c++11 -g -pthread -o bf_a -lpthread -I/opt/intel/vtune_amplifier/include

bf_b: graph.h graph.cpp
	$(CC) graph.o bf_b.cpp /opt/intel/vtune_amplifier/lib64/libittnotify.a -O3 -std=c++11 -g -pthread -o bf_b -lpthread -I/opt/intel/vtune_amplifier/include

bf_c: graph.h graph.cpp
	$(CC) graph.o bf_c.cpp /opt/intel/vtune_amplifier/lib64/libittnotify.a -O3 -std=c++11 -g -pthread -o bf_c -lpthread -I/opt/intel/vtune_amplifier/include

bf_d: graph.h graph.cpp
	$(CC) graph.o bf_d.cpp /opt/intel/vtune_amplifier/lib64/libittnotify.a -O3 -std=c++11 -g -pthread -o bf_d -lpthread -I/opt/intel/vtune_amplifier/include

bf_e: graph.h graph.cpp
	$(CC) graph.o bf_e.cpp /opt/intel/vtune_amplifier/lib64/libittnotify.a -O3 -std=c++11 -g -pthread -o bf_e -lpthread -I/opt/intel/vtune_amplifier/include

clean:
	for f in $(REMOVE_TARGETS) ; do \
		rm -f f; \
	done


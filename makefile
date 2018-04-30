# the compiler: gcc for C program, define as g++ for C++
CC = icc

# compiler flags:
#  -g    adds debugging information to the executable file
#  -Wall turns on most, but not all, compiler warnings
CFLAGS = -std=c++11 -g -fopenmp -O3
LINKS = -lm
DEPS = graph.h
VTUNE_LINK = -I/opt/intel/vtune_amplifier/include
VTUNE = /opt/intel/vtune_amplifier/lib64/libittnotify.a
#VTUNE_LINK = 
#VTUNE = 
REMOVE_TARGETS = bf_a bf_b bf_c bf_e bf_d

all: graph \
	 bf_a \
	 bf_b \
	 bf_d \
	 bf_e

graph: graph.cpp graph.h
	$(CC) graph.cpp -O3 -std=c++11 -g -c -o graph.o 

bf_a: graph.h graph.cpp bf_a.cpp
	$(CC) graph.o bf_a.cpp $(VTUNE) -O3 $(CFLAGS) -o bf_a $(VTUNE_LINK) $(LINKS)

bf_b: graph.h graph.cpp bf_b.cpp
	$(CC) graph.o bf_b.cpp $(VTUNE) -O3 $(CFLAGS) -o bf_b $(VTUNE_LINK) $(LINKS)

bf_d: graph.h graph.cpp bf_d.cpp
	$(CC) graph.o bf_d.cpp $(VTUNE) -O3 $(CFLAGS) -o bf_d $(VTUNE_LINK) $(LINKS)

bf_e: graph.h graph.cpp bf_e.cpp
	$(CC) graph.o bf_e.cpp $(VTUNE) -O3 $(CFLAGS) -o bf_e $(VTUNE_LINK) $(LINKS)

clean:
	for f in $(REMOVE_TARGETS) ; do \
		rm -f $$f; \
	done


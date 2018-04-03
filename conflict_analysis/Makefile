CC = g++
CFLAG = -Wall -g
C11 = -std=c++11

default: sat

sat: main.o clause.o graph.o cnf.o
	$(CC) $(CFLAG) -o sat main.o clause.o graph.o cnf.o

main.o: main.cpp clause.h graph.h cnf.h
	$(CC) $(CFLAG) $(C11) -c main.cpp

clause.o: clause.cpp clause.h
	$(CC) $(CFLAG) $(C11) -c clause.cpp

graph.o: graph.cpp graph.h 
	$(CC) $(CFLAG) $(C11) -c graph.cpp

cnf.o: cnf.cpp cnf.h graph.h clause.h
	$(CC) $(CFLAG) $(C11) -c cnf.cpp

clean:
	$(RM) sat* *.o
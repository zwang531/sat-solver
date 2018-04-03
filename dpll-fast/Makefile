CC = g++
CFLAG = -Wall -g
C11 = -std=c++11

default: sat

sat: main.o clause.o cnf.o solver.o
	$(CC) $(CFLAG) -o sat main.o clause.o cnf.o solver.o

main.o: main.cpp solver.h clause.h cnf.h
	$(CC) $(CFLAG) $(C11) -c main.cpp

clause.o: clause.cpp clause.h
	$(CC) $(CFLAG) $(C11) -c clause.cpp

cnf.o: cnf.cpp cnf.h clause.h
	$(CC) $(CFLAG) $(C11) -c cnf.cpp

solver.o: solver.cpp solver.h cnf.h clause.h
	$(CC) $(CFLAG) $(C11) -c solver.cpp

clean:
	$(RM) sat* *.o
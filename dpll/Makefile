CC = g++
CFLAG = -Wall -g
C11 = -std=c++11

default: sat

sat: main.o clause.o solver.o
	$(CC) $(CFLAG) -o sat main.o clause.o solver.o 

main.o: main.cpp clause.h solver.h
	$(CC) $(CFLAG) $(C11) -c main.cpp

clause.o: clause.cpp clause.h
	$(CC) $(CFLAG) $(C11) -c clause.cpp

solver.o: solver.cpp solver.h clause.h
	$(CC) $(CFLAG) $(C11) -c solver.cpp

clean:
	$(RM) sat* *.o
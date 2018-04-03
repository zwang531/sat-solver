#ifndef SOLVER_H
#define SOLVER_H

#include "clause.h"
#include "cnf.h"

#define NOT_ASSIGNED -1
#define ASSIGNED_POSITIVE 1
#define ASSIGNED_NEGATIVE 0

#define TIMEOUT 2
#define SAT 1
#define UNSAT 0

#define DURATION 30.0


class Solver {

	Cnf* cnf;

public:
	Solver(Cnf* cnf);

	~Solver();

    int dpll(clock_t start);

	void print();
};

#endif
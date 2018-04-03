#include "solver.h"

#include <iostream>
#include <ctime>

using namespace std;


Solver::Solver(Cnf* cnf) {
    this->cnf = cnf;
}

Solver::~Solver() {
    if (cnf)
        delete cnf;
    cnf = NULL;
}

int Solver::dpll(clock_t start) {
    cnf->preliminaryClauses();

    while (true) {
    	double duration = (clock() - start) /(double) CLOCKS_PER_SEC;
    	if (duration >= DURATION)
    		return TIMEOUT;

    	cnf->unitPropagate();

    	if (cnf->checkConfliction()) {
    		if (cnf->isDecisionStackEmpty()) {
    			return UNSAT;
    		}

    		// backtrack to the last decision
    		vector<int> d = cnf->backtrack();

    		int l = d[0];
    		int v = d[1] == ASSIGNED_POSITIVE ? ASSIGNED_NEGATIVE : ASSIGNED_POSITIVE;
    		cnf->assignLiteral(l, v);

    	} else {
    		if (cnf->isComplete() == 0 && cnf->isSatisfied())
    			return SAT;

			// save a snapshot of current state for backtracking
			cnf->saveSnapshot();

			cnf->makeDecision();
    	}

    	cnf->updateClauses();
    }

    return 1;
}

void Solver::print() {
    if (cnf) {
        // cnf->printClauses();
        cnf->printAssignments();
    }
}

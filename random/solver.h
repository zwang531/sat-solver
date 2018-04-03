#ifndef SOLVER_H
#define SOLVER_H

#include "clause.h"

#include <iostream>
#include <vector>
using namespace std;

class Solver {
    int nLiterals, nClauses;

    vector<Clause*> *clauses;

public:
    Solver(int, int);

    ~Solver();

    int getNumLiterals() {return nLiterals;};

    int getNumClauses() {return nClauses;};

    void addClause(const vector<int>&);

    bool isSatisfiable(const vector<bool>&);

    void print();
};


#endif
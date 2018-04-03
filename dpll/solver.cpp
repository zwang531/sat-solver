#include "solver.h"

Solver::Solver(int nL, int nC) {
    nLiterals = nL;
    nClauses = nC;

    clauses = new vector<Clause*>();
}

Solver::~Solver() {
    if (clauses) {
        for (int i = 0; i < nClauses; i++) {
            delete clauses->at(i);
        }
    }
}

void Solver::addClause(const vector<int> &literals) {
    if (literals.empty()) return;

    Clause *clause = new Clause(literals);
    clauses->push_back(clause);
}

bool Solver::isSatisfiable(const vector<bool> &assignments) {
    for (int i = 0; i < nClauses; i++) {
        if (!clauses->at(i)->isSatisfiable(assignments)) {
            return false;
        }
    }
    return true;
}

void Solver::print() {
    for (int i = 0; i < nClauses; i++) {
        clauses->at(i)->print();
    }
}

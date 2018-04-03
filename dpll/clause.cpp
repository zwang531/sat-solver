#include "clause.h"

Clause::Clause(const vector<int>& l) {
    literals = new vector<int>(l);
}

Clause::~Clause() {
    if (literals) {
        delete literals;
    }
}

bool Clause::isSatisfiable(const vector<bool>& assignments) {
    int n = literals->size();
    for (int i = 0; i < n; i++) {
        int literal = literals->at(i);
        if (literal < 0) {
            literal = -1 * literal;
            if (!assignments[literal-1]) {
                return true;
            }
        }
        else {
            if (assignments[literal-1]) {
                return true;
            }
        }
    }
    return false;
}

void Clause::print() {
    int n = literals->size();
    for (int i = 0; i < n; i++) {
        cout << literals->at(i) << " ";
    }
    cout << endl;
}
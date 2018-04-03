#include "clause.h"

Clause::Clause(const vector<int>& a) {
    literals = new vector<int>();

    int nLiterals = a.size();
    for (int i = 0; i < nLiterals; i++) 
        literals->push_back(a[i]);
}

Clause::~Clause() {
    if (literals && literals->size()) 
        delete literals;
}

vector<int> Clause::getAssignedLiterals(const vector<int>* a) {
    vector<int> b;

    int nLiterals = literals->size();
    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);

        if (a->at(abs(l)-1) != NOT_ASSIGNED)
            b.push_back(abs(l));
    }

    return b;
}

bool Clause::isUnitClause(const vector<int>* a) {
    int nLiterals = literals->size();
    int n = 0;

    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);
        if (a->at(abs(l)-1) == NOT_ASSIGNED)
            n++;
    }

    return !this->isSatisfiable(a) && n == 1;
}

int Clause::isComplete(const vector<int>* a) {
    int nLiterals = literals->size();

    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);
        if (a->at(abs(l)-1) == NOT_ASSIGNED) 
            return l;
    }

    return 0;
}

bool Clause::isSatisfiable(const vector<int>* a) {
    int nLiterals = literals->size();

    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);

        if (l > 0 && a->at(l-1) == ASSIGNED_POSITIVE) 
            return true;
        else if (l < 0 && a->at(-1*l-1) == ASSIGNED_NEGATIVE)
            return true;
    }

    return false;
}

void Clause::print() {
    int nLiterals = literals->size();
    for (int i = 0; i < nLiterals; i++) {
        cout << literals->at(i) << " ";
    }
    cout << endl;
}

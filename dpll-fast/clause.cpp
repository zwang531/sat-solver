#include "clause.h"

#include <iostream>
#include <sstream>
#include <vector>
#include <string>
#include <set>

using namespace std;

Clause::Clause(const vector<int>& a) {
    signature = "";

    literals = new vector<int>();
    signs = new vector<bool>();

    nLiterals = a.size();

    // eliminate duplicate literals
    set<int> singletonLiteralsSet;

    for (int i = 0; i < nLiterals; i++) {
        int size = singletonLiteralsSet.size();
        singletonLiteralsSet.insert(a[i]);

        if (size == singletonLiteralsSet.size()) {
            nLiterals--;
            continue;
        }

        if (a[i] < 0) {
            literals->push_back(-1*a[i]);
            signs->push_back(false);
        } else {
            literals->push_back(a[i]);
            signs->push_back(true);
        }
    }

    this->updateSignature();
}

Clause::~Clause() {
    if (literals && literals->size()) 
        delete literals;
    literals = NULL;

    if (signs && signs->size())
        delete signs;
    signs = NULL;
}

string Clause::getSignature() {
    return signature;
}

void Clause::updateSignature() {
    signature = "";

    for (int i = 0; i < nLiterals; i++) {
        if (!signs->at(i))
            signature += "-";

        stringstream ss;
        ss << literals->at(i);

        signature += ss.str();
        
        if (i < nLiterals-1)
            signature += ",";
    }
}

void Clause::update(const vector<int>* a) {
    vector<int> badLiterals;
    int k = 0;

    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);
        bool sign = signs->at(i);

        if (a->at(l-1) != NOT_ASSIGNED) {
            if ((sign && a->at(l-1) == ASSIGNED_NEGATIVE)
                || (!sign && a->at(l-1) == ASSIGNED_POSITIVE)) {
                badLiterals.push_back(i-k);
                k++;
            }
        }
    }

    for (int i = 0; i < badLiterals.size(); i++) {
        literals->erase(literals->begin()+badLiterals[i]);
        signs->erase(signs->begin()+badLiterals[i]);
        nLiterals--;
    }

    this->updateSignature();
}

bool Clause::isUnitClause() {
    return nLiterals == 1;
}

bool Clause::isComplete(const vector<int>* a) {
    if (nLiterals == 0) 
        return true;

    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);
        if (a->at(l-1) == NOT_ASSIGNED)
            return false;
    }

    return true;
}

bool Clause::isSatisfied(const vector<int>* a) {
    if (nLiterals == 0) 
        return false;

    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);
        bool sign = signs->at(i);

        if (a->at(l-1) == NOT_ASSIGNED) continue;

        if ((sign && a->at(l-1) == ASSIGNED_POSITIVE) 
            || (!sign && a->at(l-1) == ASSIGNED_NEGATIVE))
            return true;
    }

    return false;
}

bool Clause::isSlefConflict() {
    set<int> literalSet;

    for (int i = 0; i < nLiterals; i++) {
        int l = literals->at(i);

        int size = literalSet.size();
        literalSet.insert(l);

        if (size == literalSet.size())
            return true;
    }

    return false;
}

void Clause::print() {
    for(int i = 0; i < nLiterals; i++) {
        string sign = "";
        if (!signs->at(i))
            sign = "-";
        cout << sign << literals->at(i) << " ";
    }
    cout << endl;
}

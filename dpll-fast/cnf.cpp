#include "cnf.h"

#include <algorithm>
#include <iostream>
#include <sstream>
#include <cstring>
#include <vector>
#include <string>
#include <stack>
#include <set>

using namespace std;

Cnf::Cnf(int a) {
	nClauses = 0;
    conflict = false;

    clauses = new vector<Clause*>();
    assignments = new vector<int>(a, NOT_ASSIGNED);

    decisions = new stack<vector<int>*>();
    snapshots = new stack<SCnf*>();
}

Cnf::~Cnf() {
    if(clauses && nClauses) {
        for (int i = 0; i < nClauses; i++) {
            delete clauses->at(i);
        }
        clauses->clear();
    }
    clauses = NULL;

    if (assignments && assignments->size()) 
    	delete assignments;
    assignments = NULL;

    if (decisions && decisions->size()) {
        int i = decisions->size();
        while (i > 0) {
            vector<int>* d = decisions->top();
            decisions->pop();
            delete d;
            d = NULL;
            i--;
        }
        delete decisions;
    }
    decisions = NULL;

    if (snapshots && snapshots->size()) {
        int i = snapshots->size();
        while (i > 0) {
            SCnf* scnf = snapshots->top();
            snapshots->pop();

            if (scnf->signaturesOfClauses)
                delete scnf->signaturesOfClauses;
            scnf->signaturesOfClauses = NULL;

            if (scnf->assignments)
                delete scnf->assignments;
            scnf->assignments = NULL;

            delete scnf;
            i--;
        }
        delete snapshots;
    }
    snapshots = NULL;
}

void Cnf::saveSnapshot() {
    SCnf* scnf = new SCnf();

    scnf->signaturesOfClauses = new vector<string>();
    for (int i = 0; i < nClauses; i++) {
        scnf->signaturesOfClauses->push_back(clauses->at(i)->getSignature());
    }

    scnf->assignments = new vector<int>();
    for (int i = 0; i < assignments->size(); i++) {
        scnf->assignments->push_back(assignments->at(i));
    }

    snapshots->push(scnf);
}

void Cnf::assignLiteral(int l, int v) {
    if (l < 1 || l > assignments->size()) 
        return;

    if (assignments->at(l-1) != NOT_ASSIGNED && assignments->at(l-1) != v) 
        conflict = true;

    assignments->at(l-1) = v;
}

void Cnf::makeDecision() {
    // need some heuristic to decide which literal to pick
    // TODO
    int l = this->isComplete();
    this->assignLiteral(l, ASSIGNED_NEGATIVE);

    vector<int>* decision = new vector<int>();
    decision->push_back(l);
    decision->push_back(ASSIGNED_NEGATIVE);
    decisions->push(decision);
}

void Cnf::updateClauses() {
    vector<int> badClauses;
    int k = 0;

    for (int i = 0; i < nClauses; i++) {
        Clause* c = clauses->at(i);

        c->update(assignments);

        if (c->isComplete(assignments) && !c->isSatisfied(assignments)) {
            conflict = true;
        } else if (c->isSatisfied(assignments)) {
            badClauses.push_back(i-k);
            k++;
        }
    }

    for (int j = 0; j < badClauses.size(); j++)
        this->removeClause(badClauses[j]);
}

void Cnf::preliminaryClauses() {
	vector<int> badClauses;
    set<string> singletonClausesSet;

    int i = 0, k = 0;
    while (i < nClauses) {
        int size = singletonClausesSet.size();

        singletonClausesSet.insert(clauses->at(i)->getSignature());

        if (size == singletonClausesSet.size()) {
            badClauses.push_back(i-k);
            k++;
        }
        else if (clauses->at(i)->isSlefConflict()) {
            badClauses.push_back(i-k);
            k++;
        }

        i++;
    }

    for (int j = 0; j < badClauses.size(); j++)
        this->removeClause(badClauses[j]);

    // pure literals elimination 
    // TODO
}

Clause* Cnf::findUnitClause() {
    Clause* uc = NULL;

    for (int i = 0; i < nClauses; i++) {
        if (clauses->at(i)->isUnitClause()) {
            uc = clauses->at(i);
            break;
        }
    }

    return uc;
}

void Cnf::unitPropagate() {
    Clause* uClause = this->findUnitClause();

    while (uClause) {
        // update assignments
        int l;
        string lStr = uClause->getSignature();
        stringstream ss(lStr);
        ss >> l;

        if (l < 0) {
            this->assignLiteral(-1*l, ASSIGNED_NEGATIVE);
        } else {
            this->assignLiteral(l, ASSIGNED_POSITIVE);
        }

        // remove the unit clause
        this->removeClause(this->findIndexOfClause(uClause));

        // update other clauses with updated assignments(check conflict)
        this->updateClauses();

        uClause = this->findUnitClause();
    }
}

int Cnf::findIndexOfClause(const Clause* c) {
    int index;

    for (int i = 0; i < nClauses; i++) {
        if (clauses->at(i) == c) {
            index = i;
        }
    }

    return index;
}

vector<int> Cnf::backtrack() {
    vector<int> d;

    SCnf* scnf = snapshots->top();
    snapshots->pop();

    this->clearClauses();
    
    int csize = scnf->signaturesOfClauses->size();
    for (int i = 0; i < csize; i++) {
        vector<int> c;
        string signature = scnf->signaturesOfClauses->at(i);

        char* cSignature = new char[signature.length()+1];
        strcpy(cSignature, signature.c_str());

        char* token = NULL;
        token = strtok(cSignature, ",");
        while (token) {
            int l;
            stringstream(token) >> l;
            c.push_back(l);
            token = strtok(NULL, ",");
        }

        Clause* clause = new Clause(c);
        this->addClause(clause);
    }

    int lsize = scnf->assignments->size();
    for (int i = 0; i < lsize; i++) {
        assignments->at(i) = scnf->assignments->at(i);
    }

    if (scnf->signaturesOfClauses)
        delete scnf->signaturesOfClauses;
    scnf->signaturesOfClauses = NULL;

    if (scnf->assignments)
        delete scnf->assignments;
    scnf->assignments = NULL;

    scnf = NULL;

    vector<int>* lastD = decisions->top();
    decisions->pop();
    
    d.push_back(lastD->at(0));
    d.push_back(lastD->at(1));

    delete lastD;
    lastD = NULL;

    conflict = false;

    return d;
}

void Cnf::clearClauses() {
    if(clauses && nClauses) {
        for (int i = 0; i < nClauses; i++) {
            delete clauses->at(i);
        }
    }
    clauses->clear();
    nClauses = 0;
}

void Cnf::addClause(Clause* c) {
    clauses->push_back(c);
    nClauses++;
}

void Cnf::removeClause(int i) {
    if (i >= 0 && i < nClauses) {
        Clause* c = clauses->at(i);
        clauses->erase(clauses->begin()+i);
        delete c;
        c = NULL;
        nClauses--;
    }
}

bool Cnf::checkConfliction() {
    return conflict;
}

bool Cnf::isDecisionStackEmpty() {
    return decisions->size() == 0;
}

int Cnf::isComplete() {
    for (int i = 0; i < assignments->size(); i++) {
        if (assignments->at(i) == NOT_ASSIGNED)
            return i+1;
    }
    return 0;
}

bool Cnf::isSatisfied() {
    if (conflict) 
        return false;
    
    if (nClauses == 0)
        return true;
    
    for (int i = 0; i < nClauses; i++) {
        if (!clauses->at(i)->isSatisfied(assignments))
            return false;
    }

    return true;
}

void Cnf::printClauses() {
	cout << "# of clauses = " << nClauses << endl;
	for (int i = 0; i < nClauses; i++) {
		clauses->at(i)->print();
	}
}

void Cnf::printAssignments() {
	cout << "# of literals = " << assignments->size() << endl;
    for(int i = 0; i < assignments->size(); i++) {
        cout << "l" << i+1 << " = " << assignments->at(i) << " ";
    }
    cout << endl;
}
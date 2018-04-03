#ifndef CNF_H
#define CNF_H

#include "clause.h"
#include "graph.h"

#include <iostream>
#include <algorithm>
#include <cstdlib>
#include <vector>

using namespace std;

#define NOT_ASSIGNED -1
#define ASSIGNED_POSITIVE 1
#define ASSIGNED_NEGATIVE 0


class Cnf {

    bool conflict;

	vector<Clause*>* clauses;
    vector<int>* assignments;

    Graph* graph;

public:
	Cnf(int n);

	~Cnf();

    void addClause(Clause* c);

    Clause* getLastClause();

    void assignLiteral(int l, int b);

    void preliminary(const vector<int>& a);

    int findUnitClause();

    void unitPropagate(int d);

    void makeDecision(int d);

    void backtrack(int d);

    int analyzeAndLearn();

    bool checkConfliction();

    int isComplete();

    bool isSatisfiable();

    void print();

    void printClauses();

    void printAssignments();

    void printDecisionLiterals();
};

#endif
#ifndef CNF_H
#define CNF_H

#include "clause.h"

#include <vector>
#include <stack>

using namespace std;

#define NOT_ASSIGNED -1
#define ASSIGNED_POSITIVE 1
#define ASSIGNED_NEGATIVE 0

struct SCnf {
    vector<string>* signaturesOfClauses;
    vector<int>* assignments;
};


class Cnf {
	int nClauses;
    bool conflict;

	vector<Clause*>* clauses;
    vector<int>* assignments;

    stack<vector<int>*>* decisions;
    stack<SCnf*>* snapshots;

public:
	Cnf(int a);

	~Cnf();

    void assignLiteral(int l, int v);

    void saveSnapshot();

    void makeDecision();

    void updateClauses();

    void preliminaryClauses();

    Clause* findUnitClause();

    void unitPropagate();

    int findIndexOfClause(const Clause* c);

    vector<int> backtrack();

    void clearClauses();

    void addClause(Clause* c);

    void removeClause(int i);

    bool checkConfliction();

    bool isDecisionStackEmpty();

    int isComplete();

    bool isSatisfied();

    void printClauses();

    void printAssignments();
};

#endif
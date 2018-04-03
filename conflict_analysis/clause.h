#ifndef CLAUSE_H
#define CLAUSE_H

#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

#define NOT_ASSIGNED -1
#define ASSIGNED_POSITIVE 1
#define ASSIGNED_NEGATIVE 0


class Clause {

	vector<int>* literals;

public:
	Clause(const vector<int>& a);

	~Clause();

    vector<int> getLiterals();

    vector<int> getAssignedLiterals(const vector<int>* a);

    bool isUnitClause(const vector<int>* a);

    int isComplete(const vector<int>* a);

    bool isSatisfiable(const vector<int>* a);

    void print();
};

#endif
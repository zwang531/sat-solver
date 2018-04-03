#ifndef CLAUSE_H
#define CLAUSE_H

#include <string>
#include <vector>

using namespace std;

#define NOT_ASSIGNED -1
#define ASSIGNED_POSITIVE 1
#define ASSIGNED_NEGATIVE 0


class Clause {
	int nLiterals;
    string signature;

	vector<int>* literals;
	vector<bool>* signs;

public:
	Clause(const vector<int>& a);

	~Clause();

    string getSignature();

    void updateSignature();

    void update(const vector<int>* a);

    bool isUnitClause();

    bool isComplete(const vector<int>* a);

    bool isSatisfied(const vector<int>* a);

    bool isSlefConflict();

	void print();
};

#endif
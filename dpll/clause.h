#ifndef CLAUSE_H
#define CLAUSE_H

#include <iostream>
#include <vector>
using namespace std;

class Clause {
    vector<int> *literals;

public:
    Clause(const vector<int>&);

    ~Clause();

    bool isSatisfiable(const vector<bool>&);

    void print();
};


#endif
#include "clause.h"
#include "cnf.h"
#include "solver.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

using namespace std;

#define TIMEOUT 2
#define SAT 1
#define UNSAT 0


int main(int argc, char* argv[]) {
	if (argc < 2) return 1;

    ifstream file(argv[1]);
    if (!file.is_open()) return 1;

    Cnf* cnf = NULL;
	int nLiterals = 0, nClauses = 0;

    string line;
    bool isHeaderSeen = false;
    while (getline(file, line)) {
    	if (!line.empty()) {
    		if (isHeaderSeen && nClauses == 0) break;

    		// ignore comment line
            if (line[0] == 'c') continue;

            char* cLine = new char[line.length()+1];
            strcpy(cLine, line.c_str());

            if (line[0] == 'p' && !isHeaderSeen) {
            	isHeaderSeen = true;

            	int i = 0;
            	char* token = NULL;
                token = strtok(cLine, " ");
                while (token) {
                	stringstream ss(token);

                	if (i == 2) {
                		ss >> nLiterals;
                	} else if (i == 3) {
                		ss >> nClauses;
                	}

                    token = strtok(NULL, " ");
                    i++;
                }

                if (nLiterals > 0 && nClauses > 0) 
                	cnf = new Cnf(nLiterals);

                delete[] cLine;
                continue;
            }

            if (isHeaderSeen && nLiterals > 0 && nClauses > 0) {
            	vector<int> literals;

            	char* token = NULL;
            	token = strtok(cLine, " ");

            	while (token) {
            		int l = atoi(token);
            		if (l == 0) break;
            		literals.push_back(l);
            		token = strtok(NULL, " ");
            	}

            	Clause* c = new Clause(literals);
            	cnf->addClause(c);

            	nClauses--;
            }

            delete[] cLine;
        }
    }

    if (cnf) {
    	Solver* solver = new Solver(cnf);

		srand(time(NULL));

        clock_t start = clock();

        int satisfiable = solver->dpll(start);
    	if (satisfiable == SAT) 
    		cout << "SAT" << endl;
    	else if (satisfiable == UNSAT)
    		cout << "UNSAT" << endl;
    	else 
    		cout << "TIMEOUT" << endl;

    	// solver->print();

    	delete solver;
    }
}	
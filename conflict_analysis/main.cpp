#include "clause.h"
#include "cnf.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

using namespace std;


int main(int argc, char* argv[]) {
	if (argc < 2) 
		return 1;

    ifstream file(argv[1]);
    if (!file.is_open()) 
    	return 1;

	int nLiterals = 0, nClauses = 0;

	vector<int> counts;
	vector<bool> signs;

	Cnf* cnf = NULL;

    string line;
    bool isHeaderSeen = false;

    while (getline(file, line)) {
    	if (!line.empty()) {
    		if (isHeaderSeen && nClauses == 0) break;

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

                if (nLiterals > 0 && nClauses > 0) {
                	counts.resize(nLiterals, 0);
                	signs.resize(nLiterals, true);

                	cnf = new Cnf(nLiterals);
                }

                delete[] cLine;
                continue;
            }

            if (isHeaderSeen && nLiterals > 0 && nClauses > 0) {
            	vector<int> literals;

            	char* token = NULL;
            	token = strtok(cLine, " ");

            	while (token) {
            		int l;
            		stringstream(token) >> l;

            		if (l == 0) break;

            		counts[abs(l)-1]++;
            		if (l < 0) 
            			signs[abs(l)-1] = false;

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

    if (!cnf) 
    	return 1;

    srand(time(NULL));

    clock_t start = clock();

    vector<int> pureLiterals;
    for (int i = 0; i < nLiterals; i++) {
    	if (counts[i] == 1 && signs[i]) 
    		pureLiterals.push_back(i+1);
    	else if (counts[i] == 1 && !signs[i])
    		pureLiterals.push_back(-1*(i+1));
    }

  	cnf->preliminary(pureLiterals);
  	// cnf->print();

    int d = 0;

    while (true) {
        double duration = (clock() - start) /(double) CLOCKS_PER_SEC;
        if (duration >= 30.0) {
            cout << "TIMEOUT" << endl;
            break;
        }

    	cnf->unitPropagate(d);
    	// cnf->printAssignments();
    	// cout << cnf->checkConfliction() << endl;
    	if (cnf->checkConfliction()) { 
    		d = cnf->analyzeAndLearn();
break;
    		if (d < 0) {
    			cout << "UNSAT" << endl;
    			break;
    		} else {
    			cnf->backtrack(d);
    		}

    	} else {
    		if (cnf->isSatisfiable()) {
    			cout << "SAT" << endl;
    			break;
    		} else {
    			d++;
    			cnf->makeDecision(d);
    		}
    	}
    }

    delete cnf;
    return 0;
}
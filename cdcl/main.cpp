#include "clause.h"
#include "cnf.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <csignal>
#include <cstring>
#include <string>
#include <vector>
#include <ctime>

using namespace std;


bool timeoutMode = false;
bool verboseMode = false;

double limit = 0.;

Cnf* cnf = NULL;

void signalHandler(int signum) {
    cout << "\nunknown" << endl;
    if (verboseMode) 
        cnf->printDecisionLiterals();
    if (cnf) 
        delete cnf;
    exit(signum);
}

int main(int argc, char* argv[]) {
	if (argc < 2) {
        cout << "missing the input CNF file.\n";
		return 1;
    } 

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cout << "failed to open the file.\n";
        return 1;
    }

    if (argc > 2) {
        int i = 2;

        while (i < argc) {
            if (verboseMode && timeoutMode) {
                cout << "invalid argument" << endl;
                return -1;
            }

            if (!timeoutMode && strcmp(argv[i], "--time") == 0) {
                timeoutMode = true;
                if (i+1 == argc) {
                    cout << "missing timeout" << endl;
                    return -1;
                }
                i++;

                stringstream ss(argv[i]);
                ss >> limit;
                if (limit <= 0) {
                    cout << "invalid timeout" << endl;
                    return -1;
                }

            } else if (!verboseMode && strcmp(argv[i], "--verbose") == 0) {
                verboseMode = true;
            } else {
                cout << "invalid argument" << endl;
                return -1;
            }

            i++;
        }
    }

	int nLiterals = 0, nClauses = 0;

    // for purpose of pure literal elimination 
	vector<int> counts;
	vector<bool> signs;

	// Cnf* cnf = NULL;

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

                    if (i == 1 && ss.str().compare("cnf") != 0) {
                        cout << "wrong DIMACS format\n";
                        delete[] cLine;
                        return 1;
                    }

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

                } else {
                    cout << "wrong DIMACS format\n";
                    delete[] cLine;
                    return 1;
                }

                delete[] cLine;
                continue;
            }

            if (isHeaderSeen) {
            	vector<int> literals;

            	char* token = NULL;
            	token = strtok(cLine, " ");

                int l;
                bool exceedBound = false;
            	while (token) {
            		stringstream ss(token);
                    ss >> l;

            		if (l == 0) break;
                    else if (abs(l) > nLiterals) {
                        exceedBound = true;
                        break;
                    }

            		counts[abs(l)-1]++;
            		if (l < 0) 
            			signs[abs(l)-1] = false;

            		literals.push_back(l);
            		token = strtok(NULL, " ");
            	}

                if (exceedBound || l != 0 || !literals.size()) {
                    cout << exceedBound << l << literals.size() << endl;
                    cout << line << endl;
                    cout << "wrong DIMACS format\n";
                    if (cnf)
                        delete cnf;
                    delete[] cLine;
                    return 1;
                }

            	Clause* c = new Clause(literals);
            	cnf->addClause(c);

            	nClauses--;

            } else {
                cout << "wrong DIMACS format\n";
                delete[] cLine;
                return 1;
            }

            delete[] cLine;
        }
    }

    // if (!cnf) 
    // 	return 1;
    if (!isHeaderSeen) {
        cout << "wrong DIMACS format\n";
        if (cnf)
            delete cnf;
        return -1;
    }

    signal(SIGINT, signalHandler);  

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
        if (timeoutMode && duration >= limit) {
            cout << "unknown" << endl;
            if (verboseMode) 
                cnf->printDecisionLiterals();
            break;
        }

    	cnf->unitPropagate(d);
    	// cnf->printAssignments();
    	// cout << cnf->checkConfliction() << endl;
    	if (cnf->checkConfliction()) { 
    		d = cnf->analyzeAndLearn();

    		if (d < 0) {
    			cout << "unsat" << endl;
                if (verboseMode)
                    cnf->getLastClause()->print();
    			break;
    		} else {
    			cnf->backtrack(d);
    		}

    	} else {
    		if (cnf->isSatisfiable()) {
    			cout << "sat" << endl;
                if (verboseMode) {
                    while (cnf->isComplete()) {
                        cnf->assignLiteral(cnf->isComplete(), 1);
                    }
                    cnf->printAssignments();
                }
    			break;
    		} else {
    			d++;
    			cnf->makeDecision(d);
    		}
    	}
    }

    if (cnf)
        delete cnf;
    return 0;
}
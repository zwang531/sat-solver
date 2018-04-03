#include "clause.h"
#include "solver.h"

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <cstdio>
#include <ctime>
#include <cstring>
#include <string>
#include <vector>
#include <map>
#include <cmath>

using namespace std;

int main(int argc, char *argv[]) {
    if (argc < 2) {
        cout << "missing input cnf file" << endl;
        return 1;
    }

    ifstream file(argv[1]);
    if (!file.is_open()) {
        cout << "file not found" << endl;
        return 1;
    }

    int nLiterals = 0, nClauses = 0;

    Solver *solver = NULL;

    string line;
    while (getline(file, line)) {
        if (!line.empty()) {
            if (line[0] == 'c') {
                continue;
            }

            char* cline = new char[line.length()+1];
            strcpy(cline, line.c_str());

            if (line[0] == 'p') {
                int i = 0;
                char *token[4] = {};

                token[i] = strtok(cline, " ");
                while (token[i]) {
                    i++;
                    token[i] = strtok(NULL, " ");
                }

                if (token[2] && token[3]) {
                    stringstream ss1(token[2]);
                    ss1 >> nLiterals;

                    stringstream ss2(token[3]);
                    ss2 >> nClauses;

                    if (!solver) {
                        solver = new Solver(nLiterals, nClauses);
                    } 
                }
                else {
                    cout << "wrong format" << endl;
                    delete[] cline;
                    return 1;
                }
            }
            else {
                vector<int> literals;

                char *token;
                token = strtok(cline, " ");

                while (token) {
                    int literal;

                    stringstream ss(token);
                    ss >> literal;

                    if (!literal) break;

                    literals.push_back(literal);

                    token = strtok(NULL, " ");
                }
                if (solver) {
                    solver->addClause(literals);
                }
            }

            delete[] cline;
        }        
    }

    srand(time(NULL));

    clock_t start = clock();
    double duration = 0.;

    map<string, bool> book;
    map<string, bool>::iterator it;

    while (true) {
        duration = (clock() - start) /(double) CLOCKS_PER_SEC;
        if (duration >= 10.) {
            break;
        }

        string assignment = "";

        vector<bool> assignments(nLiterals, false);
        for (int i = 0; i < nLiterals; i++) {
            int num = rand() % 10 + 1;
            if (num > 5) {
                assignments[i] = true;
                assignment += '1';
            }
            else {
                assignment += '0';
            }
        }

        // debug
        // cout << solver->isSatisfiable(assignments) << " " << assignment << endl;

        it = book.find(assignment);
        if (book.empty() || it == book.end()) {
            book[assignment] = solver->isSatisfiable(assignments);
            if (book[assignment]) {
                cout << "sat\n";
                cout << assignment << endl;
                return 0;
            }
        }
    }

    if ((int)log2(book.size()) == nLiterals) {
        cout << "unsat" << endl;
    }
    else {
        cout << "unknown\n";
        cout << book.size() << "/(2^" << nLiterals << ")\n";
    }

    if (solver) {
        // debug
        // solver->print();
        delete solver;
    }

    return 0;
}


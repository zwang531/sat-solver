#include "cnf.h"

Cnf::Cnf(int n) {
    conflict = false;;

    clauses = new vector<Clause*>();
    assignments = new vector<int>(n, NOT_ASSIGNED);

    graph = new Graph(n);
}

Cnf::~Cnf() {
    if (clauses && clauses->size()) {
        int nClauses = clauses->size();
        for (int i = 0; i < nClauses; i++) {
            delete clauses->at(i);
        }
    }
    delete clauses;

    if (assignments && assignments->size()) 
        delete assignments;

    if (graph)
        delete graph;
}

void Cnf::addClause(Clause* c) {
    clauses->push_back(c);
}

Clause* Cnf::getLastClause() {
    return clauses->at(clauses->size()-1);
}

void Cnf::assignLiteral(int l, int b) {
    if (l < 1 || l > assignments->size()) 
        return;

    // if (assignments->at(l-1) != NOT_ASSIGNED && assignments->at(l-1) != b) 
    //     conflict = true;
    // else 
        assignments->at(l-1) = b;
}

void Cnf::preliminary(const vector<int>& a) {
    int n = a.size();

    for (int i = 0; i < n; i++) {
        if (a[i] < 0) {
            this->assignLiteral(-1*a[i], ASSIGNED_NEGATIVE);
        } else {
            this->assignLiteral(a[i], ASSIGNED_POSITIVE);
        }
    }
}

int Cnf::findUnitClause() {
    int c = -1;
    int nClauses = clauses->size();

    for (int i = 0; i < nClauses; i++) {
        if (clauses->at(i)->isUnitClause(assignments)) {
            c = i;
            break;
        }
    }

    return c;
}

void Cnf::unitPropagate(int d) {
    int c = this->findUnitClause();

    while (c >= 0) {
        int l = clauses->at(c)->isComplete(assignments);

        vector<int> a = clauses->at(c)->getAssignedLiterals(assignments);

        if (l < 0) {
            this->assignLiteral(-1*l, ASSIGNED_NEGATIVE);
        } else {
            this->assignLiteral(l, ASSIGNED_POSITIVE);
        }

        // cout << abs(l) << " test " << d << endl;
        graph->addNode(abs(l), d, a);

        Clause* clause;
        int nClauses = clauses->size();
        for (int i = 0; i < nClauses; i++) {
        	clause = clauses->at(i);
        	if (clause->isComplete(assignments) == 0 && !clause->isSatisfiable(assignments)) {
        		conflict = true;
        		// clause->print();
        		// cout << d << endl;
        		break;
        	}
        }

        // graph->addNode(abs(l), d, a);

        if (conflict) {
        	vector<int> b(clause->getAssignedLiterals(assignments));
        	b.insert(b.begin(), d);

        	graph->setReasonQueue(b);
        	return;
        } 
        // else {
        // 	cout << abs(l) << " test " << d << endl;
        // 	graph->addNode(abs(l), d, a);
        // }

        // if (conflict) {
        //     graph->setConflictNode(abs(l), d, a);
        //     return;
        // } else {
        //     graph->addNode(abs(l), d, a);
        // }

        c = this->findUnitClause();
    }
}

void Cnf::makeDecision(int d) {
    int l = this->isComplete();
    this->assignLiteral(l, ASSIGNED_NEGATIVE);

    vector<int> a;
    graph->addNode(l, d, a);
}

void Cnf::backtrack(int d) {
    graph->backtrack(d, assignments);
    conflict = false;
}

int Cnf::analyzeAndLearn() {
    vector<int> a = graph->findCut();

    int d = a[0]; 
    if (d < 0)
        return d;

    vector<int> b(a.begin()+1, a.end());

    int n = b.size();
    for (int i = 0; i < n; i++) {
        if (assignments->at(b[i]-1) == ASSIGNED_POSITIVE)
            b[i] *= -1;
    }

    Clause* c = new Clause(b);
    this->addClause(c);

    vector<int> levels;
    for (int i = 0; i < n; i++) {
        int level = graph->getLevelOfLiteral(abs(b[i]));
        levels.push_back(level);
    }

    sort(levels.begin(), levels.end());
    
    int secondHighestLevel = 0;
    if (n > 1)
        secondHighestLevel = levels[n-2];

    return secondHighestLevel;
}

bool Cnf::checkConfliction() {
    return conflict;
}

int Cnf::isComplete() {
    int nLiterals = assignments->size();
    for (int i = 0; i < nLiterals; i++) {
        if (assignments->at(i) == NOT_ASSIGNED) {
            return i+1;
        }
    }
    return 0;
}

bool Cnf::isSatisfiable() {
    int nClauses = clauses->size();
    for (int i = 0; i < nClauses; i++) {
        if (!clauses->at(i)->isSatisfiable(assignments))
            return false;
    }
    return true;
}

void Cnf::print() {
    this->printClauses();
    this->printAssignments();
}

void Cnf::printClauses() {
    int nClauses = clauses->size();
    // cout << "# of clauses = " << nClauses << endl;
    for (int i = 0; i < nClauses; i++) {
        clauses->at(i)->print();
    }
}

void Cnf::printAssignments() {
    int nLiterals = assignments->size();
    // cout << "# of literals = " << nLiterals << endl;
    for(int i = 0; i < nLiterals; i++) {
        int sign = 1;
        if (assignments->at(i) == ASSIGNED_NEGATIVE)
            sign = -1;
        cout << sign*(i+1) << " ";
        // cout << "v" << i+1 << "=" << assignments->at(i) << " ";
    }
    cout << endl;
}

void Cnf::printDecisionLiterals() {
    vector<int> literals = graph->getDecisionLiterals();
    int nLiterals = literals.size();
    for (int i = 0; i < nLiterals; i++) {
        int l = literals[i];
        int sign = 1;
        if (assignments->at(i) == ASSIGNED_NEGATIVE)
            sign = -1;
        cout << sign*l << " ";
    }
    cout << endl;
}

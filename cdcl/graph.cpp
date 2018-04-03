#include "graph.h"

Graph::Graph(int n) {
	// conflictNode = NULL;
	reasonQueue = NULL;

	vertices = new vector<Vertex*>(n, NULL);

	nodeStack = new stack<int>();
}

Graph::~Graph() {
	// if (conflictNode) {
	// 	if (conflictNode->parents && conflictNode->parents->size())
	// 		delete conflictNode->parents;
	// 	delete conflictNode;
	// }

	if (vertices && vertices->size()) {
		int n = vertices->size();

		for (int i = 0; i < n; i++) {
			Vertex* v = vertices->at(i);
			if (v) {
				if (v->parents && v->parents->size())
					delete v->parents;
				delete v;
			}
		}
		delete vertices;
	}

	if (nodeStack && nodeStack->size())
		delete nodeStack;

	if (reasonQueue && reasonQueue->size())
		delete reasonQueue;
}

void Graph::addNode(int l, int d, const vector<int>& a) {
	Vertex* v = new Vertex();

	v->l = l; 
	v->d = d;

	if (a.size() == 0) 
		v->parents = NULL;
	else 
		v->parents = new vector<int>(a);

	vertices->at(l-1) = v;
	nodeStack->push(l-1);
}

void Graph::setReasonQueue(const vector<int>& a) {
	reasonQueue = new vector<int>(a);
}

// void Graph::setConflictNode(int l, int d, const vector<int>& a) {
// 	conflictNode = new Vertex();

// 	conflictNode->l = l; 
// 	conflictNode->d = d;

// 	if (a.size() == 0) 
// 		conflictNode->parents = NULL;
// 	else 
// 		conflictNode->parents = new vector<int>(a);
// }

vector<int> Graph::findCut() {
	// int l = conflictNode->l;
	// int d = conflictNode->d;
	int d = reasonQueue->at(0);

	vector<int> a;

	if (d == 0) {
		a.push_back(-1);
		return a;
	}

	int k = 0;
	queue<int> q;

	for (int i = 1; i < reasonQueue->size(); i++) {
		int l = reasonQueue->at(i);
		if (vertices->at(l-1)->d == d)
			k++;
		q.push(l);
	}

	while (true) {
		if (k == 1) {
			if (q.size() > 1) {
				a.push_back(d-1);
				while (q.size()) {
					a.push_back(q.front());
					q.pop();
				}
			} else if (q.size() == 1) {
				a.push_back(0);
				a.push_back(q.front());
				q.pop();
			}
			break;

		} else if (k > 1) {
			while (true) {
				int p = q.front();
				q.pop();

				vector<int>* par = vertices->at(p-1)->parents;

				if (vertices->at(p-1)->d < d)
					q.push(p);
				else if (par == NULL || par->size() == 0)
					q.push(p);
				else {
					vector<int> b;
					set<int> s;

					while (q.size()) {
						s.insert(q.front());
						b.push_back(q.front());
						q.pop();
					}

					for (int i = 0; i < par->size(); i++) {
						int size = s.size();
						s.insert(par->at(i));

						if (s.size() > size) {
							q.push(par->at(i));
							if (vertices->at(par->at(i)-1)->d == d)
								k++;
						}
					}

					for (int i = 0; i < b.size(); i++) 
						q.push(b[i]);

					k--;
					break;
				}
			}
		} 
	}

	return a;
}

void Graph::backtrack(int d, vector<int>* a) {
	// int d = conflictNode->d;
	// stack<int> q;

	while (nodeStack->size()) {
		int i = nodeStack->top();
		// nodeStack->pop();

		if (vertices->at(i)->d <= d) break;

		nodeStack->pop();
		// if (vertices->at(i)->d == d) {
			int l = vertices->at(i)->l;
			a->at(l-1) = NOT_ASSIGNED;

			if (vertices->at(i)->parents && vertices->at(i)->parents->size())
				delete vertices->at(i)->parents;
			delete vertices->at(i);
			vertices->at(i) = NULL;
		// } 
		// else {
		// 	q.push(i);
		// }	
	}

	// if (q.size()) {
	// 	nodeStack->push(q.top());
	// 	q.pop();
	// }

	delete reasonQueue;
	reasonQueue = NULL;

	// if (conflictNode) {
	// 	if (conflictNode->parents && conflictNode->parents->size())
	// 		delete conflictNode->parents;
	// 	delete conflictNode;
	// }
}

int Graph::getLevelOfLiteral(int l) {
	return vertices->at(l-1)->d;
}

vector<int> Graph::getDecisionLiterals() {
	vector<int> decisionLiterals;
	int nLiterals = vertices->size();
	for (int i = 0; i < nLiterals; i++) {
		if (vertices->at(i) && !vertices->at(i)->parents) {
			decisionLiterals.push_back(vertices->at(i)->l);
		}
	}
	return decisionLiterals;
}
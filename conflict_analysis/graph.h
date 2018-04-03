#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <cstdlib>
#include <vector>
#include <stack>
#include <queue>
#include <set>

using namespace std;

#define NOT_ASSIGNED -1
#define ASSIGNED_POSITIVE 1
#define ASSIGNED_NEGATIVE 0


struct Vertex {
	int l, d;

	vector<int>* parents;
};


class Graph {
	// Vertex* conflictNode;
	vector<int>* reasonQueue;

	vector<Vertex*>* vertices;
	stack<int>* nodeStack;

public:
	Graph(int n);

	~Graph();

	void addNode(int l, int d, const vector<int>& a);

	void setReasonQueue(const vector<int>& a);

	// void setConflictNode(int l, int d, const vector<int>& a);

	vector<int> findCut();

	void backtrack(int d, vector<int>* a);

	int getLevelOfLiteral(int l);
};

#endif
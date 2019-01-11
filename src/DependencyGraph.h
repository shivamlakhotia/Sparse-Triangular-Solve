#ifndef DEPENDENCYGRAPH_H
#define DEPENDENCYGRAPH_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <chrono>

using namespace std;

class DependencyGraph
{
  private:
    map<int, unordered_set<int>> graph;

  public:
    DependencyGraph();
    void insertEdge(int i, int j);
    void traverseDG();
    void findReachable(unordered_set<int> seed, unordered_set<int> &visited);
};

#endif
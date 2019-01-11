#ifndef IO_H
#define IO_H

#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <map>
#include <unordered_set>
#include <vector>
#include <queue>
#include <chrono>
#include <omp.h>
#include "DependencyGraph.h"

using namespace std;
using namespace std::chrono;

// code referred from http://www.cplusplus.com/forum/general/65804/
int readMatrix(int &order, int *&Lp, int *&Li, double *&Lx);
int readRHS(const int order, double *&B);
int print(int N, const double *x);
void printExecutionTime(std::chrono::_V2::system_clock::time_point start, std::chrono::_V2::system_clock::time_point end, string msg);

#endif
/**
 * UoT, sts.cpp
 * Purpose: Optimize Sparse Triangular Solve
 * 
 * @author Shivam Lakhotia
 * @version 1.0 10/01/2019
 * 
 * References: 
 * Execution-time logging: https://www.geeksforgeeks.org/measure-execution-time-function-cpp/
 * Reading .mtx format: http://www.cplusplus.com/forum/general/65804/
 * 
 **/

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
#include "IO.h"

using namespace std;
using namespace std::chrono;

/**
 * Lower triangular solver Lx=b
 * L is stored in the compressed column storage format
 * Inputs are:
 * n : the matrix dimension
 * Lp : the column pointer of L
 * Li : the row index of L
 * Lx : the values of L
 * In/Out:
 * x : the right hand-side b at start and the solution x at the end.
 */
int lsolve(int n, int *Lp, int *Li, double *Lx, double *x)
{
    cout << "Without Optimization\n";

    //x is initilized with b
    int p, j, count = 0;
    if (!Lp || !Li || !x)
        return (0);
    /* check inputs */

    //j is column
    //x[j] = b[j]
    //Lp[j] + 1 = starting index of nz elements in jth column
    //total number of nz elements in jth column is Lp[j+1] - Lp[j]
    //Lx[p] = the actual value itself
    for (j = 0; j < n; j++)
    {
        // count = 0;
        if (x[j])
        {
            x[j] /= Lx[Lp[j]];
            for (p = Lp[j] + 1; p < Lp[j + 1]; p++)
            {
                x[Li[p]] -= Lx[p] * x[j];
            }
        }
    }
    return 0;
}

int lsolve_optimized(int n, int *Lp, int *Li, double *Lx, double *x, vector<int> &reachSet)
{
    //x is initilized with b

    cout << "With Optimization\n";
    int p, j;
    if (!Lp || !Li || !x)
        return (0);
    /* check inputs */

    //j is column
    //x[j] = b[j]
    //Lp[j] + 1 = starting index of nz elements in jth column
    //total number of nz elements in jth column is Lp[j+1] - Lp[j]
    //Lx[p] = the actual value itself

    int size = reachSet.size();

    for (int i = 0; i < size; i++)
    // for (vector<int>::iterator it = reachSet.begin(); it != reachSet.end(); it++)
    {
        j = reachSet[i];
        if (x[j])
        {
            x[j] /= Lx[Lp[j]];
            for (p = Lp[j] + 1; p < Lp[j + 1]; p++)
            {
                x[Li[p]] -= Lx[p] * x[j];
            }
        }
    }
    return 0;
}


void buildDependencyGraph(int n, const int *Lp, const int *Li, DependencyGraph &dg)
{
// #pragma omp parallel
// #pragma omp for
    for (int i = 0; i < n; i++)
    {
        for (int j = Lp[i]; j < Lp[i + 1]; j++)
        {
            dg.insertEdge(Li[j], i);
        }
    }

    // dg.traverseDG();
}

void buildSeedSet(int order, const double *x, unordered_set<int> &seedSet)
{
    for (int i = 0; i < order; i++)
    {
        if (x[i])
            seedSet.insert(i);
    }

    cout << "\nSize of Seed Set is: " << seedSet.size() << "\n";
    cout << "Seed Set:\n";

    for (unordered_set<int>::iterator it = seedSet.begin(); it != seedSet.end(); it++)
        cout << *it << " ";
    cout << "\n";

    return;
}



int main()
{
    int n;
    double *x;
    int *Lp;
    int *Li;
    double *Lx;
    unordered_set<int> seedSet;
    unordered_set<int> reachSet;
    seedSet.clear();
    reachSet.clear();
    DependencyGraph dg = DependencyGraph();

    // cout << "Hello World!\n\n";

    readMatrix(n, Lp, Li, Lx);
    readRHS(n, x);

    // Get starting timepoint
    auto startTime = high_resolution_clock::now();

    buildDependencyGraph(n, Lp, Li, dg);

    auto graphBuiltTime = high_resolution_clock::now();

    buildSeedSet(n, x, seedSet);

    auto startTime2 = high_resolution_clock::now();

    dg.findReachable(seedSet, reachSet);

    vector<int> reachList(reachSet.begin(), reachSet.end());

    sort(reachList.begin(), reachList.end());

    auto reachSetTime = high_resolution_clock::now();

    // lsolve(n, Lp, Li, Lx, x);

    lsolve_optimized(n, Lp, Li, Lx, x, reachList);

    auto stopTime = high_resolution_clock::now();

    printExecutionTime(startTime, graphBuiltTime, "build the graph");
    printExecutionTime(startTime2, reachSetTime, "find reachability set");
    printExecutionTime(reachSetTime, stopTime, "solve Lx = B");

    // print(n, x);
    // readMatrix();
    return 0;
}
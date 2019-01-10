#include <iostream>
#include <fstream>
#include <algorithm>
#include <assert.h>
#include <map>
#include <unordered_set>
#include <vector>
#include <queue>

using namespace std;

class DependencyGraph
{
  private:
  public:
    map<int, unordered_set<int>> graph;
    DependencyGraph()
    {
        this->graph = map<int, unordered_set<int>>();
    }

    void insertEdge(int i, int j)
    {
        map<int, unordered_set<int>>::iterator it;
        it = this->graph.find(j);
        if (it != this->graph.end())
        {
            (it->second).insert(i);
        }
        else
        {
            unordered_set<int> dependencySet = unordered_set<int>();
            dependencySet.insert(i);
            this->graph.insert(pair<int, unordered_set<int>>(j, dependencySet));
        }
        return;
    }

    void traverseDG()
    {
        cout << "Dependency Graph:\n";
        map<int, unordered_set<int>>::iterator it;
        for (it = this->graph.begin(); it != this->graph.end(); it++)
        {
            // cout << it->first << "\n";
            unordered_set<int>::iterator it2;

            for (it2 = (it->second).begin(); it2 != (it->second).end(); it2++)
            {
                cout << it->first << "-->" << *it2 << "\n";
            }
            cout << "\n";
        }
    }

    void findReachable(unordered_set<int> seed, unordered_set<int> &visited)
    {
        cout << "Finding reachability set now\n";
        int head = -1;
        unordered_set<int>::iterator seedIt;
        while (!seed.empty())
        {
            seedIt = seed.begin();
            head = *seedIt;
            seed.erase(seedIt);

            // cout << head << " is reachable\n";
            visited.insert(head);

            map<int, unordered_set<int>>::iterator it;
            unordered_set<int>::iterator it2;

            it = (this->graph).find(head);

            if (it != (this->graph).end())
            {
                for (it2 = (it->second).begin(); it2 != (it->second).end(); it2++)
                {
                    if (visited.find(*it2) == visited.end())
                    {
                        seed.insert(*it2);
                    }
                }
            }
        }
    }
};

// code referred from http://www.cplusplus.com/forum/general/65804/
int readMatrix(int &order, int *&Lp, int *&Li, double *&Lx)
{
    // Open the file:
    ifstream fin("../res/af_0_k101/af_0_k101.mtx");
    // ifstream fin("../res/unit_test/L.mtx");

    // Declare variables:
    int M, N, NNZ;

    DependencyGraph dg = DependencyGraph();

    // Ignore headers and comments:
    while (fin.peek() == '%')
        fin.ignore(2048, '\n');

    // Read defining parameters:
    fin >> M >> N >> NNZ;

    order = M;
    Lp = new int[M + 1];      // column pointer
    Li = new int[NNZ + 1];    // row index
    Lx = new double[NNZ + 1]; // non zero

    // initilize all the values with zero (will increase latency)
    // fill(Lp, Lp + M, 0);
    // fill(Li, Li + NNZ, 0);
    // fill(Lx, Lx + NNZ, 0.);

    // Create your matrix:
    // Memory footprint of this program should be around 100 MB.
    // Size of Lx should be around 70 MB.
    // Size of Lp and Li should be around 2 MB each.
    // Verify this using resourse monitor.

    // initilize the starting points.
    int currCol = -1;
    // Lp[0] = 0;

    // Read the data and create the matrix in CSC format
    for (int l = 0; l < NNZ; l++)
    {
        // getting data
        int m, n;
        double data;
        fin >> m >> n >> data;
        
        // maintaining zero starting index
        dg.insertEdge(m, n);
        m--;
        n--;
        
        

        if (n != currCol)
        {
            Lp[n] = l; // if we are traversing the next column mark its starting
            currCol = n;
        }

        Lx[l] = data; // nz data
        Li[l] = m;    // row of the lth nz data point
    }

    cout << "Dependency Graph Built\n"; 

    Lp[N] = NNZ;  // only this should be required
    Li[NNZ] = -1; // JSICS
    Lx[NNZ] = -1; // JSICS
    // dg.traverseDG();

    unordered_set<int> seed;
    seed.clear();
    seed.insert(5);
    unordered_set<int> reachable;
    reachable.clear();

    dg.findReachable(seed, reachable);

    cout << "Reachable set size: " << reachable.size() << "\n" ;
    // unordered_set<int>::iterator it;
    // for(it = reachable.begin(); it != reachable.end(); it++)
    //     cout << *it << " \n";

    fin.close();

    return 0;
}

int readRHS(const int order, double *&B)
{
    // Open the file:
    ifstream fin("../res/af_0_k101/af_0_k101_b.mtx");
    // ifstream fin("../res/unit_test/B.mtx");

    // Declare variables:
    int M, N;

    // Ignore headers and comments:
    while (fin.peek() == '%')
        fin.ignore(2048, '\n');

    // Read defining parameters:
    fin >> M >> N;

    assert(order == M);

    B = new double[M + 2];

    // Read the data and create the matrix in CSC format
    for (int l = 0; l < M; l++)
    {
        // getting data
        // double data;
        // fin >> data;
        fin >> B[l];
    }
    B[M] = -1; // JSICS

    fin.close();

    return 0;
}

/*
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
    //x is initilized with b
    int p, j;
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

int print(int N, const double *x)
{
    cout << "Solving Lx+B gives: \n";
    for (int i = 0; i < N; i++)
    {
        cout << "x[" << i << "] = " << *(x + i) << "\n";
    }
    return 0;
}

int main()
{
    int n;
    double *x;
    int *Lp;
    int *Li;
    double *Lx;
    // double *B;
    cout << "Hello World!\n\n";

    readMatrix(n, Lp, Li, Lx);
    readRHS(n, x);

    // lsolve(n, Lp, Li, Lx, x);
    // print(n, x);
    // readMatrix();
    return 0;
}
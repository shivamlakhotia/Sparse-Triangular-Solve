#include "IO.h"


using namespace std;
using namespace std::chrono;

// code referred from http://www.cplusplus.com/forum/general/65804/
int readMatrix(int &order, int *&Lp, int *&Li, double *&Lx)
{

    cout << "Reading the LHS matrix from file.\n";

    // Open the file:
    ifstream fin("../res/af_0_k101/af_0_k101.mtx");
    // ifstream fin("../res/unit_test/L2.mtx");

    // Declare variables:
    int M, N, NNZ;

    // DependencyGraph dg = DependencyGraph();

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
        m--;
        n--;
        // dg.insertEdge(m, n);

        if (n != currCol)
        {
            Lp[n] = l; // if we are traversing the next column mark its starting
            currCol = n;
        }

        Lx[l] = data; // nz data
        Li[l] = m;    // row of the lth nz data point
    }

    // cout << "Dependency Graph Built\n";

    Lp[N] = NNZ;  // only this should be required
    Li[NNZ] = -1; // JSICS
    Lx[NNZ] = -1; // JSICS
    // dg.traverseDG();

    // unordered_set<int> seed;
    // seed.clear();
    // seed.insert(1);
    // seed.insert(6);
    // unordered_set<int> reachable;
    // reachable.clear();

    // dg.findReachable(seed, reachable);

    // cout << "Reachable set size: " << reachable.size() << "\n";
    // unordered_set<int>::iterator it;
    // for (it = reachable.begin(); it != reachable.end(); it++)
    //     cout << *it << " ";
    // cout << "\n\n";
    fin.close();

    return 0;
}

int readRHS(const int order, double *&B)
{
    // Open the file:
    // ifstream fin("../res/af_0_k101/af_0_k101_b.mtx");
    // ifstream fin("../res/unit_test/B2.mtx");
    ifstream fin("../res/af_0_k101/b_sparse_af_0_k101.mtx");

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
int print(int N, const double *x)
{
    cout << "Solving Lx+B gives: \n";
    for (int i = 0; i < N; i++)
    {
        cout << "x[" << i << "] = " << *(x + i) << "\n";
    }
    return 0;
}

void printExecutionTime(std::chrono::_V2::system_clock::time_point start, std::chrono::_V2::system_clock::time_point end, string msg)
{
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Time taken to " << msg << ": "
         << duration.count() << " microseconds" << endl;

    return;
}
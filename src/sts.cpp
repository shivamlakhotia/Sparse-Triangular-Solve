#include <iostream>
#include <fstream>
#include <algorithm>

using namespace std;

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

// code referred from http://www.cplusplus.com/forum/general/65804/
int readMatrix(int &order, int *&Lp, int *&Li, double *&Lx)
{
    // Open the file:
    std::ifstream fin("../res/af_0_k101/af_0_k101.mtx");
    // std::ifstream fin("../res/adjnoun/adjnoun.mtx");

    // Declare variables:
    int M, N, NNZ;

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

        if (n != currCol)
        {
            Lp[n] = l; // if we are traversing the next column mark its starting
            currCol = n;
        }

        Lx[l] = data; // nz data
        Li[l] = m;    // row of the lth nz data point
    }

    Lp[N] = NNZ;  // only this should be required
    Li[NNZ] = -1; // JSICS
    Lx[NNZ] = -1; // JSICS

    fin.close();

    return 0;
}

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
        x[j] /= Lx[Lp[j]];
        for (p = Lp[j] + 1; p < Lp[j + 1]; p++)
        {
            x[Li[p]] -= Lx[p] * x[j];
        }
    }
    return (1);
}

int main()
{
    int n;
    double *x;
    int *Lp;
    int *Li;
    double *Lx;

    cout << "Hello World!\n";

    readMatrix(n, Lp, Li, Lx);

    // readMatrix();
    return 0;
}
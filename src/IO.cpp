#include "IO.h"

// code referred from http://www.cplusplus.com/forum/general/65804/
/**
 * Reading the Matrix from a file in Matrix Market format (.mtx)
 * Storing the sparse matrix in Compressed Sparse Column (CSC) format.
 * 
 * Outputs are:
 * order : the matrix dimension
 * Lp : the column pointer of L
 * Li : the row index of L
 * Lx : the values of L
 */
int readMatrix(int &order, int *&Lp, int *&Li, double *&Lx)
{
    string fileName;
    cout << "The Matrix file should be in coordinate format. Sparse Martix (L) File: (file should be in 'res/' directory) ";
    cin >> fileName;
    cout << "Reading the LHS matrix from: " << fileName << "\n";

    // Open the file:
    // ifstream fin("../res/af_0_k101/af_0_k101.mtx");
    ifstream fin("../res/" + fileName);

    // ifstream fin("../res/unit_test/L2.mtx");

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

    // initilize all the values with zero (will increase exec time)
    // fill(Lp, Lp + M, 0);
    // fill(Li, Li + NNZ, 0);
    // fill(Lx, Lx + NNZ, 0.);

    // Create your matrix:
    // Memory footprint of this program should be around 100 MB.
    // Size of Lx should be around 70 MB.
    // Size of Lp and Li should be around 2 MB each.
    // Verify this using resourse monitor. Verified.

    // initialize the starting points.
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

/**
 * As the name suggests, this method reads the RHS array B.
 * This method first finds out the format of the matrix
 * and calls the respective function to read the RHS.
 * Supports two formats right now: Array and Coordinate
 */
int readRHS(const int order, double *&B)
{
    string fileName;
    enum Format
    {
        Coordinate,
        Array,
        Undefined
    };

    enum Format format = Undefined;
    string line;

    cout << "RHS (B) File: (file should be in 'res/' directory) ";

    cin >> fileName;

    ifstream fin("../res/" + fileName);

    while (true)
    {
        getline(fin, line);
        if (line.find("%%MatrixMarket") != string::npos)
        {
            if (line.find("coordinate")!= string::npos)
                format = Coordinate;
            else if (line.find("array")!= string::npos)
                format = Array;
            else
                cout << "\nError: Unhandled format\n";
            break;
        }
    }

    fin.close();

    assert(format != Undefined);

    if (format == Coordinate)
        readRHS_coordinate(order, B, fileName);
    else if (format == Array)
        readRHS_array(order, B, fileName);
    else
        cout << "Should never be here!";

    return 0;
}

/**
 * Utility function: reads the RHS present in array format
 */
int readRHS_array(const int order, double *&B, string fileName)
{
    cout << "Reading the RHS column from: " << fileName << "\n";

    // // Open the file:
    // // ifstream fin("../res/af_0_k101/af_0_k101_b.mtx");
    // // ifstream fin("../res/unit_test/B2.mtx");
    // // ifstream fin("../res/af_0_k101/b_dense_af_0_k101.mtx");
    ifstream fin("../res/" + fileName);

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


/**
 * Utility function: reads the RHS present in coordinate format
 */
int readRHS_coordinate(const int order, double *&B, string fileName)
{
    cout << "Reading the RHS column from: " << fileName << "\n";

    // Open the file:
    // ifstream fin("../res/af_0_k101/af_0_k101_b.mtx");
    // ifstream fin("../res/unit_test/B2.mtx");
    // ifstream fin("../res/af_0_k101/b_sparse_af_0_k101.mtx");
    // ifstream fin("../res/af_0_k101/b_dense_af_0_k101.mtx");

    ifstream fin("../res/" + fileName);

    // Declare variables:
    int M, N, NNZ;

    // Ignore headers and comments:
    while (fin.peek() == '%')
        fin.ignore(2048, '\n');

    // Read defining parameters:
    fin >> M >> N >> NNZ;

    assert(order == M);

    B = new double[M + 2];

    memset(B, 0., sizeof(B));

    // Read the data and create the matrix in CSC format
    for (int l = 0; l < NNZ; l++)
    {
        // getting data
        int m, n;
        double data;
        fin >> m >> n >> data;
        m--;
        n--;
        // fin >> B[l];
        B[m] = data;
    }
    B[M] = -1; // JSICS

    fin.close();

    return 0;
}

/**
 * Prints the solution x of Lx=B
 */
int printSolution(int N, const double *x)
{
    cout << "Solving Lx+B gives: \n";
    for (int i = 0; i < N; i++)
    {
        cout << "x[" << i << "] = " << *(x + i) << "\n";
    }
    return 0;
}

/**
 * Prints the execution time provided the starting and ending tie
 */ 
void printExecutionTime(std::chrono::_V2::system_clock::time_point start, std::chrono::_V2::system_clock::time_point end, string msg)
{
    auto duration = duration_cast<microseconds>(end - start);

    cout << "Time taken to " << msg << ": "
         << duration.count() << " microseconds" << endl;

    return;
}
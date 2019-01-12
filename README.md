# Sparse Triangular Solve
## Folder structure
* `res` - Resources
* `src` - Source Code
* `docs` - Documents

## Building and Running
1) Go to the `src` directory
2) run `make clean`
3) run `make`
4) run `./sparseTriangularSolve < input.txt`

## Changing Inputs
* `input.txt` - Input File
* First Line: File name of the L matrix. This file should be in `/res` directory
* Second File: File name of B matrix.
This file should be in `/res` directory.
* Third Line: 
    * For unoptimized lsolve: 0
    * For Optimized lsolve: 1

## Results
* Input: 
    * L = `af_0_k101/af_0_k101.mtx`
    * B = `b_sparse_af_0_k101.mtx`
* For the above input the results were similar for both optimized and unoptimized.
* The reason being that the size of reachSet in case of optimized is N.
* This happens because B contains non-zero at starting indices. And the structure of L is such that the entire graph is reachable if seed set contains the starting vertices.
* In the case when reach set is small as compared to N their is a significant improvement in the execution time.
* Using threads also didn't give any significant improvement. Threads were used for the inner for-loop of lsolve algo. Currently the parallelization is disabled. To enable add `-fopenmp` in the Makefile.

| Operation | Execution Time |
|-----------|----------------|
|Building Dependency Graph| 13.480387 seconds|
|Finding Reachability Set|20.936 milliseconds|
|Solving Lx=B (Unoptimized, \|reachSet\| = N) |52.631 milliseconds|
|Solving Lx=B (Optimized, \|reachSet\| = N)|54.58 milliseconds|
|Solving Lx=B (Unoptimized, \|reachSet\| ~ N/2)| 56.271 milliseconds|
|Solving Lx=B (Optimized, \|reachSet\| ~ N/2)| 27.219 milliseconds|

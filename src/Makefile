sparseTriangularSolve: sts.o IO.o DependencyGraph.o
	g++ -fopenmp sts.o IO.o DependencyGraph.o -o sparseTriangularSolve
sts.o: sts.cpp DependencyGraph.h IO.h
	g++ -fopenmp -c sts.cpp
IO.o: IO.cpp DependencyGraph.h
	g++ -c IO.cpp
DependencyGraph.o: DependencyGraph.cpp
	g++ -c DependencyGraph.cpp
clean:
	rm *.o sparseTriangularSolve

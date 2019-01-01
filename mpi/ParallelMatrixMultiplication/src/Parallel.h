#ifndef MPI_PARALLELMATRIXMULTIPLICATION_PARALLEL_H
#define MPI_PARALLELMATRIXMULTIPLICATION_PARALLEL_H

#include <vector>
#include <cmath>

#include "mpi.h"

#include "Multiplication.h"
#include "Worker.h"

using std::vector;


class Parallel: public Multiplication {
private:
    void calculateWorkerNodeLoads(int numPartitions, vector<int>& workerNodeLoads) const;
    void sendDataToWorkerNodes(vector<int>& workerNodeLoads) const;
    void receiveResultsFromWorkerNodes(vector<int>& workerNodeLoads, Matrix& matR) const;
public:
    Parallel(const Matrix& matA, const Matrix& matB);

    void calculate(Matrix& matR, int numPartitions) const;
};


#endif //MPI_PARALLELMATRIXMULTIPLICATION_PARALLEL_H

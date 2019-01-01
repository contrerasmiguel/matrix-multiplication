#ifndef MPI_PARALLELMATRIXMULTIPLICATION_WORKER_H
#define MPI_PARALLELMATRIXMULTIPLICATION_WORKER_H

#include "mpi.h"

#include "Node.h"
#include "Parallel.h"

class Worker: public Node {
private:
    void processIncomingData() const;
    void receiveMatricesSizeData(int& load, int& numColumnsMatB, int& numRowsMatB) const;
    void receiveMatricesData(Matrix& partialMatA, Matrix& matB) const;

public:
    virtual void start() const;
};


#endif //MPI_PARALLELMATRIXMULTIPLICATION_WORKER_H

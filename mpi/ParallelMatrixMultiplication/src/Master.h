#ifndef MPI_PARALLELMATRIXMULTIPLICATION_MASTER_H
#define MPI_PARALLELMATRIXMULTIPLICATION_MASTER_H

#include <algorithm>
#include <limits>
#include <sstream>

#include "mpi.h"

#include "Input.h"
#include "Matrix.h"
#include "Multiplication.h"
#include "Parallel.h"
#include "Node.h"

using std::numeric_limits;

class Master;

// Una función de tipo OptionHandler es cualquiera que reciba dos matrices
// y retorne void.
typedef void (Master::*OptionHandler)(const Matrix&, const Matrix&) const;

// Las opciones del menú junto con sus respectivas funciones gestoras.
struct Option {
    string description;
    OptionHandler optionHandler;
};

class Master: public Node {
private:
    static const Option MENU_OPTIONS[];

    static const short MIN_WORKER_NODES;
    static const short MIN_RANDOM;
    static const short MAX_RANDOM;

    const short NUM_MENU_OPTIONS;

    void showMenu() const;
    bool executeOption(short inputOption
                     , const Matrix& matA
                     , const Matrix& matB) const;

    int readPartitionNumber(int numRowsA, int numWorkerNodes) const;
    void showResults(const Matrix& matR) const;

    void showMatrices(const Matrix& matA, const Matrix& matB) const;

    double multiplySequentialTime(const Matrix& matA
            , const Matrix& matB
            , Matrix& matR) const;
    void multiplySequential(const Matrix& matA, const Matrix& matB) const;

    double multiplyParallelTime(const Matrix& matA
            , const Matrix& matB
            , Matrix& matR
            , int numPartitions) const;
    void multiplyParallel(const Matrix& matA, const Matrix& matB) const;

    void timeSequentialParallel(const Matrix& matA, const Matrix& matB) const;
    void timeParallelParallel(const Matrix& matA, const Matrix& matB) const;
    void resultsSequentialParallel(const Matrix& matA, const Matrix& matB) const;

public:
    Master();
    virtual void start() const;
};


#endif //MPI_PARALLELMATRIXMULTIPLICATION_MASTER_H

#ifndef MPI_PARALLELMATRIXMULTIPLICATION_MULTIPLICATION_H
#define MPI_PARALLELMATRIXMULTIPLICATION_MULTIPLICATION_H

#include "Matrix.h"


class Multiplication {
protected:
    const Matrix & matA
               , & matB
    ;

    void calculate(int numRowsA
                 , int numColumnsB
                 , int numColumsA
                 , Matrix& matR) const;

public:
    Multiplication(const Matrix& matA, const Matrix& matB);

    void calculate(Matrix& matR) const;
};


#endif //MPI_PARALLELMATRIXMULTIPLICATION_MULTIPLICATION_H

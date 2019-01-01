#include "Multiplication.h"


Multiplication::Multiplication(const Matrix& matA, const Matrix& matB)
        : matA(matA), matB(matB) {  }

void Multiplication::calculate(int numRowsA
                             , int numColumnsB
                             , int numColumsA
                             , Matrix& matR) const {
    for (int i = 0; i < numRowsA; ++i) {
        for (int j = 0; j < numColumnsB; ++j) {
            int partialSum = 0;
            for (int k = 0; k < numColumsA; ++k) {
                partialSum += this->matA.get(i, k) * this->matB.get(k, j);
            }
            matR.set(i, j, partialSum);
        }
    }
}

void Multiplication::calculate(Matrix& matR) const {
    calculate(matA.getRows()
            , matB.getColumns()
            , matA.getColumns(), matR);
}

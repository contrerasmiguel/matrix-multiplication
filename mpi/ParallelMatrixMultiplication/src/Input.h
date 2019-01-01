#ifndef MPI_PARALLELMATRIXMULTIPLICATION_INPUT_H
#define MPI_PARALLELMATRIXMULTIPLICATION_INPUT_H

#include <iostream>

using std::cout;
using std::cin;
using std::endl;


class Input {
private:
    static void readMatrixDimension(int& rowsMat, int& columnsMat);

public:
    struct InputData {
        int rowsMatA;
        int columnsMatA;
        int rowsMatB;
        int columnsMatB;
    };

    static void readMatrixDimensions(InputData& inputData);
};


#endif //MPI_PARALLELMATRIXMULTIPLICATION_INPUT_H

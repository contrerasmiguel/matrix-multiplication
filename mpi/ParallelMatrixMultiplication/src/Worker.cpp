#include "Worker.h"


void Worker::start() const {
    MPI_Status status;
    unsigned char workMessage;

    while (true) {
        processIncomingData();
    }
}

void Worker::processIncomingData() const {
    int load
      , numColumnsMatB
      , numRowsMatB;

    receiveMatricesSizeData(load, numColumnsMatB, numRowsMatB);

    Matrix partialMatA(load, numRowsMatB);
    Matrix matB(numRowsMatB, numColumnsMatB);

    receiveMatricesData(partialMatA, matB);

    Matrix matR(partialMatA.getRows(), matB.getColumns());

    Multiplication multiplication(partialMatA, matB);
    multiplication.calculate(matR);

    // Se envían los resultados al nodo maestro.
    MPI_Send(matR.getRawData(0, 0), matR.getRows() * matR.getColumns(), MPI_INT, 0
            , 0, MPI_COMM_WORLD);
}

void Worker::receiveMatricesSizeData(int& load, int& numColumnsMatB, int& numRowsMatB) const {
    MPI_Status status;

    // Se recibe la cantidad de filas que debe manejar el nodo.
    MPI_Recv(&load, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    // Se recibe la cantidad de columnas de la segunda matriz.
    MPI_Recv(&numColumnsMatB, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    // Se recibe el numero de filas de la segunda matriz (que es el
    // número de columnas de la primera matriz).
    MPI_Recv(&numRowsMatB, 1, MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
}

void Worker::receiveMatricesData(Matrix& partialMatA, Matrix& matB) const {
    MPI_Status status;

    // Se recibe parte de las filas de la primera matriz.
    MPI_Recv((int*)partialMatA.getRawData(0, 0), partialMatA.getRows() * partialMatA.getColumns()
            , MPI_INT, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    // Se recibe la segunda matriz completa.
    MPI_Recv((int*)matB.getRawData(0, 0), matB.getRows() * matB.getColumns(), MPI_INT, 0, MPI_ANY_TAG
            , MPI_COMM_WORLD, &status);
}


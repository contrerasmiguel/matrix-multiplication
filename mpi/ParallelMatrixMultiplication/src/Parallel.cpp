#include "Parallel.h"


Parallel::Parallel(const Matrix& matA, const Matrix& matB)
        : Multiplication(matA, matB) {  }

void Parallel::calculateWorkerNodeLoads(int numPartitions, vector<int>& workerNodeLoads) const {
    const int loadTotal = matA.getRows()
    , meanWorkerNodeLoad = loadTotal/numPartitions;

    int assignedLoad = numPartitions * meanWorkerNodeLoad;
    workerNodeLoads = vector<int>((unsigned long)numPartitions, meanWorkerNodeLoad);

    for ( int workerId = 1
            ; assignedLoad < loadTotal
            ; workerId = (workerId == numPartitions)
                         ? 1
                         : workerId + 1) {
        workerNodeLoads[workerId - 1] += 1;
        assignedLoad += 1;
    }
};

void Parallel::calculate(Matrix& matR, int numPartitions) const {
    vector<int> workerNodeLoads;
    calculateWorkerNodeLoads(numPartitions, workerNodeLoads);
    sendDataToWorkerNodes(workerNodeLoads);
    receiveResultsFromWorkerNodes(workerNodeLoads, matR);
}

void Parallel::sendDataToWorkerNodes(vector<int>& workerNodeLoads) const {

    const int numColumnsMatA = matA.getColumns();

    const int numRowsMatB = numColumnsMatA;
    const int numColumnsMatB = matB.getColumns();

    for ( int workerId = 1, firstRow = 0
        ; workerId <= workerNodeLoads.size()
        ; firstRow += workerNodeLoads[workerId - 1], ++workerId) {

        // Se le envía el número de filas de la primera matriz que le toca procesar.
        MPI_Send(&workerNodeLoads[workerId - 1], 1, MPI_INT, workerId, 0, MPI_COMM_WORLD);

        // Se envía el número de columnas de la segunda matriz.
        MPI_Send(&numColumnsMatB, 1, MPI_INT, workerId, 0, MPI_COMM_WORLD);

        // Se envía el número de columnas de A (o número de filas de B).
        MPI_Send(&numRowsMatB, 1, MPI_INT, workerId, 0, MPI_COMM_WORLD);

        // Se envía el grupo de filas de la primera matriz que le toca procesar.
        MPI_Send(matA.getRawData(firstRow, 0), workerNodeLoads[workerId - 1] * numColumnsMatA, MPI_INT
                , workerId, 0, MPI_COMM_WORLD);

        // Se envía la segunda matriz completa.
        MPI_Send(matB.getRawData(0, 0), numRowsMatB * numColumnsMatB, MPI_INT, workerId
                , 0, MPI_COMM_WORLD);

    }
}

void Parallel::receiveResultsFromWorkerNodes(vector<int>& workerNodeLoads, Matrix& matR) const {
    MPI_Status status;

    for ( int workerId = 1, firstRow = 0
            ; workerId <= workerNodeLoads.size()
            ; firstRow += workerNodeLoads[workerId - 1], ++workerId) {

        // Se recibe parte de las filas de la primera matriz.
        MPI_Recv((int*)matR.getRawData(firstRow, 0), workerNodeLoads[workerId - 1] * matR.getColumns()
                , MPI_INT, workerId, MPI_ANY_TAG, MPI_COMM_WORLD, &status);

    }
}





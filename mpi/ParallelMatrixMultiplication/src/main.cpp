#include "main.h"

int main(int argc, char* argv[]) {
    int nodeId;

    MPI_Init(&argc, &argv);
    MPI_Comm_rank(MPI_COMM_WORLD, &nodeId);

    // Se creará un Master o un Worker, dependiendo del
    // ID del nodo/proceso.
    unique_ptr<Node> node = (nodeId == MASTER_NODE_ID)
                            ? unique_ptr<Node>(new Master)
                            : unique_ptr<Node>(new Worker)
    ;
    node->start();

    exit(EXIT_SUCCESS);
}

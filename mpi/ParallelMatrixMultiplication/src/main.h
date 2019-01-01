#ifndef MPI_PARALLELMATRIXMULTIPLICATION_MAIN_H
#define MPI_PARALLELMATRIXMULTIPLICATION_MAIN_H

#include <cstdlib>
#include <memory>

#include "mpi.h"

#include "Master.h"
#include "Worker.h"

using std::unique_ptr;

const short MASTER_NODE_ID = 0;

#endif //MPI_PARALLELMATRIXMULTIPLICATION_MAIN_H

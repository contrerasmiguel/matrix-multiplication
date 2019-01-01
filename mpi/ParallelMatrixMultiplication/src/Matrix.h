#ifndef MPI_PARALLELMATRIXMULTIPLICATION_MATRIX_H
#define MPI_PARALLELMATRIXMULTIPLICATION_MATRIX_H

#include <random>
#include <sstream>

using std::endl;
using std::mt19937;
using std::ostringstream;
using std::random_device;
using std::string;
using std::to_string;
using std::uniform_int_distribution;


class Matrix {
private:
    const int rows, columns;
    int* m;

public:
    Matrix(int rows, int columns);
    ~Matrix();

    int getRows() const;
    int getColumns() const;
    int get(int row, int column) const;
    const int* getRawData(int row, int column) const;
    void set(int row, int column, int value);
    string show() const;
    void setRandomValues(int min, int max);
    bool equals(const Matrix& mx) const;
};


#endif //MPI_PARALLELMATRIXMULTIPLICATION_MATRIX_H

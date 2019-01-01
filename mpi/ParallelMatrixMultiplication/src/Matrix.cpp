#include "Matrix.h"


Matrix::Matrix(int rows, int columns) : rows(rows), columns(columns) {
    m = new int[rows*columns];
}

Matrix::~Matrix() {
    delete [] m;
}

int Matrix::getRows() const {
    return rows;
}

int Matrix::getColumns() const {
    return columns;
}

int Matrix::get(int row, int column) const {
    return m[row*columns + column];
}

const int* Matrix::getRawData(int row, int column) const {
    return &m[row*columns + column];
}

void Matrix::set(int row, int column, int value) {
    m[row*columns + column] = value;
}

string Matrix::show() const {
    string output;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            output = output + "\t" + to_string(m[i*columns + j]);
        }
        output = output + "\n";
    }
    return output;
}

void Matrix::setRandomValues(int min, int max) {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dist(min, max);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            m[i*columns + j] = dist(gen);
        }
    }
}

bool Matrix::equals(const Matrix& mx) const {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (m[i*columns + j] != mx.get(i,j))
                return false;
        }
    }
    return true;
}

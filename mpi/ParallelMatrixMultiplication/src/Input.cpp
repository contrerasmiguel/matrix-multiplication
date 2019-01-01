#include "Input.h"


void Input::readMatrixDimension(int& rowsMat, int& columnsMat) {
    cout << "Numero de filas: ";
    cin >> rowsMat;
    cout << "Numero de columnas: ";
    cin >> columnsMat;
}

void Input::readMatrixDimensions(InputData& inputData)
{
    do {
        cout << "Matriz #1:" << endl;
        readMatrixDimension(inputData.rowsMatA, inputData.columnsMatA);

        cout << "Matriz #2:" << endl;
        readMatrixDimension(inputData.rowsMatB, inputData.columnsMatB);

        if (inputData.columnsMatA != inputData.rowsMatB) {
            cout        << "El numero de columnas de la primera matriz debe ser igual al "
            << endl << "numero de filas de la segunda matriz."
            << endl;
        }
    }
    while (inputData.columnsMatA != inputData.rowsMatB);
}
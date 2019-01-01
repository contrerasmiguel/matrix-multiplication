/*
 *  Autor: Miguel Contreras
 *  C.I. 24401758
 *  Sistemas Concurrentes
 */

#include <cstdlib>
#include <ctime>
#include <cstdio>
#include <iostream>
#include <limits>
#include <omp.h>
#include <sstream>
#include <string>
#include <sys/time.h>

using namespace std;

template <class T>
class Matrix
{
private:
    int rows, columns;
    T* m;

public:
    Matrix(int rows, int columns) {
        m = NULL;
        this->rows = rows;
        this->columns = columns;

        m = new T[rows*columns];
    }

    ~Matrix() {
        delete [] m;
    }

    int getRows() {
        return rows;
    }

    int getColumns() {
        return columns;
    }

    T get(int row, int column) {
        return m[row*columns + column];
    }

    void set(int row, int column, T value) {
        m[row*columns + column] = value;
    }

    string show() {
        ostringstream oss;
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                oss << "\t" << m[i*columns + j];
            }
            oss << endl;
        }
        return oss.str();
    }

    void setRandomValues(unsigned seed, int min, int max) {
        srand(seed);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < columns; ++j) {
                m[i*columns + j] = min + rand() % ((max+1) - min);
            }
        }
    }

};

template <class T>
class Multiplication
{
protected:
    Matrix<T>& matA;
    Matrix<T>& matB;
    Matrix<T>& matR;
public:
    Multiplication(Matrix<T>& matA, Matrix<T>& matB, Matrix<T>& matR)
    : matA(matA), matB(matB), matR(matR) {  }

    virtual void calculate() = 0;
};

template <class T>
class Sequential : public Multiplication<T>
{
public:
    Sequential(Matrix<T>& matA, Matrix<T>& matB, Matrix<T>& matR): Multiplication<T>(matA, matB, matR) {  }

    void calculate() {
        for (int i = 0; i < this->matA.getRows(); ++i) {
            for (int j = 0; j < this->matB.getColumns(); ++j) {
            	int partialSum = 0;
                for (int k = 0; k < this->matA.getColumns(); ++k) {
                    partialSum += this->matA.get(i, k) * this->matB.get(k, j);
                }
                this->matR.set(i, j, partialSum);
            }
        }
    }
};

template <class T>
class ParallelCell : public Multiplication<T>
{
public:
    ParallelCell(Matrix<T>& matA, Matrix<T>& matB, Matrix<T>& matR): Multiplication<T>(matA, matB, matR) {  }

    void calculate() {
        for (int i = 0; i < this->matA.getRows(); ++i) {
            for (int j = 0; j < this->matB.getColumns(); ++j) {
                int partialSum = 0;
#pragma omp parallel for reduction(+:partialSum)
                for (int k = 0; k < this->matA.getColumns(); ++k) {
                    partialSum += this->matA.get(i, k) * this->matB.get(k, j); 
                }
                this->matR.set(i, j, partialSum);
            }
        }
    }        
};

template <class T>
class ParallelRow : public Multiplication<T>
{
public:
    ParallelRow(Matrix<T>& matA, Matrix<T>& matB, Matrix<T>& matR): Multiplication<T>(matA, matB, matR) {  }

    void calculate() {
        for (int i = 0; i < this->matA.getRows(); ++i) {
#pragma omp paralalel for        	
            for (int j = 0; j < this->matB.getColumns(); ++j) {
            	int partialSum = 0;
                for (int k = 0; k < this->matA.getColumns(); ++k) {
                    partialSum += this->matA.get(i, k) * this->matB.get(k, j);
                }
                this->matR.set(i, j, partialSum);
            }
        }
    }        
};

template <class T>
class ParallelMatrix : public Multiplication<T>
{
public:
    ParallelMatrix(Matrix<T>& matA, Matrix<T>& matB, Matrix<T>& matR): Multiplication<T>(matA, matB, matR) {  }

    void calculate() {
#pragma omp paralalel for 
        for (int i = 0; i < this->matA.getRows(); ++i) {       	
            for (int j = 0; j < this->matB.getColumns(); ++j) {
            	int partialSum = 0;
                for (int k = 0; k < this->matA.getColumns(); ++k) {
                    partialSum += this->matA.get(i, k) * this->matB.get(k, j);
                }
                this->matR.set(i, j, partialSum);
            }
        }
    }        
};

class Input
{
private:
    static void readMatrixDimension(int& rowsMat, int& columnsMat) {
        cout << "Numero de filas: ";
        cin >> rowsMat;
        cout << "Numero de columnas: ";
        cin >> columnsMat;
    }    

public:
    struct InputData {
        int rowsMatA;
        int columnsMatA;
        int rowsMatB;
        int columnsMatB;
    };

    static void readMatrixDimensions(InputData& inputData)
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

};

template <typename T>
long calculate_time(Multiplication<T>& multiplication)
{
    timeval start, end;
    long seconds, useconds; 

    gettimeofday(&start, NULL);
    multiplication.calculate();
    gettimeofday(&end, NULL);

    seconds  = end.tv_sec  - start.tv_sec;
    useconds = end.tv_usec - start.tv_usec;

    return ((seconds) * 1000 + useconds/1000.0) + 0.5;
}

template <class T>
class OptionExecuter
{
private:
    Matrix<T>* matA; 
    Matrix<T>* matB;
    Matrix<T>* matR;

    void executeOption(char option)
    {
        cout << endl;
        switch (option) {
        case '1':
        	multiplySequential();
            break;
        case '2':
            multiplyParallel();
            break;
        case '3':
        	compareTimesSequentialParallel();
            break;
        case '4':
            compareTimesParallelParallel();
            break;
        case '5':
            compareResults();
            break;
        }
        cout << endl;
    }

    long multiplySequential()
    {
        Sequential<T> sequential(*matA, *matB, *matR);
        long time = calculate_time(sequential);
        cout << "La multiplicacion de matrices entre una matriz " << matA->getRows() << "x" << matA->getColumns()
             << " y una matriz " << matB->getRows() << "x" << matB->getColumns() << " en forma secuencial toma: " << time 
             << " milisegundos" << endl;
        return time;         
    }    

    int leerMetodoParalelo() {
    	int opt;

    	do {
    		cout << 	    "Seleccione un metodo de ejecucion paralela."
    			 << endl << "1. Paralelizacion de sumas parciales."
    			 << endl << "2. Paralelizacion de cada columna por la matriz completa."
    			 << endl << "3. Paralelizacion de cada columna por cada columna."
    			 << endl
    		;

    		cin >> opt;

    		if (opt < 1 || opt > 3)
    			cout << "Opcion invalida." << endl;
    	}
    	while (opt < 1 || opt > 3);

    	return opt;    	
    }

    long multiplyParallel()
    {
    	ParallelCell<T> pCell(*matA, *matB, *matR);
    	ParallelRow<T> pRow(*matA, *matB, *matR);
    	ParallelMatrix<T> pMatrix(*matA, *matB, *matR);

    	Multiplication<T>* pSelected;

    	int opt = leerMetodoParalelo();

    	if (opt == 1)
    		pSelected = &pCell;
    	else if (opt == 2)
    		pSelected = &pRow;
    	else
    		pSelected = &pMatrix;

        long time = calculate_time(*pSelected);
        cout << "La multiplicacion de matrices entre una matriz " << matA->getRows() << "x" << matA->getColumns()
             << " y una matriz " << matB->getRows() << "x" << matB->getColumns() << " en forma paralela toma: " << time 
             << " milisegundos" << endl;
        return time;
    }

    void compareTimesSequentialParallel()
    {
        long timeParallel = multiplyParallel();
        long timeSequential = multiplySequential();

        cout << endl;

        if (timeParallel < timeSequential) {
            cout << "Para este caso, la ejecucion paralela resulta favorable." << endl;
        }
        else if (timeSequential < timeParallel) {
            cout << "Para este caso, la ejecucion secuencial resulta favorable." << endl;
        }
        else {
            cout << "Ambos tipo de ejecucion, paralela y secuencial, tardan el mismo \ntiempo para calcular el resultado." << endl;
        }
    }

	void compareTimesParallelParallel() {
    	ParallelCell<T> pCell(*matA, *matB, *matR);
    	ParallelRow<T> pRow(*matA, *matB, *matR);
    	ParallelMatrix<T> pMatrix(*matA, *matB, *matR);

        long timeCell = calculate_time(pCell);
        cout << "Paralelizando cada suma parcial (la operacion mas pequeña): " << timeCell << " milisegundos." << endl;

        long timeRow = calculate_time(pRow);
        cout << endl << "Paralelizando cada multiplicacion de una \nfila por una fila de la otra matriz: " << timeRow << " milisegundos."
             << endl
             << endl << "Ejemplo:"
             << endl << "1 4 3 0     0"
        	 << endl << "         x  5"
        	 << endl << "            6"
        	 << endl << "            4"
        	 << endl
        ;

        long timeMatrix = calculate_time(pMatrix);
        cout << endl << "Paralelizando cada multiplicacion de una \nfila por la otra matriz entera: " << timeMatrix << " milisegundos."
             << endl
             << endl << "Ejemplo:"             
             << endl << "2 1 0 3     1 9 5"
        	 << endl << "         x  2 7 6"
        	 << endl << "            1 3 4"
        	 << endl << "            2 3 1"
        	 << endl
        ;

        if (timeCell < timeRow && timeCell < timeMatrix)
        	cout << "La paralelizacion de sumas parciales es la mas rapida." << endl;
        else if (timeRow < timeCell && timeRow << timeMatrix)
        	cout << "La paralelizacion de filas por filas es la mas rapida." << endl;
        else
        	cout << "La paralelizacion de filas por la matriz entera es la mas rapida" << endl;

        if (timeCell > timeRow && timeCell > timeMatrix)
        	cout << "La paralelizacion de sumas parciales es la mas lenta." << endl;
        else if (timeRow > timeCell && timeRow > timeMatrix)
        	cout << "La paralelizacion de filas por filas es la mas lenta." << endl;
        else
        	cout << "La paralelizacion de filas por la matriz entera es la mas lenta." << endl;

	}

    void compareResults()
    {
    	ParallelCell<T> pCell(*matA, *matB, *matR);
    	ParallelRow<T> pRow(*matA, *matB, *matR);
    	ParallelMatrix<T> pMatrix(*matA, *matB, *matR);

    	Multiplication<T>* pSelected;

    	int opt = leerMetodoParalelo();

    	if (opt == 1)
    		pSelected = &pCell;
    	else if (opt == 2)
    		pSelected = &pRow;
    	else
    		pSelected = &pMatrix;

        Matrix<T> matRSequential(matA->getRows(), matB->getColumns());
        Sequential<T> sequential(*matA, *matB, matRSequential);

        cout << "Matriz #1\n\n";
        cout << matA->show() << endl;

        cout << "Matriz #2\n\n";
        cout << matB->show() << endl;

        pSelected->calculate();
        cout << "\tResultados del calculo con paralelismo:\n";
        cout << matR->show() << endl;

        sequential.calculate();
        cout << "\tResultados del calculo en forma secuencial:\n";
        cout << matRSequential.show() << endl;
    }

public:
    OptionExecuter(Input::InputData inputData) {
        matA = new Matrix<T>(inputData.rowsMatA, inputData.columnsMatA);
        matB = new Matrix<T>(inputData.rowsMatB, inputData.columnsMatB);
        matR = new Matrix<T>(inputData.rowsMatA, inputData.columnsMatB);

        unsigned seed = time(NULL);

        matA->setRandomValues(seed, 1, 3);
        matB->setRandomValues(seed, 1, 3);        
    }

    ~OptionExecuter() {
        delete matA;
        delete matB;
        delete matR;
    }

    void showMenu()
    {
        char option;
        do
        {
            string menuMessage =
                    "\n"
                            "Seleccione una opcion."
                    "\n"
                    "\n"    "1. Multiplicacion de matrices en forma secuencial."
                    "\n"    "2. Multiplicacion de matrices en forma paralela."
                    "\n"    "3. Comparacion entre tiempos de metodo paralelo y metodo secuencial."
                    "\n"    "4. Comparacion entre tiempos de metodos paralelos."
                    "\n"    "5. Comparacion entre resultados de metodo paralelo y metodo secuencial."
                    "\n"    "0. Salir.";

            cout << menuMessage << endl;

            cin >> option;

            if (option < '0' || option > '5') {
                cout << "Opción invalida." << endl;
            }
            else {
                executeOption(option);
            }
        }
        while (option != '0');
    }
};

int main(int argc, char** argv)
{
    Input::InputData inputData;
    Input::readMatrixDimensions(inputData);
    OptionExecuter<int> OptionExecuter(inputData);
    OptionExecuter.showMenu();
    exit(EXIT_SUCCESS);
}

#include "Master.h"

const Option Master::MENU_OPTIONS[] = {

      { .description = "Salir"
      , .optionHandler = NULL }

    , { .description = "Mostrar matrices que se multiplicarán"
      , .optionHandler = (OptionHandler)&Master::showMatrices }

    , { .description = "Multiplicar matrices usando PROCESAMIENTO SECUENCIAL"
      , .optionHandler = (OptionHandler)&Master::multiplySequential }

    , { .description = "Multiplicar matrices usando PROCESAMIENTO PARALELO"
      , .optionHandler = (OptionHandler)&Master::multiplyParallel }

    , { .description = "Tiempos de PROCESAMIENTO SECUENCIAL vs. PROCESAMIENTO PARALELO"
      , .optionHandler = (OptionHandler)&Master::timeSequentialParallel }

    , { .description = "Tiempos de diferentes formas de PROCESAMIENTO PARALELO"
      , .optionHandler = (OptionHandler)&Master::timeParallelParallel }

    , { .description = "Resultados de PROCESAMIENTO SECUENCIAL vs. PROCESAMIENTO PARALELO"
      , .optionHandler = (OptionHandler)&Master::resultsSequentialParallel }

};

const short Master::MIN_WORKER_NODES = 2;
const short Master::MIN_RANDOM = 1;
const short Master::MAX_RANDOM = 3;


Master::Master()
        : NUM_MENU_OPTIONS((sizeof MENU_OPTIONS) / sizeof (Option)) {  }

void Master::start() const {
    // Se leen las dimensiones de las matrices.
    Input::InputData inputData;
    Input::readMatrixDimensions(inputData);

    // Se declaran las matrices con las dimensiones leídas.
    Matrix matA(inputData.rowsMatA, inputData.columnsMatA);
    Matrix matB(inputData.rowsMatB, inputData.columnsMatB);

    matA.setRandomValues(MIN_RANDOM, MAX_RANDOM);
    matB.setRandomValues(MIN_RANDOM, MAX_RANDOM);

    short inputOption;

    do {
        showMenu();
        cin >> inputOption;
        cout << endl;
        if (!executeOption(inputOption, matA, matB)) {
            cout << "Opcián inválida." << endl;
        }
    }
    while (inputOption != 0);
}

void Master::showMenu() const {
    cout << "\n\nSeleccione una opción.\n\n";
    // Se muestran las descripciones de cada una de las opciones del menú.
    for (int i = 1; i < NUM_MENU_OPTIONS; ++i) {
        cout << " " << i << ". " << MENU_OPTIONS[i].description << "." << endl;
    }
    cout << " " << "0" << ". " << MENU_OPTIONS[0].description << "." << endl;
    cout << endl;
}

bool Master::executeOption(short inputOption
                         , const Matrix &matA
                         , const Matrix &matB) const {
    if (inputOption >= 0 && inputOption <= NUM_MENU_OPTIONS) {
        if ((this->MENU_OPTIONS[inputOption].optionHandler) != NULL) {
            (this->*MENU_OPTIONS[inputOption].optionHandler)(matA, matB);
        }
        return true;
    }
    return false;
}

int Master::readPartitionNumber(int numRowsA, int numWorkerNodes) const {
    return (numRowsA > (numWorkerNodes))
           ? (numWorkerNodes)
           : numRowsA;
}

void Master::showResults(const Matrix& matR) const {
    char input;

    cout << "Desea ver la matriz resultante? (s/n): ";
    cin >> input;
    cout << endl;

    if (input == 's' || input =='S') {
        cout << "Matriz resultante:\n\n";
        cout << matR.show() << "\n\n";
    }
}

void Master::showMatrices(const Matrix& matA, const Matrix& matB) const {
    cout << "Matriz #1\n\n";
    cout << matA.show() << "\n\n";
    cout << "Matriz #2\n\n";
    cout << matB.show() << "\n\n";
}

double Master::multiplySequentialTime(const Matrix& matA
                              , const Matrix& matB
                              , Matrix& matR) const {
    Multiplication sequential(matA, matB);
    double timeInit = MPI_Wtime();
    sequential.calculate(matR);
    double timeEnd = MPI_Wtime();
    double secs = timeEnd - timeInit;
    printf("Tiempo de ejecución de multiplicación de matrices con procesamiento \n"
           "secuencial: %.16g milisegundos\n", secs * 1000.0);
    return secs;
}

void Master::multiplySequential(const Matrix& matA, const Matrix& matB) const {
    Matrix matR(matA.getRows(), matB.getColumns());
    multiplySequentialTime(matA, matB, matR);
    cout << endl;
    showResults(matR);
}

double Master::multiplyParallelTime(const Matrix& matA
                            , const Matrix& matB
                            , Matrix& matR
                            , int numPartitions) const {
    Parallel parallel(matA, matB);
    double timeInit = MPI_Wtime();
    parallel.calculate(matR, numPartitions);
    double timeEnd = MPI_Wtime();
    double secs = timeEnd - timeInit;

    printf("Tiempo de ejecución de multiplicación de matrices con procesamiento \n"
           "paralelo, dividiendo la matriz #1 en %i partes: %.16g milisegundos\n", numPartitions, secs * 1000.0);
    return secs;
}

void Master::multiplyParallel(const Matrix& matA, const Matrix& matB) const {
    int numNodes;
    MPI_Comm_size(MPI_COMM_WORLD, &numNodes);

    if ((numNodes - 1) >= MIN_WORKER_NODES) {
        Matrix matR(matA.getRows(), matB.getColumns());
        const int numPartitions = readPartitionNumber(matA.getRows(), (numNodes - 1));
        multiplyParallelTime(matA, matB, matR, numPartitions);
        cout << endl;
        showResults(matR);
    }
    else {
        cout << "Son necesarios dos o mas nodos trabajadores para poder continuar." << endl;
    }
}

void Master::timeSequentialParallel(const Matrix& matA, const Matrix& matB) const {
    int numNodes;
    MPI_Comm_size(MPI_COMM_WORLD, &numNodes);

    if ((numNodes - 1) >= MIN_WORKER_NODES) {
        Matrix matR(matA.getRows(), matB.getColumns());
        const int numPartitions = readPartitionNumber(matA.getRows(), (numNodes - 1));
        cout << endl;
        double timeSequential = multiplySequentialTime(matA, matB, matR);
        cout << endl;
        double timeParallel = multiplyParallelTime(matA, matB, matR, numPartitions);

        cout << endl;

        if (timeSequential < timeParallel) {
            cout << "La multiplicación con PROCESAMIENTO SECUENCIAL se ejecutó en menor tiempo.\n\n";
        }
        else if (timeParallel < timeSequential) {
            cout << "La multiplicación con PROCESAMIENTO PARALELO se ejecutó en menor tiempo.\n\n";
        }
        else {
            cout << "Ambas formas de procesamiento, PARALELO y SECUENCIAL, tardaron el mismo tiempo en \n"
                 << "multiplicar dos matrices.\n\n";
        }
    }
    else {
        cout << "Son necesarios dos o mas nodos trabajadores para poder continuar." << endl;
    }
}

void Master::timeParallelParallel(const Matrix& matA, const Matrix& matB) const {
    int numNodes;

    MPI_Comm_size(MPI_COMM_WORLD, &numNodes);
    const int numWorkerNodes = numNodes - 1;

    if (numWorkerNodes >= MIN_WORKER_NODES) {
        int numTestCases;
        vector<int> partitionNums;

        const int MIN_TEST_CASES = 2;
        const int MAX_PARTITIONS = (matA.getRows() > (numWorkerNodes))
                                   ? (numWorkerNodes)
                                   : matA.getRows();

        cout << "Actualmente, la matriz se está dividiendo entre el número de nodos \n"
                "trabajadores que se especifica al ejecutar el programa. Esta opción, \n"
                "por su parte, permite comparar los tiempos de ejecución cuando solo \n"
                "se utiliza cierta parte del total de nodos trabajadores.\n\n"
                "Ingrese el número de casos que desea probar: ";

        do {
            cin >> numTestCases;

            if (numTestCases < MIN_TEST_CASES || numTestCases > MAX_PARTITIONS) {
                printf("El número de casos de prueba debe estar entre %i y %i.",
                       MIN_TEST_CASES, MAX_PARTITIONS);
            }
        }
        while (numTestCases < 1 || numTestCases > MAX_PARTITIONS);

        // En cuanto debe ir incrementando la secuencia.
        int incrementValue = MAX_PARTITIONS/numTestCases;

        for (int i = 0, testCase = incrementValue
                ; i < (numTestCases - 1)
                ; ++i, testCase += incrementValue) {
            partitionNums.push_back(testCase);
        }
        // La secuencia debe incluir el caso por defecto.
        partitionNums.push_back(MAX_PARTITIONS);

        if (partitionNums.size() >= 2) {
            // Ordenar los casos de prueba de menor a mayor.
            sort(partitionNums.begin(), partitionNums.end());

            Parallel parallel(matA, matB);
            Matrix dummyMat(matA.getRows(), matB.getColumns());

            int fastestTestCase = 0;
            double fastestTime = numeric_limits<double>::infinity();

            int slowestTestCase = 0;
            double slowestTime = 0;

            for (int i = 0; i < partitionNums.size(); ++i) {
                double timeInit = MPI_Wtime();
                parallel.calculate(dummyMat, partitionNums[i]);
                double timeEnd = MPI_Wtime();
                double secs = timeEnd - timeInit;

                if (secs < fastestTime) {
                    fastestTestCase = i;
                    fastestTime = secs;
                }

                if (secs > slowestTime) {
                    slowestTestCase = i;
                    slowestTime = secs;
                }

                printf("Tiempo de ejecución de multiplicación de matrices con procesamiento \n"
                               "paralelo, dividiendo la matriz #1 en %i partes: %.16g milisegundos\n\n",
                       partitionNums[i], secs * 1000.0
                );
            }

            printf("El menor tiempo de ejecución se obtuvo dividiendo la primera matriz en %i \n"
                           "partes. Tiempo: %.16g milisegundos.\n\n"
                           "El mayor tiempo de ejecución se obtuvo dividiendo la primera matriz en %i \n"
                           "partes. Tiempo: %.16g milisegundos.\n", partitionNums[fastestTestCase],
                   fastestTime * 1000.0, partitionNums[slowestTestCase], slowestTime * 1000.0
            );

        }
        else {
            cout << "No se encontró suficientes casos de prueba válidos en la entrada." << endl;
        }
    }
    else {
        cout << "Son necesarios dos o mas nodos trabajadores para poder continuar." << endl;
    }
}

void Master::resultsSequentialParallel(const Matrix& matA, const Matrix& matB) const {
    int numNodes;
    MPI_Comm_size(MPI_COMM_WORLD, &numNodes);

    if (numNodes >= 3) {
        Matrix matRSequential(matA.getRows(), matB.getColumns());
        Matrix matRParallel(matA.getRows(), matB.getColumns());
        const int numPartitions = readPartitionNumber(matA.getRows(), (numNodes - 1));

        Multiplication sequential(matA, matB);
        sequential.calculate(matRSequential);

        Parallel parallel(matA, matB);
        parallel.calculate(matRParallel, numPartitions);

        cout << endl;

        if (matRSequential.equals(matRParallel)) {
            cout << "Los resultados son idénticos." << endl;
        }
        else {
            cout << "Los resultados son diferentes." << endl;
        }

        char input;

        cout << "Desea ver las matrices resultantes? (s/n): ";
        cin >> input;
        cout << endl;

        if (input == 's' || input =='S') {
            cout << endl;

            cout << "Resultados de multiplicación con PROCESAMIENTO SECUENCIAL\n\n"
            << matRSequential.show() << endl;

            cout << endl;

            cout << "Resultados de multiplicación con PROCESAMIENTO PARALELO\n\n"
            << matRParallel.show() << endl;

            cout << endl;
        }
    }
}


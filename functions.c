#include "functions.h"
#include <limits.h>

void startRandomTour(int* tour, int maxCitySize) {
    //Fill the array with sequencial values
    for (int i = 0; i < maxCitySize; ++i) {
        tour[i] = i;
    }

    //Shuffle the array values
    for (int i = maxCitySize - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
    }
}

int calcTourDistance(const int* tour, const int *graph, int maxCitySize) {
    int distance = 0;
    for (int i = 0; i < maxCitySize - 1; ++i) {
        distance += *(graph + tour[i + 1] * maxCitySize + tour[i]);
    }
    distance += *(graph + tour[0] * maxCitySize + tour[maxCitySize-1]); // Return to starting city
    return distance;
}

int generateNewUniqueCity(const int *currentPossibleTours, int maxCitySize, int currentCityIndex) {
    int newCity;
    int cityAlreadyUsed;

    do {
        // Generate a new candidate city
        newCity = rand() % maxCitySize;

        // Check if the candidate city has already been used in the current tour
        cityAlreadyUsed = 0;
        for (int k = 0; k < currentCityIndex; ++k) {
            if (newCity == *(currentPossibleTours + k)) {
                cityAlreadyUsed = 1;
                break;
            }
        }
    } while (cityAlreadyUsed);

    return newCity;
}

int* loadTestCities(int matrixSize) {
    int *matrix = (int *)malloc(matrixSize * matrixSize * sizeof(int));

    *(matrix + 0 * matrixSize + 0) = 0;
    *(matrix + 1 * matrixSize + 0) = 23;
    *(matrix + 2 * matrixSize + 0) = 10;
    *(matrix + 3 * matrixSize + 0) = 4;
    *(matrix + 4 * matrixSize + 0) = 1;

    *(matrix + 0 * matrixSize + 1) = 23;
    *(matrix + 1 * matrixSize + 1) = 0;
    *(matrix + 2 * matrixSize + 1) = 9;
    *(matrix + 3 * matrixSize + 1) = 5;
    *(matrix + 4 * matrixSize + 1) = 4;

    *(matrix + 0 * matrixSize + 2) = 10;
    *(matrix + 1 * matrixSize + 2) = 9;
    *(matrix + 2 * matrixSize + 2) = 0;
    *(matrix + 3 * matrixSize + 2) = 8;
    *(matrix + 4 * matrixSize + 2) = 2;

    *(matrix + 0 * matrixSize + 3) = 4;
    *(matrix + 1 * matrixSize + 3) = 5;
    *(matrix + 2 * matrixSize + 3) = 8;
    *(matrix + 3 * matrixSize + 3) = 0;
    *(matrix + 4 * matrixSize + 3) = 11;

    *(matrix + 0 * matrixSize + 4) = 1;
    *(matrix + 1 * matrixSize + 4) = 4;
    *(matrix + 2 * matrixSize + 4) = 2;
    *(matrix + 3 * matrixSize + 4) = 11;
    *(matrix + 4 * matrixSize + 4) = 0;

    return matrix;
}

int* fetchInputFileData(int *matrixSize) {
    /* --- Read user input --- */
    char inputPath[150];

    printf("Insert file path: ");
    scanf("%s", inputPath);
    /* ----------------------- */


    /* --- Read data from file --- */
    FILE *file = fopen(inputPath, "r");
    if(file == NULL) { // Invalid file, load test data
        printf("Invalid file, using default test city map...\n");
        return loadTestCities(*matrixSize);
    }

    char ch; // Temp char used for reading

    // Read first integer, which corresponds to the size of the matrix
    fscanf(file, "%d", matrixSize);

    // Declare the matrix
    int *cityMap = (int *)malloc(*matrixSize * *matrixSize * sizeof(int));

    // Read data from file
    for (int i = 0; i < *matrixSize; i++) {
        for (int j = 0; j < *matrixSize; j++) {
            fscanf(file, "%d", &(*(cityMap + j * *matrixSize + i)));
        }
    }

    fclose(file); // Close file stream
    /* --------------------------- */

    // Return final map pointer
    return cityMap;
}
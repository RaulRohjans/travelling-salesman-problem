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

    // Read first char, which corresponds to the size of the matrix
    if(((ch = fgetc(file)) != EOF)) {
        *matrixSize = ch - '0'; //Convert to int and save value
    }
    else { // File is empty, so end application
        printf("The input file is invalid!\n");
        exit(1);
    }

    // Declare the matrix
    int *cityMap = (int *)malloc(*matrixSize * *matrixSize * sizeof(int));

    // Read file one char at a time
    int row = 0, col = 0;
    bool isFirstTime = true;
    while ((ch = fgetc(file)) != EOF) {
        if(isFirstTime && ch == '\n') {
            // It may happen the first char that is read from the file is a \n
            // Since the first line is dedicated to the matrix size, lets just skip it
            isFirstTime = false;
            continue;
        }
        else if(ch == '\n' || row == *matrixSize-1) { // Means we finished reading a row, need to increment the col
            col++;
            row = 0;
            continue;
        }

        if(!isdigit(ch)) // Skip if its just some blank space, we only want numbers
            continue;

        *(cityMap + row * *matrixSize + col) = ch - '0';
        row++;
    }

    fclose(file); // Close file stream
    /* --------------------------- */

    // Return final map pointer
    return cityMap;
}
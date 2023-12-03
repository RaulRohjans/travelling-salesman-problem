#ifndef FUNCIONS_H_
#define FUNCIONS_H_

void startRandomTour(int* tour, int maxCitySize);

int calcTourDistance(const int* tour, const int *graph, int maxCitySize);

int generateNewUniqueCity(const int *currentPossibleTours, int maxCitySize, int currentCityIndex);

int* loadTestCities(int matrixSize);

int * fetchInputFileData(int *matrixSize);

#endif // FUNCIONS_H_
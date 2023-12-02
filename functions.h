#ifndef FUNCIONS_H_
#define FUNCIONS_H_

#define MAX_SIZE 5

void startRandomTour(int* tour);

int calcTourDistance(int* tour, int graph[MAX_SIZE][MAX_SIZE]);

int generateNewUniqueCity(int currentPossibleTours[MAX_SIZE], int currentCityIndex);

#endif // FUNCIONS_H_
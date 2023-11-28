#include "functions.h"
#include <limits.h>

#define MAX_SIZE 5

void startRandomTour(int* tour) {
    srand(5831); //Seed rand

    //Fill the array with sequencial values
    for (int i = 0; i < MAX_SIZE; ++i) {
        tour[i] = i;
    }

    //Shuffle the array values
    for (int i = MAX_SIZE - 1; i > 0; --i) {
        int j = rand() % (i + 1);
        int temp = tour[i];
        tour[i] = tour[j];
        tour[j] = temp;
    }
}

int calcTourDistance(int* tour, int graph[MAX_SIZE][MAX_SIZE]) {
    int distance = 0;
    for (int i = 0; i < MAX_SIZE - 1; ++i) {
        distance += graph[tour[i]][tour[i + 1]];
    }
    distance += graph[tour[MAX_SIZE - 1]][tour[0]]; // Return to starting city
    return distance;
}
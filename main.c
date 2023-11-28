#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#include "functions.c"

#define MAX_SIZE 5
#define POSSIBLE_TOURS_SIZE 10
#define MAX_GENERATIONS 1000 //Number of times the possible tours will be improved
#define CROSSOVER_RATE 0.7
#define MUTATION_RATE 0.2
#define DE_FACTOR 0.5

int main()
{
    // Test matrix
    // This should be replaced with a function that reads data from a text file
    int cityMap[MAX_SIZE][MAX_SIZE] = {
        {0, 23, 10, 4, 1},
        {23, 0, 9, 5, 4},
        {10, 9, 0, 8, 2},
        {4, 5, 8, 0, 11},
        {1, 4, 2, 11, 0}
    };

    int possibleTours[POSSIBLE_TOURS_SIZE][MAX_SIZE];
    int distances[POSSIBLE_TOURS_SIZE];

    // Initialize possible tours with random tours
    for (int i = 0; i < POSSIBLE_TOURS_SIZE; ++i) {
        startRandomTour(possibleTours[i]);
        distances[i] = calcTourDistance(possibleTours[i], cityMap);
    }

    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        for (int i = 0; i < POSSIBLE_TOURS_SIZE; ++i) {
            int targetIndex, mutantIndex1, mutantIndex2;
            do {
                targetIndex = rand() % POSSIBLE_TOURS_SIZE;
                mutantIndex1 = rand() % POSSIBLE_TOURS_SIZE;
                mutantIndex2 = rand() % POSSIBLE_TOURS_SIZE;
            } while (mutantIndex1 == targetIndex || mutantIndex2 == targetIndex || mutantIndex1 == mutantIndex2);

            // Create a mutant by combining two other individuals
            for (int j = 0; j < MAX_SIZE; ++j) {
                if (((double)rand() / RAND_MAX) < CROSSOVER_RATE || j == MAX_SIZE - 1) {
                    possibleTours[i][j] = possibleTours[mutantIndex1][j] + DE_FACTOR * (possibleTours[mutantIndex2][j] - possibleTours[mutantIndex1][j]);
                }
            }

            // Apply mutation
            for (int j = 0; j < MAX_SIZE; ++j) {
                if (((double)rand() / RAND_MAX) < MUTATION_RATE) {
                    int mutationPoint = rand() % MAX_SIZE;
                    int temp = possibleTours[i][j];
                    possibleTours[i][j] = possibleTours[i][mutationPoint];
                    possibleTours[i][mutationPoint] = temp;
                }
            }

            // Clip values to ensure they are valid indices
            for (int j = 0; j < MAX_SIZE; ++j) {
                if (possibleTours[i][j] < 0) {
                    possibleTours[i][j] = 0;
                } else if (possibleTours[i][j] >= MAX_SIZE) {
                    possibleTours[i][j] = MAX_SIZE - 1;
                }
            }

            // Calculate the distance of the mutant
            int mutantDistance = calcTourDistance(possibleTours[i], cityMap);

            // Update the possibleTours if the mutant is better
            if (mutantDistance < distances[i]) {
                distances[i] = mutantDistance;
            }
        }
    }

    // Find the best tour in the final possibleTours
    int bestIndex = 0;
    for (int i = 1; i < POSSIBLE_TOURS_SIZE; ++i) {
        if (distances[i] < distances[bestIndex]) {
            bestIndex = i;
        }
    }

    // Print the best tour and its distance
    printf("Best tour: ");
    for (int i = 0; i < MAX_SIZE; ++i) {
        printf("%d ", possibleTours[bestIndex][i]);
    }
    printf("\n");
    printf("Distance: %d\n", distances[bestIndex]);
}
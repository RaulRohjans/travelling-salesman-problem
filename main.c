#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>

#include "functions.c"

#define MAX_CITY_TEST_SIZE 5
#define POSSIBLE_TOURS_SIZE 10
#define MAX_GENERATIONS 1000 // Number of times the possible tours will be improved
#define CROSSOVER_RATE 0.7
#define MUTATION_RATE 0.2
#define DE_FACTOR 0.5

int main()
{
    /* --- Program Data --- */
    int cityMapSize = MAX_CITY_TEST_SIZE; // Assign fallback size
    int *cityMap = fetchInputFileData(&cityMapSize); // Init program data

    int possibleTours[POSSIBLE_TOURS_SIZE][cityMapSize];
    int distances[POSSIBLE_TOURS_SIZE];
    /* ---------------------- */


    /* Initialize possible tours with random values */
    for (int i = 0; i < POSSIBLE_TOURS_SIZE; ++i) {
        startRandomTour(possibleTours[i], cityMapSize);
        distances[i] = calcTourDistance(possibleTours[i], cityMap, cityMapSize);
    }
    /* -------------------------------------------- */

    /* --- Start Differential Evolution Algo --- */
    for (int generation = 0; generation < MAX_GENERATIONS; ++generation) {
        for (int i = 0; i < POSSIBLE_TOURS_SIZE; ++i) {
            /*
             * Select three indexes for the crossover process.
             * AlterIndexes are indexes from other individuals that will be crossed and merged into the targetIndex
             *
             * We have to make sure these are different from each other so the same city is not visited more than once.
             * */
            int targetIndex, alterIndex1, alterIndex2;
            do {
                targetIndex = rand() % POSSIBLE_TOURS_SIZE;
                alterIndex1 = rand() % POSSIBLE_TOURS_SIZE;
                alterIndex2 = rand() % POSSIBLE_TOURS_SIZE;
            } while (alterIndex1 == targetIndex || alterIndex2 == targetIndex || alterIndex1 == alterIndex2);

            /*
             * Create an altered individual by combining the other two into this one
             * */
            for (int j = 0; j < cityMapSize; ++j) {
                /*
                 * Crossover process (where the merging is done):
                 *
                 * We want to base our crossover of off a probability, since we don't always want to apply it.
                 * To make this process work, we generate a double between 0 and 1, and compare it to the CROSSOVER_RATE,
                 * which is what defines the chances of it to happen.
                 * If the generated number is lower, than we can apply the crossover.
                 *
                 * To eliminate the chances of never applying crossover to a given individual, we have the second part
                 * of the if statement, which forces the crossover if we are on the last iteration.
                 * */
                if (((double)rand() / RAND_MAX) < CROSSOVER_RATE || j == cityMapSize - 1) {
                    possibleTours[i][j] = possibleTours[alterIndex1][j] + DE_FACTOR * (possibleTours[alterIndex2][j] - possibleTours[alterIndex1][j]);
                }

                /*
                 * Repair process:
                 *
                 * The crossover process has quite a high chance of merging duplicated values into the individual.
                 * In this context, we do not want to go through the same city more than once.
                 *
                 * To fix this, we can implement a repair logic which checks for duplicated cities and keeps generating
                 * new ones until they are unique.
                 * */
                for (int k = 0; k < j; ++k) {
                    if (possibleTours[i][j] == possibleTours[i][k]) { // The city is duplicated
                        possibleTours[i][j] = generateNewUniqueCity(possibleTours[i], cityMapSize, j);
                        k = -1; // Check again to make sure the new city is also not duplicated
                    }
                }
            }

            // Apply the altering of the values
            for (int j = 0; j < cityMapSize; ++j) {
                /*
                 * Use a similar logic to the crossover process where we generate a random number between 0 and 1.
                 * If this is lower than the altering rate (mutation rate), than we apply it to the city.
                 * */
                if (((double)rand() / RAND_MAX) < MUTATION_RATE) {
                    int mutationPoint = rand() % cityMapSize; //Select another city in the tour (random) to swap with current
                    int temp = possibleTours[i][j];
                    possibleTours[i][j] = possibleTours[i][mutationPoint]; //Alter the tour with, perform swap
                    possibleTours[i][mutationPoint] = temp; //Assign the current city to the mutation point
                }
            }

            /*
             * Enforce value check
             *
             * This is an "overkill" check to make sure there are no invalid values being stored
             * in the possible tours
             * */
            for (int j = 0; j < cityMapSize; ++j) {
                if (possibleTours[i][j] < 0) {
                    possibleTours[i][j] = 0;
                } else if (possibleTours[i][j] >= cityMapSize) {
                    possibleTours[i][j] = cityMapSize - 1;
                }
            }

            /* --- Calculate Distance --- */
            // Calculate the distance of the altered tour
            int mutantDistance = calcTourDistance(possibleTours[i], cityMap, cityMapSize);

            // Update the distance if the altered is better
            if (mutantDistance < distances[i]) {
                distances[i] = mutantDistance;
            }
            /* -------------------------- */

        }
    }
    /* ----------------------------------------- */


    /* --- Get the best tour & show the results --- */
    int bestIndex = 0; // Find the best tour
    for (int i = 1; i < POSSIBLE_TOURS_SIZE; ++i) {
        if (distances[i] < distances[bestIndex]) {
            bestIndex = i;
        }
    }

    //Show it
    printf("Best tour: ");
    for (int i = 0; i < cityMapSize; ++i) {
        printf("%d ", possibleTours[bestIndex][i] + 1);
    }
    printf("\n");
    printf("Distance: %d\n", distances[bestIndex]);
    /* --------------------------------------------- */
}
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>

#include "functions.c"

#define MAX_SIZE 5

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

    int travel[MAX_SIZE]; // City travel sequence
    int currentCity = 0;

    int newClosestCity = findClosestCity(cityMap, currentCity);
    printf("number: %d\n", newClosestCity);
}
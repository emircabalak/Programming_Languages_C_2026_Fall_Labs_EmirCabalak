/*
 * week4_1_dynamic_array.c
 * Author: Emir Cabalak
 * Student ID: 260AEB076
 * Description:
 *   Demonstrates creation and usage of a dynamic array using malloc.
 *   Allocate memory for n integers, read them from the user,
 *   print their sum and average, and then free the memory.
 *
 *   Output must match the format in the Week 4 instructions exactly
 *   (it is checked by the autograder).
 */

#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int n;
    int *arr = NULL;

    printf("Enter number of elements: ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Invalid size.\n");
        return 1;
    }

    // We only learn the size at runtime, so the array has to live on the heap.
    arr = malloc(n * sizeof(int));

    // malloc returns NULL if it cannot reserve the memory, and using a NULL pointer would crash the program.
    if (arr == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    printf("Enter %d integers: ", n);
    for (int i = 0; i < n; i++) {
        // Free the array before exiting so a bad value does not leak memory.
        if (scanf("%d", &arr[i]) != 1) {
            printf("Invalid input.\n");
            free(arr);
            return 1;
        }
    }

    // long long keeps the sum from overflowing when many large values are entered.
    long long sum = 0;
    for (int i = 0; i < n; i++) {
        sum += arr[i];
    }

    // Casting to double avoids integer division, so 7 and 8 give 7.50 instead of 7.00.
    double average = (double)sum / n;

    printf("Sum = %lld\n", sum);
    printf("Average = %.2f\n", average);

    // Every successful malloc needs a matching free.
    free(arr);
    return 0;
}

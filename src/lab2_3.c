#include <stdio.h>

int is_prime(int n) {
    if (n < 2) {
        return 0;
    }
    for (int i = 2; i <= n / i; i++) {
        if (n % i == 0) {
            return 0;
        }
    }
    return 1;
}

int main(void) {
    int n;

    printf("Enter an integer n (>= 2): ");
    if (scanf("%d", &n) != 1) {
        printf("Input error. Expected an integer.\n");
        return 1;
    }

    if (n < 2) {
        printf("Error: n must be at least 2.\n");
        return 1;
    }

    printf("Primes up to %d:", n);
    for (int i = 2; i <= n; i++) {
        if (is_prime(i)) {
            printf(" %d", i);
        }
    }
    printf("\n");
    return 0;
}

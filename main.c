/*
 * This program performs basic arithmetic operations on very large numbers disregarding operator priorities:
 * multiplication, sum and subtract.
 *
 * Written by Daryush Tsydzik
 */

#include "functions.h"

#define SIZE 501

int main() {
    char *expr, *res;

    expr = malloc(SIZE * sizeof(char));
    if (expr == NULL) {
        printf("Failed to allocate memory");
        return 8;
    }

    printf("Enter expression: ");
    scanf("%500s", expr);
    while(getchar() != '\n');

    // calculate
    switch (calculate(expr, &res)) {
        case 2:
            free(expr);
            expr = NULL;
            printf("Incorrect input");
            return 1;
        case 3:
            free(expr);
            expr = NULL;
            printf("Failed to allocate memory");
            return 8;
        default:
            printf("%s\n", res);
            free(res);
            res = NULL;
    }

    free(expr);
    free(res);
    expr = NULL;
    res = NULL;
    return 0;
}

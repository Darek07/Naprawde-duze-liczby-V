//
// Created by Daryush Tsydzik on 16.04.2021.
//

#ifndef NAPRAWDE_DUZE_LICZBY_V_FUNCTIONS_H
#define NAPRAWDE_DUZE_LICZBY_V_FUNCTIONS_H

#include <stdio.h>
#include <stdlib.h>

int multiply(const char* number1, const char* number2, char** result);
int subtract(const char* number1, const char* number2, char** result);
int add(const char* number1, const char* number2, char** result);

int validate(const char *number);
int compare(const char* number1, const char* number2);

int validate_expression(const char *expr);
int calculate(const char* expr, char **result);

#endif //NAPRAWDE_DUZE_LICZBY_V_FUNCTIONS_H

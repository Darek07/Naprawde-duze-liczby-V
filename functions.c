//
// Created by Daryush Tsydzik on 13.04.2021.
//

#include "functions.h"
#include <string.h>
#include <ctype.h>

int multiply(const char* number1, const char* number2, char** result) {
    if (number1 == NULL || number2 == NULL || result == NULL)
        return 1;
    else if (validate(number1) == 2 || validate(number2) == 2)
        return 2;

    if (*number1 == '0' || *number2 == '0') {
        *result = calloc(2,sizeof(char));
        if (*result == NULL)
            return 3;
        **result = '0';
        *(*result + 1) = '\0';
        return 0;
    }

    signed char sign1 = (signed char) (*number1 == '-'),
            sign2 = (signed char) (*number2 == '-');

    if (sign1 == 1 && sign2 == 0 || sign1 == 0 && sign2 == 1) {
        int l;
        if (sign1)
            l = multiply(number1 + 1, number2, result);
        else l = multiply(number1, number2 + 1, result);
        if (l != 0)
            return l;

        l = (int)strlen(*result);
        for (int i = l; i > 0; i--)
            *(*result + i) = *(*result + i - 1);

        **result = '-';

        return 0;
    }
    else if (sign1 && sign2)
        return multiply(number1 + 1, number2 + 1, result);

    int len1 = strlen(number1), len2 = strlen(number2), len = len1 + len2 + 3;

    // memory for result
    *result = calloc(len, sizeof(char));
    if (*result == NULL)
        return 3;
    for (int i = 0; i < (int)len; i++)
        *(*result + i) = '0';

    unsigned char digit, dot, k, left_shift = 0;

    for (int i = (int)len2 - 1; i >= 0; i--) {
        dot = 0, k = left_shift;
        for (int j = (int)len1 - 1; j >= 0; j--) {
            digit = (*(number1 + j) - '0') * (*(number2 + i) - '0') + dot;
            if (*(*result + k))
                digit += *(*result + k) - '0';
            *(*result + k++) = (char)( digit % 10 + '0');
            dot = digit / 10;
        }
        if (dot > 0)
            *(*result + k) = (char) (dot + '0');
        left_shift++;
    }

    // reverse deleting zeros at the beginning
    while (*(*result + len - 1) == '0') {
        *(*result + len - 1) = 0;
        len--;
    }

    char swap;
    for (int i = 0, j = (int) len - 1; i < j; i++, j--){
        swap = *(*result + i);
        *(*result + i) = *(*result + j);
        *(*result + j) = swap;
    }

    return 0;
}

int subtract(const char* number1, const char* number2, char** result) {
    if (number1 == NULL || number2 == NULL || result == NULL)
        return 1;
    else if (validate(number1) == 2 || validate(number2) == 2)
        return 2;

    signed char sign1 = (signed char)compare(number1, "0"),
            sign2 = (signed char)compare(number2, "0");

    if ((sign1 == 1 || sign1 == 0) && sign2 == -1)
        return add(number1, number2 + 1, result);
    else if (sign1 == -1 && sign2 == -1)
        return subtract(number2 + 1, number1 + 1, result);
    else if (sign1 == -1 && (sign2 == 1 || sign2 == 0)) {
        int l = add(number1 + 1, number2, result);
        if (l != 0)
            return l;

        l = (int)strlen(*result);
        for (int i = l; i > 0; i--)
            *(*result + i) = *(*result + i - 1);

        **result = '-';
        *(*result + l + 1) = '\0';

        return 0;
    }

    unsigned int len = 0, len1 = strlen(number1), len2 = strlen(number2);

    // memory for result
    if (len1 > len2) {
        *result = malloc((len1 + 3) * sizeof(char));
        if (*result == NULL)
            return 3;
    } else {
        *result = malloc((len2 + 3) * sizeof(char));
        if (*result == NULL)
            return 3;
    }

    unsigned char digit1, digit2, dot = 0;
    int greater = compare(number1, number2);

    while (len1 > 0 && len2 > 0) {
        // find the number from the end by subtracting what we have taken
        digit1 = (int)*(number1 + --len1) - '0';
        digit1 -= (greater != -1) * (dot * (-9 * (digit1 == 0) + (digit1 != 0)));

        digit2 = (int)*(number2 + --len2) - '0';
        digit2 -= (greater == -1) * (dot * (-9 * (digit2 == 0) + (digit2 != 0)));

        if (greater == -1) {
            // notice that we took a dozen and set a dot
            if (digit2 < digit1) {
                digit2 += 10;
                dot = 1;
            } else if (dot && digit2 != 9)
                dot = 0;

            *(*result + len) = (char) (digit2 - digit1 + '0');
        }
        else {
            if (digit1 < digit2) {
                digit1 += 10;
                dot = 1;
            } else if (dot && digit1 != 9)
                dot = 0;

            *(*result + len) = (char)(digit1 - digit2 + '0');
        }

        len++;
    }

    // if number1 has more digits than number2
    while (len1 > 0) {
        digit1 = (int)*(number1 + --len1) - '0';
        digit1 -= dot * (-9 * (digit1 == 0) + (digit1 != 0));

        if (dot && digit1 != 9)
            dot = 0;

        *(*result + len) = (char)(digit1 + '0');

        len++;
    }

    // if number2 has more digits than number1
    while (len2 > 0) {
        digit2 = (int)*(number2 + --len2) - '0';
        digit2 -= dot * (-9 * (digit2 == 0) + (digit2 != 0));

        if (dot && digit2 != 9)
            dot = 0;

        *(*result + len) = (char)(digit2 + '0');

        len++;
    }

    // reverse deleting zeros at the beginning. dot checks the start of result here
    dot = 1;
    unsigned int i = 0;
    char swap;
    while (len > 0 && i < len) {
        if (*(*result + --len) == '0' && dot)
            continue;
        else if (dot){
            if (greater == -1) {
                len++;
                *(*result + len) = '-';
            }
            *(*result + len + dot) = '\0';
            dot = 0;
        }

        swap = *(*result + i);
        *(*result + i) = *(*result + len);
        *(*result + len) = swap;
        i++;
    }

    if ((*(*result)) == '0')
        *(*result + 1) = '\0';

    return 0;
}

int add(const char* number1, const char* number2, char** result) {
    if (number1 == NULL || number2 == NULL || result == NULL)
        return 1;
    else if (validate(number1) == 2 || validate(number2) == 2)
        return 2;

    signed char sign1 = (signed char)compare(number1, "0"),
            sign2 = (signed char)compare(number2, "0");

    if ((sign1 == 1 || sign1 == 0) && sign2 == -1)
        return subtract(number1, number2 + 1, result);
    else if (sign1 == -1 && (sign2 == 1 || sign2 == 0))
        return subtract(number2, number1 + 1, result);
    else if (sign1 == -1 && sign2 == -1) {
        int l = add(number1 + 1, number2 + 1, result);
        if (l != 0)
            return l;

        l = (int)strlen(*result);
        for (int i = l; i > 0; i--)
            *(*result + i) = *(*result + i - 1);

        **result = '-';
        *(*result + l + 1) = '\0';

        return 0;
    }

    unsigned int len = 0, len1 = strlen(number1), len2 = strlen(number2);

    // memory for result
    if (len1 > len2) {
        *result = malloc((len1 + 3) * sizeof(char));
        if (*result == NULL)
            return 3;
    } else {
        *result = malloc((len2 + 3) * sizeof(char));
        if (*result == NULL)
            return 3;
    }

    unsigned char digit1, digit2, dot = 0;

    while (len1 > 0 && len2 > 0) {
        // find the number from the end
        digit1 = (int)*(number1 + --len1) - '0';
        digit2 = (int)*(number2 + --len2) - '0';

        *(*result + len) = (char) ((digit1 + digit2 + dot) % 10 + '0');

        // notice that we took an additional unit and set a dot
        if (digit1 + digit2 + dot >= 10)
            dot = 1;
        else dot = 0;

        len++;
    }

    // if number1 has more digits than number2
    while (len1 > 0) {
        digit1 = (int)*(number1 + --len1) - '0';

        *(*result + len) = (char)((digit1 + dot) % 10 + '0');

        if (digit1 + dot >= 10)
            dot = 1;
        else dot = 0;

        len++;
    }

    // if number2 has more digits than number1
    while (len2 > 0) {
        digit2 = (int)*(number2 + --len2) - '0';

        *(*result + len) = (char)((digit2 + dot) % 10 + '0');

        if (digit2 + dot >= 10)
            dot = 1;
        else dot = 0;

        len++;
    }

    if (dot) {
        *(*result + len) = '1';
        len++;
    }

    // reverse deleting zeros at the beginning. dot checks the start of result here
    dot = 1;
    unsigned int i = 0;
    char swap;
    while (len > 0 && i < len) {
        if (*(*result + --len) == '0' && dot)
            continue;
        else if (dot){
            *(*result + len + dot) = '\0';
            dot = 0;
        }

        swap = *(*result + i);
        *(*result + i) = *(*result + len);
        *(*result + len) = swap;
        i++;
    }

    return 0;
}

int validate(const char *number) {
    if (number == NULL)
        return -1;
    unsigned int len = strlen(number);
    if (*number == '\0' || (len == 1 && *number == '-') || len > 1 && *number == '0')
        return 2;

    for (int i = *number == '-'; *(number + i) != '\0'; i++)
        if (!(*(number + i) >= '0' && *(number + i) <= '9'))
            return 2;

    return 0;
}

int compare(const char* number1, const char* number2) {
    if (number1 == NULL || number2 == NULL)
        return 2;
    else if (validate(number1) == 2 || validate(number2) == 2)
        return 3;

    if (*number1 == '-' && *number2 != '-')
        return -1;
    else if (*number1 != '-' && *number2 == '-')
        return 1;

    unsigned int len1 = strlen(number1), len2 = strlen(number2);
    if (len1 > len2 && *number1 != '-' || len1 < len2 && *number1 == '-')
        return 1;
    else if (len1 < len2 && *number2 != '-' || len1 > len2 && *number1 == '-')
        return -1;

    len1 = *number1 == '-';
    unsigned char digit1, digit2;
    while (len1 < len2) {
        digit1 = (int)*(number1 + len1) - '0';
        digit2 = (int)*(number2 + len1) - '0';
        if (digit1 > digit2 && *number1 != '-' || digit1 < digit2 && *number1 == '-')
            return 1;
        else if (digit1 < digit2 && *number1 != '-' || digit1 > digit2 && *number1 == '-')
            return -1;

        len1++;
    }

    return 0;
}

int validate_expression(const char *expr) {
    if (expr == NULL)
        return 2;
    else if (*expr == '\0')
        return 1;

    if (strspn(expr, "+-*0123456789") != strlen(expr))
        return 1;

    const char *ch = expr;
    while ((ch = strpbrk(ch, "+-*")) != NULL) {
        if (ch == expr) {
            if (*ch != '-' || *(++ch) == '\0' || !isdigit(*(ch)))
                return 1;
        }
        else if (*(++ch) == '\0')
            return 1;
        else if (!isdigit(*(ch))) {
            if (*(ch) != '-' || *(ch) == '-' && *(++ch) == '\0' || !isdigit(*(ch)))
                return 1;
        }
    }

    return 0;
}

int calculate(const char* expr, char **result) {
    if (expr == NULL || result == NULL)
        return 1;
    else if (validate_expression(expr))
        return 2;

    const char *ch = expr;
    char *num1, *num2;
    unsigned int len2, len;

    if (*ch == '-')
        len = 1 + strspn(ch + 1, "0123456789");
    else len = strspn(ch, "0123456789");

    *result = malloc((len + 1) * sizeof(char));
    if (*result == NULL)
        return 3;

    strncpy(*result, ch, len);
    *(*result + len) = '\0';

    while ((ch = strpbrk(++ch, "+-*")) != NULL) {
        len = strlen(*result) + 1;

        num1 = malloc(len * sizeof(char));
        if (num1 == NULL)
            return 3;

        strncpy(num1, *result, len);
        free(*result);
        *result = NULL;

        if (*(ch + 1) == '-')
            len2 = 1 + strspn(ch + 2, "0123456789");
        else len2 = strspn(ch + 1, "0123456789");

        num2 = malloc((len2 + 1) * sizeof(char));
        if (num2 == NULL) {
            free(num1);
            num1 = NULL;
            return 3;
        }

        strncpy(num2, ch + 1, len2);
        *(num2 + len2) = '\0';

        int l;
        switch (*ch) {
            case '+':
                l = add(num1, num2, result);
                if (l != 0) {
                    free(num1);
                    free(num2);
                    num1 = NULL;
                    num2 = NULL;
                    return l;
                }
                break;
            case '-':
                l = subtract(num1, num2, result);
                if (l != 0) {
                    free(num1);
                    free(num2);
                    num1 = NULL;
                    num2 = NULL;
                    return l;
                }
                break;
            case '*':
                l = multiply(num1, num2, result);
                if (l != 0) {
                    free(num1);
                    free(num2);
                    num1 = NULL;
                    num2 = NULL;
                    return l;
                }
                break;
        }
        free(num1);
        free(num2);
        num1 = NULL;
        num2 = NULL;
        if (*(ch + 1) == '-')
            ch++;
    }

    return 0;
}
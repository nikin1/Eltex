#include "calc.h"

int my_subtraction(int a, int b) {
    return a - b;
}

int my_multiplication(int a, int b) {
    return a * b;
}

int my_addition(int a, int b) {
    return a + b;
}

int my_division(int a, int b) {
    
    if (b == 0) {
        return 99999999;
    } else {
        return a / b;
    } 
}
#include <stdio.h>
#include <unistd.h>
#include "calc.h"


int main() {
    int flag_to_break = 0, flag_to_continue = 1;

    while (1) {
        flag_to_continue = 0;

        int res = 0;
        printf("В программе доступно 4 функции:\n");
        printf("1.Сложение\n2.Вычитание\n3.Умножение\n4.Деление\n5.Выход из программы\n");
        int a, b, operand;

        printf("Введите номер операции: ");
        scanf("%d", &operand);
        
        if (operand == 5) {
            break;
        }

        printf("Введите 1-е число: ");
        scanf("%d", &a);
        printf("Введите 2-е число: ");
        scanf("%d", &b);


        switch (operand)
        {
        case 1:
            res = my_addition(a, b);
            break;
        case 2:
            res = my_subtraction(a, b);
            break;    
        case 3:
            res = my_multiplication(a, b);
            break;
        case 4:
            res = my_division(a, b);
            break;
        default:
            flag_to_continue = 1;
            printf("Такой операции не существует, напишите число от 1 до 4");
            break;
        }

        if (1 == flag_to_continue) {
            continue;
        } else {
            printf("Результат: %d\n\n\n", res);
            sleep(1);
        }
    }

    return 0;
}

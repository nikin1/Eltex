#include <stdio.h>

#define N 5

#define RIGHT 0
#define DOWN 1
#define LEFT 2
#define UP 3





void print_one_dimesional_array(int Array[N]) {
    for (int j = 0; j < N; j++) {
        printf("%d ", Array[j]);
    }

    printf("\n");
}

void print_array_square(int Array[N][N]) {
    for (int i = 0; i < N; i++) {
        printf("\n");
        for (int j = 0; j < N; j++) {
            printf("%d ", Array[i][j]);
        }
    }
    printf("\n");
}


void task_1(int Array[N][N]) {
    // Вывести квадратную матрицу по заданному N
    // 1 2 3 4 5 
    // 6 7 8 9 10 
    // 11 12 13 14 15 
    // 16 17 18 19 20 
    // 21 22 23 24 25 

    printf("TASK_1\n");
    int cnt = 1;
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            Array[i][j] = cnt;
            cnt++;
        }
    }
}

void task_2_part_1(int Array[N]) {
    // Вывести заданный массив размером N в обратном порядке
    // 0 1 2 3 4 
    // 4 3 2 1 0 


    printf("\n\nTASK_2\n\n");
    int cnt = 0;
    for (int i = 0; i < N; i++) {
        Array[i] = cnt++;
    }

}

void task_2_part_2(int Array[N]) {
    int cnt = 0;
    for (int i = 0; i < (N / 2); i++) {
        int tmp = Array[i];
        Array[i] = Array[N - i - 1];
        Array[N - i - 1] = tmp;                
    }

}



void task_3(int Array[N][N]) {
    // Заполнить верхний треугольник матрицы 1, а нижний 0
    // 0 0 0 0 1 
    // 0 0 0 1 1 
    // 0 0 1 1 1 
    // 0 1 1 1 1 
    // 1 1 1 1 1 

    printf("\n\nTASK_3\n\n");
    int flag = 0;
    for (int i = 0; i < N; i++) {
        flag = 0;
        for (int j = 0; j < N; j++) {
            if (j == (N - 1 - i) ) flag = 1; // Пересекли диагональ (диагональ не главная)
            Array[i][j] = flag;
        }
    }
}


void task_4(int Array[N][N]) {
    // Заполнить матрицу числами от 1 до N^2 улиткой

    // 1 2 3 4 5 
    // 16 17 18 19 6 
    // 15 24 25 20 7 
    // 14 23 22 21 8 
    // 13 12 11 10 9 

    printf("\n\nTASK_4\n\n");
    int route = RIGHT;
    // 0 -> вправо
    // 1 |  вниз
    // 2 <- влево
    // 3 | вверх

    int border_right = N - 1, border_left = 0, border_up = 0, border_down = N - 1;
    int i = 0, j = 0;
    int cnt = 1;



    int max_cnt = N * N + 1;

    while (cnt != max_cnt) {
        // printf("cnt: %d\n", cnt);
        switch (route) {

            case RIGHT:
                while (j != border_right && cnt != max_cnt)
                {
                    Array[i][j++] = cnt++;
                }
                border_up++;
                route = DOWN;
                break;


            case DOWN:
                while (i != border_down && cnt != max_cnt)
                {
                    Array[i++][j] = cnt++;
                }
                border_right--;
                route = LEFT;
                break;

            case LEFT:
                while (j != border_left && cnt != max_cnt)
                {
                    Array[i][j--] = cnt++;
                }
                border_down--;
                route = UP;
                break;

            case UP:
                while (i != border_up && cnt != max_cnt)
                {
                    Array[i--][j] = cnt++;
                }
                border_left++;
                route = RIGHT;
                break;

            default:
                break;
        }
    }
}


int main() {
    printf("N: %d\n\n", N);

    int Array_1[N][N];
    task_1(Array_1);
    print_array_square(Array_1);

    int Array_2[N];
    task_2_part_1(Array_2);
    print_one_dimesional_array(Array_2);
    task_2_part_2(Array_2);
    print_one_dimesional_array(Array_2);

    int Array_3[N][N];
    task_3(Array_3);
    print_array_square(Array_3);

    int Array_4[N][N];
    task_4(Array_4);
    print_array_square(Array_4);

    return 0;
}


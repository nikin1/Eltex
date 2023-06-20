#include <stdio.h>
#include <string.h>

typedef struct phone_book_t {

    char name[100];
    char surname[100];
    char number[100];
    int STATUS;

} ph_book_t;


void add_person(char *name, char *surname, char *number, ph_book_t *book) {
    strcpy(book->name, name);
    strcpy(book->surname, surname);
    strcpy(book->number, number);
    book->STATUS = 1;
}

void remove_person(ph_book_t *book) {
    strcpy(book->name, "");
    strcpy(book->surname, "");
    strcpy(book->number, "");

    book->STATUS = 0;

}

int find_person(char *name, ph_book_t *book) {
    for (int i = 0; i < 100; i++) {
        // const int index = i;
        int status = strcmp(book[i].name, name);
        // if (1 == i) {
        //     printf("name1: %s\nname2: %s\nstatus: %d\n", book[1].name, name, status);
        // }

        if (0 == status) {
            // printf("<<<>>>\n");
            printf("Name: %s\nSurname: %s\nNumber: %s\n", book[i].name, book[i].surname, book[i].number);

            return i;
        }

    }

    return -1;
}

void print_book(ph_book_t *book) {
    printf("   Name%17cSurname%14cNumber\n", ' ', ' ');
    // Name%17c - Name и 17 пробелов
    // Surname%14c - Surname и 14 пробелов 

    int cnt = 1;
    for (int i = 0; i < 100; i++) {
        if (1 == book[i].STATUS) {
            printf("%d. %-20s %-20s %-20s\n", cnt, book[i].name, book[i].surname, book[i].number);
            // %-20s . Длина строки - 20 сим. + вырав по лев краю
            cnt++;
        }
    }
}

int main() {
    ph_book_t book[100];
    // add_person("Nikita", "Shapoval", "+7 999 999 99 00", &book[3]);

    // printf("Name: %s\nSurname: %s\nNumber: %s\n", book[3].name, book[3].surname, book[3].number);

    // remove_person(&book[1]);
    // printf("\n_______\n\n");
    // printf("Name: %s\nSurname: %s\nNumber: %s\n", book[1].name, book[1].surname, book[1].number);

    // int find_status = find_person("Nikita", book);
    // printf("find_status: %d\n", find_status);

    // print_book(book);

    // printf("Hello, world!\n");
    int INDEX = 0;
    while(1) {
        int tmp;
        int flag_break = 0;
        
        printf("Выберите одну из функций: \n");
        printf("1.добавление абонента в телефонную книгу;\n2.удаление абонента;\n3.поиск абонента;\n4.вывод всей телефонной книги;\n5.выход из программы.\n");
        printf(": ");

        scanf("%d", &tmp);
        
        char name[100];
        char surname[100];
        char number[100];
        
        int number_for_delete;
        
        char name_for_search[100];
        
        switch (tmp)
        {
        case 1:
            
            printf("Введите имя: ");
            scanf("%s", name);

            printf("Введите фамилию: ");
            scanf("%s", surname);

            printf("Номер телефона: ");
            scanf("%s", number);

            add_person(name, surname, number, &book[INDEX++]);

            break;
        case 2:
            printf("Какой объект вы хотите удалить?\n");
            printf("Введите число от 1 до 100: ");
            scanf("%d", &number_for_delete);
            if (number_for_delete >= 1 && number_for_delete <= 100) {
                remove_person(&book[number_for_delete - 1]);
            } else {
                printf("Некорректный ввод...\n");
            }

            break;
        case 3:
            printf("Напишите имя абнонента: ");
            scanf("%s", name_for_search);

            find_person(name_for_search, book);

            break;
        case 4:
            print_book(book);
            break;
        case 5:
            flag_break = 1;                        
            break;    
        default:
            printf("Некорректный ввод, выберите снова...\n");
            break;
        }

        if (flag_break) break;
    }


    return 0;
}


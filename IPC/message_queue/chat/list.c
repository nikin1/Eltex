#include "chat.h"

// extern int GLOBAL_LIST_LEN;

list_t *Create(int pid, char *str) {
    list_t *HEAD;
    HEAD = malloc(sizeof(list_t));

    strcpy(HEAD->name, str);
    HEAD->pid = pid;

    

    GLOBAL_LIST_LEN = 1;
    return HEAD;
}

list_t *Append(list_t *A, int pid, char *str) {
    list_t *element, *tmp;
    element = malloc(sizeof(list_t));
    assert(element != NULL);

    tmp = A->next; // сохранение указателя на следующий узел    < ----------
    A->next = element; // Собственно добавляем элемент                     |
    element->pid = pid; // сохранение поля данных добавляемого узла        |
    strcpy(element->name, str);
    element->next = tmp; // добавляем узел из ------------------------------

    GLOBAL_LIST_LEN += 1;
    return element;
}

list_t *Remove(list_t *A, list_t *head) {
    assert(A != NULL);
    list_t *tmp = head;
    while (tmp->next != A) {
        tmp = tmp->next;
    }   
    tmp->next = A->next;
    free(A);
    GLOBAL_LIST_LEN -= 1;

    return tmp;
}

int GetHead(list_t *A) {
    assert(A != NULL);
    return A->value;
}

list_t* GetTail(list_t *A) {
    assert(A != NULL);
    return A->next;
}

int IsEmpty(list_t *A) {
    if (A == NULL) return 1;
    else return 0;
}

int check_set(int *set, int index, int x) {
    for (int i = 0; i < index; i++) {
        if (set[i] == x) {
            return 1;
        }
    }
    return 0;
}

void DelDuplic(list_t *head) {
    int *set;
    set = malloc(1024 * sizeof(int));
    list_t *a = head;
    int i = 0;
    while (!IsEmpty(a)) {
        if (check_set(set, i, GetHead(a)) == 1) { // если такое значение уже есть в нашем множетсве            
            Remove(a, head);
        } else {
            set[i++] = GetHead(a);
        }
        a = GetTail(a);
    }
    free(set);
}


int Len_List(list_t *H) {
    list_t *i;
    i = H;
    int len = 0;
    while (!IsEmpty(i)) {        
        len++;
        i = GetTail(i);
    }
    return len;
}

list_t *Search_Element(list_t *H, int k) {
    int len = Len_List(H);    
    list_t *a = NULL;
    a = H;
    for (int i = 0; i < len - k; i++) {
        a = GetTail(a);
    }
    return a;
}

void Copy_Data_out_MSG_BUF(list_t *dest, struct msgbuf source) {
    if (dest != NULL ) {
        dest->pid = source.mpid;
        strcpy(dest->name, source.mtext);
    }
}


// int main() {
    // Тестирование функций

    // list_t *head = Create();
    // head->value = 1;
    // Append(head, 2);
    // Append(head, 3);    
    // int len = Len_List(head);
    // assert(len == 3);    

    // list_t *tmp;
    // assert(IsEmpty(tmp) == 1);

    // list_t *A = head;
    // A = GetTail(A);
    // assert(A->value == 3);

    // A = GetTail(A);
    // assert(GetHead(A) == 2);

    // Append(head, 4); // 1 4 3 2
    // A = head;
    // A = GetTail(A);
    // A = Remove(A, head); // 1 3 2 
    // A = GetTail(A);
    // assert(A->value == 3);

    
    // A = GetTail(A); // 2
    // A = Remove(A, head); // 1 3  
    // A = Remove(A, head); // 1
    // A = GetTail(A); // NULL
    // assert(IsEmpty(A) == 1);

    // // Search_Element
    // A = head;
    // Append(head, 2); // 1 2 
    // A = A->next;
    // Append(A, 3); // 1 2 3
    // A = A->next;
    // Append(A, 2); // 1 2 3 2
    // A = A->next;
    // Append(A, 1); // 1 2 3 2 1
    // A = A->next; // 1
    // assert(Search_Element(head, 1) == A);
    // A = head;
    // A = A->next;  // 2
    // A = A->next;  // 3
    // assert(Search_Element(head, 3) == A);

    // // Polidrom
    // assert(Check_Polindrom(head) == 1);
    
    // // DEL_DUPLICATE
    // DelDuplic(head);    
    // assert(Len_List(head) == 3);

	// return 0;
// }


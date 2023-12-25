#include "chat.h"


list_t *Create(int pid, char *str) {
    list_t *HEAD;
    HEAD = malloc(sizeof(list_t));

    strcpy(HEAD->name, str);
    HEAD->pid = pid;

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

list_t *Search_Element_on_PID(list_t *H, int pid) {
    int len = Len_List(H);
    list_t *a = NULL;
    a = H;
    for (int i = 0; i < len; i++) {
        if (a->pid == pid) {
            break;
        }
        a = GetTail(a);
    }
    return a;
}

void Copy_Data_out_MSG_BUF(list_t *dest, struct my_msgbuf source) {
    if (dest != NULL ) {
        dest->pid = source.mpid;
        strcpy(dest->name, source.mtext);
    }
}


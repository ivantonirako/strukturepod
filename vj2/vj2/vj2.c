#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 10 

typedef struct person* Ptr;
typedef struct person {
    char name[N];
    char surname[N];
    int year;
    Ptr next;
} Person;

int addFirst(Ptr);
int addLast(Ptr);
int printList(Ptr);
Ptr findPerson(Ptr);
Ptr findBefore(char[], Ptr);
int deletePerson(Ptr);
int enterData(Ptr);

int main() {
    Person head = { "", "", 0, NULL };
    Ptr found = NULL;

    // add 3 people: 2 to start, 1 to end
    addFirst(&head);
    addFirst(&head);
    addLast(&head);

    // find by surname
    found = findPerson(&head);

    // print all
    printList(&head);

    // delete by surname and print again
    deletePerson(&head);
    printList(&head);

    return 0;
}

// add new person to start of list
int addFirst(Ptr start) {
    Ptr p = (Ptr)malloc(sizeof(Person));
    if (p == NULL) {
        printf("Error: memory not allocated.\n");
        return -1;
    }

    enterData(p);          // fill info
    p->next = start->next; // link new person
    start->next = p;

    return 0;
}

// add new person to end of list
int addLast(Ptr start) {
    while (start->next != NULL)
        start = start->next;

    Ptr p = (Ptr)malloc(sizeof(Person));
    if (p == NULL) {
        printf("Error: memory not allocated.\n");
        return -1;
    }

    enterData(p);
    p->next = start->next;
    start->next = p;

    return 0;
}

// print all people
int printList(Ptr start) {
    start = start->next; // skip head

    while (start != NULL) {
        printf("%s %s %d (%p)\n", start->name, start->surname, start->year, start);
        start = start->next;
    }

    return 0;
}

// find a person by surname
Ptr findPerson(Ptr start) {
    char* s = (char*)malloc(100 * sizeof(char));

    do {
        printf("Enter surname to find: ");
        scanf("%s", s);
        while (getchar() != '\n');
    } while (strlen(s) > 9);

    while (start != NULL && strcmp(start->surname, s) != 0)
        start = start->next;

    if (start == NULL)
        printf("Not found.\n");
    else
        printf("Found at address: %p\n", start);

    free(s);
    return start;
}

// find node before one with given surname
Ptr findBefore(char sname[N], Ptr start) {
    Ptr prev = NULL;

    while (start != NULL && strcmp(start->surname, sname) != 0) {
        prev = start;
        start = start->next;
    }

    if (start == NULL)
        return NULL;

    return prev;
}

// delete a person by surname
int deletePerson(Ptr start) {
    char* s = (char*)malloc(100 * sizeof(char));

    do {
        printf("Enter surname to delete: ");
        scanf("%s", s);
        while (getchar() != '\n');
    } while (strlen(s) > 9);

    start = findBefore(s, start);
    if (start == NULL) {
        free(s);
        return -1;
    }

    Ptr del = start->next;
    start->next = del->next;

    free(del);
    free(s);
    return 0;
}

// enter info for one person
int enterData(Ptr p) {
    char* temp = (char*)malloc(100 * sizeof(char));
    int y = -1;

    do {
        printf("Enter name: ");
        scanf("%s", temp);
        while (getchar() != '\n');
    } while (strlen(temp) > 9);
    strcpy(p->name, temp);

    do {
        printf("Enter surname: ");
        scanf("%s", temp);
        while (getchar() != '\n');
    } while (strlen(temp) > 9);
    strcpy(p->surname, temp);

    do {
        printf("Enter birth year: ");
        scanf_s("%d", &y);
        while (getchar() != '\n');
    } while (y < 0 || y > 2025);
    p->year = y;

    free(temp);
    return 0;
}

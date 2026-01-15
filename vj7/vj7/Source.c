#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define N 20

/* struktura direktorija */
typedef struct directory* dirPosition;
typedef struct directory {
    char ime[N];
    dirPosition Next;
    dirPosition subDirPosition;
} Directory;

/* struktura stoga */
typedef struct stack* stackPosition;
typedef struct stack {
    stackPosition Next;
    dirPosition dirLevel;
} Stack;

/* prototipi funkcija */
int GoBack(dirPosition, stackPosition);
int deleteStack(stackPosition);
int delete(dirPosition);
int deleteChild(dirPosition);
int dir(dirPosition);
int cdTT(stackPosition);
int cd(dirPosition, stackPosition, char*);
int push(stackPosition, dirPosition);
int addDir(dirPosition, stackPosition);
int mkdir(dirPosition, char*);
int out(dirPosition);

int main()
{
    Directory headDirectory = { "", NULL, NULL };
    Stack headStack = { NULL, NULL };

    /* dodavanje dva diska */
    addDir(&headDirectory, &headStack);
    addDir(&headDirectory, &headStack);

    int active = 1;

    /* uklanjanje pocetnog elementa sa stoga */
    cdTT(&headStack);

    char* command = malloc(sizeof(char) * 7);
    char* name = malloc(sizeof(char) * N);

    while (active) {

        /* ako je stog prazan, korisnik bira disk */
        if (headStack.Next == NULL) {
            GoBack(headDirectory.Next, &headStack);
        }
        else {
            printf("Upisi: mkdir | cd | cd.. | dir | EXIT\n");
            printf("%s> ", headStack.Next->dirLevel->ime);

            scanf("%s", command);

            if (strcmp(command, "mkdir") == 0) {
                scanf("%s", name);
                mkdir(headStack.Next->dirLevel, name);
                system("cls");
            }
            else if (strcmp(command, "cd") == 0) {
                scanf("%s", name);
                cd(headStack.Next->dirLevel, &headStack, name);
                system("cls");
            }
            else if (strcmp(command, "cd..") == 0) {
                cdTT(&headStack);
                system("cls");
            }
            else if (strcmp(command, "dir") == 0) {
                system("cls");
                dir(headStack.Next->dirLevel);
            }
            else if (strcmp(command, "EXIT") == 0) {
                out(headDirectory.Next);
                delete(&headDirectory);
                deleteStack(&headStack);
                active = 0;
            }
            else {
                system("cls");
            }
        }
    }

    return 0;
}

/* ispis diskova i povratak u odabrani disk */
int GoBack(dirPosition Start, stackPosition StackStart)
{
    dirPosition P = Start;

    while (P != NULL) {
        printf("%s\n", P->ime);
        P = P->Next;
    }

    int found = 0;
    P = Start;

    while (!found) {
        char* input = malloc(sizeof(char) * N);
        printf("U koji disk ces se vratiti: ");
        scanf("%s", input);

        while (P != NULL) {
            if (strcmp(input, P->ime) == 0) {
                push(StackStart, P);
                found = 1;
                break;
            }
            P = P->Next;
        }
    }

    return 0;
}

/* brisanje cijelog stoga */
int deleteStack(stackPosition Start)
{
    stackPosition temp;

    while (Start->Next != NULL) {
        temp = Start->Next;
        Start->Next = temp->Next;
        temp->Next = NULL;
        free(temp);
    }

    return 0;
}

/* brisanje svih direktorija */
int delete(dirPosition Start)
{
    dirPosition temp;

    while (Start->Next != NULL) {
        deleteChild(Start->subDirPosition);
        temp = Start->Next;
        Start->Next = temp->Next;
        temp->Next = NULL;
        free(temp);
    }

    return 0;
}

/* rekurzivno brisanje poddirektorija */
int deleteChild(dirPosition current)
{
    if (current == NULL)
        return 0;

    dirPosition currentSub = current->subDirPosition;
    dirPosition temp;

    while (currentSub != NULL) {
        deleteChild(currentSub);
        temp = currentSub->Next;
        currentSub->Next = temp->Next;
        temp->Next = NULL;
        free(temp);
    }

    return 0;
}

/* kreiranje novog direktorija */
int mkdir(dirPosition P, char* input)
{
    dirPosition Parent = P;
    dirPosition newEl = malloc(sizeof(Directory));

    if (newEl == NULL)
        return -1;

    strcpy(newEl->ime, input);
    newEl->Next = Parent->subDirPosition;
    newEl->subDirPosition = NULL;
    Parent->subDirPosition = newEl;

    return 0;
}

/* ispis cijelog stabla direktorija */
int out(dirPosition Start)
{
    if (Start == NULL) {
        printf("Obrisano je\n");
        return 0;
    }

    while (Start != NULL) {
        printf("%s\n", Start->ime);
        dir(Start);
        Start = Start->Next;
    }

    return 0;
}

/* ispis direktorija rekurzivno */
int dir(dirPosition current)
{
    int index = 1;
    dirPosition currentSub = current->subDirPosition;

    while (currentSub != NULL) {
        printf("%d%s\n", index++, currentSub->ime);
        dir(currentSub);
        currentSub = currentSub->Next;
    }

    return 0;
}

/* izlazak iz trenutnog direktorija (pop) */
int cdTT(stackPosition stack)
{
    stackPosition temp = stack->Next;

    stack->Next = temp->Next;
    temp->Next = NULL;
    free(temp);

    return 0;
}

/* ulazak u poddirektorij */
int cd(dirPosition P, stackPosition stack, char* input)
{
    dirPosition temp = P->subDirPosition;

    while (temp != NULL) {
        if (strcmp(input, temp->ime) == 0) {
            push(stack, temp);
            printf("FOUND\n");
            return 0;
        }
        temp = temp->Next;
    }

    printf("Not FOUND\n");
    return 0;
}

/* dodavanje direktorija na stog */
int push(stackPosition StackStart, dirPosition Q)
{
    stackPosition P = malloc(sizeof(Stack));

    P->Next = StackStart->Next;
    StackStart->Next = P;
    P->dirLevel = Q;

    return 0;
}

/* dodavanje novog diska */
int addDir(dirPosition head, stackPosition stack)
{
    dirPosition Start = head;
    dirPosition Q = malloc(sizeof(Directory));

    if (Q == NULL)
        return -1;

    char* p = malloc(sizeof(char) * 10);
    printf("Upisi ime diska: ");
    scanf("%s", p);

    strcpy(Q->ime, p);

    while (Start->Next != NULL)
        Start = Start->Next;

    Q->Next = Start->Next;
    Start->Next = Q;
    Q->subDirPosition = NULL;

    push(stack, Q);

    return 0;
}
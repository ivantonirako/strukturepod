#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define N 10
#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1

// Linked list node used as a stack element
typedef struct postfix* Position;
typedef struct postfix {
    int number;
    Position Next;
} postfix;

// Function prototypes
int fileCheck();
int fileRead(Position);
int push(Position, int);
int pop(Position, Position, char);
int output(Position);

int main() {

    postfix Head = { 0, NULL };

    // Step 1: Verify that the file exists and contains a valid postfix expression
    if (fileCheck() == EXIT_FAILURE) {
        printf("Invalid or missing postfix file.\n");
        return EXIT_FAILURE;
    }

    // Step 2: Read the file and evaluate the postfix expression using a stack
    if (fileRead(&Head) == EXIT_FAILURE) {
        printf("Failed to evaluate expression.\n");
        return EXIT_FAILURE;
    }

    // Step 3: Display the final result
    output(Head.Next);

    return EXIT_SUCCESS;
}

/*
    Ensures only numbers and allowed operators are used,
    and that the postfix structure is mathematically correct
*/
int fileCheck() {
    FILE* f = fopen("postfix.txt", "r");
    if (f == NULL) {
        printf("Error: cannot open file.\n");
        return EXIT_FAILURE;
    }

    char* c = (char*)malloc(sizeof(char) * 10);
    int numberCount = 0;
    int operatorCount = 0;
    int temp;

    // Read through the entire file
    while (!feof(f)) {
        fscanf(f, "%s", c);
        printf("%s ", c);

        // Check if current token is a number
        if (sscanf(c, "%d", &temp) == 1) {
            numberCount++;
        }
        // Check if it’s one of the valid math operators
        else if (*c == '+' || *c == '-' || *c == '*' || *c == '/' || *c == '%') {
            operatorCount++;
        }
        else {
            // Found an invalid character or symbol
            free(c);
            fclose(f);
            return EXIT_FAILURE;
        }
    }

    // Validate the postfix structure
    if (operatorCount + 1 != numberCount) {
        free(c);
        fclose(f);
        return EXIT_FAILURE;
    }

    free(c);
    fclose(f);
    return EXIT_SUCCESS;
}

/*
    Numbers are pushed onto the stack, while operators pop and process the top elements.
*/
int fileRead(Position Start) {
    FILE* f = fopen("postfix.txt", "r");
    if (f == NULL) {
        return EXIT_FAILURE;
    }

    Position Q = Start;
    char* c = (char*)malloc(sizeof(char) * 10);
    int numberCount = 0;
    int temp;

    while (!feof(f)) {
        fscanf(f, "%s", c);

        // If token is a number, push it to the stack
        if (sscanf(c, "%d", &temp) == 1) {
            numberCount++;
            push(Q, temp);
        }
        else {
            // If we have at least two numbers, perform the operation
            if (numberCount > 1) {
                pop(Q->Next, Q->Next->Next, *c);
                numberCount--;
            }
            else {
                // Not enough numbers to perform operation - invalid postfix
                printf("Error: invalid postfix expression.\n");
                free(c);
                fclose(f);
                return EXIT_FAILURE;
            }
        }
    }

    free(c);
    fclose(f);
    return EXIT_SUCCESS;
}

/*
    Pushes a new number onto the stack (inserts it at the front of the list).
*/
int push(Position Start, int num) {
    Position Q = (Position)malloc(sizeof(postfix));
    if (Q == NULL) {
        printf("Memory allocation failed.\n");
        return EXIT_FAILURE;
    }

    Q->number = num;
    Q->Next = Start->Next;
    Start->Next = Q;

    return EXIT_SUCCESS;
}

/*
    The result replaces the first element, and the second is removed.
*/
int pop(Position First, Position Second, char c) {
    switch (c) {
    case '+':
        First->number = First->number + Second->number;
        break;
    case '-':
        First->number = First->number - Second->number;
        break;
    case '*':
        First->number = First->number * Second->number;
        break;
    case '/':
        First->number = First->number / Second->number;
        break;
    case '%':
        First->number = First->number % Second->number;
        break;
    default:
        printf("Unknown operator: %c\n", c);
        return EXIT_FAILURE;
    }

    // Remove the second node from the list
    First->Next = Second->Next;
    free(Second);

    return EXIT_SUCCESS;
}

/*
    Prints the final result stored in the stack and frees the allocated memory.
*/
int output(Position Q) {
    if (Q == NULL) {
        printf("Error: stack is empty.\n");
        return EXIT_FAILURE;
    }

    printf("\nResult: %d\n", Q->number);

    free(Q);
    return EXIT_SUCCESS;
}

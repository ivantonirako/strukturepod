#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

#define EXIT_SUCCESS 0
#define EXIT_FAILURE -1

typedef struct poly* Position;

typedef struct poly {
    int coef;      
    int pow;       
    Position next;
} Poly;

int readFile(Position, int);
int addPoly(Position, Position, Position);
int multiplyPoly(Position, Position, Position);
int printPoly(Position);

int main() {
    int check;

    Poly HeadSum = { 0, 0, NULL };
    Poly HeadMul = { 0, 0, NULL };

    Poly first = HeadSum;
    Poly second = HeadSum;

  
    check = readFile(&first, 1);
    if (check == EXIT_FAILURE) return EXIT_FAILURE;

    check = readFile(&second, 2);
    if (check == EXIT_FAILURE) return EXIT_FAILURE;

    
    addPoly(&first, &second, &HeadSum);
    multiplyPoly(&first, &second, &HeadMul);

    
    printPoly(&first);
    printPoly(&second);
    printPoly(&HeadSum);
    printPoly(&HeadMul);

    return EXIT_SUCCESS;
}


int multiplyPoly(Position first, Position second, Position start) {
    Position p1 = first->next;
    Position p2 = second->next;
    Position temp = start;

    // The largest possible power is the sum of the highest powers in both polynomials
    int maxPow = p1->pow + p2->pow;
    int copyMax = maxPow;

    // Initialize result list with all powers (from max to 0) and coefficients = 0
    for (; copyMax >= 0; copyMax--) {
        Position newNode = (Position)malloc(sizeof(Poly));
        if (!newNode) {
            printf("Memory error\n");
            return EXIT_FAILURE;
        }

        while (temp->next != NULL)
            temp = temp->next;

        newNode->coef = 0;
        newNode->pow = copyMax;
        newNode->next = temp->next;
        temp->next = newNode;
    }

    // Multiply each term of the first polynomial by each term of the second
    for (p2 = second->next; p2 != NULL; p2 = p2->next) {
        for (p1 = first->next; p1 != NULL; p1 = p1->next) {
            int mul = p1->coef * p2->coef;
            int powerSum = p1->pow + p2->pow;

            // Find matching power in result list and add the product
            for (temp = start->next; temp != NULL; temp = temp->next) {
                if (temp->pow == powerSum) {
                    temp->coef += mul;
                    break;
                }
            }
        }
    }

    return EXIT_SUCCESS;
}

/* Adds two polynomial lists and stores result in Start */
int addPoly(Position first, Position second, Position start) {
    Position p1 = first->next;
    Position p2 = second->next;
    Position temp = start;
    int maxPow;

    // Find the largest power among both polynomials
    maxPow = (p1->pow >= p2->pow) ? p1->pow : p2->pow;

    // Loop through all powers down to 0
    for (; maxPow >= 0; maxPow--) {
        int coef1 = 0, coef2 = 0;

        // Search for matching power in first polynomial
        for (p1 = first->next; p1 != NULL; p1 = p1->next) {
            if (p1->pow == maxPow) {
                coef1 = p1->coef;
                break;
            }
        }

        // Search for matching power in second polynomial
        for (p2 = second->next; p2 != NULL; p2 = p2->next) {
            if (p2->pow == maxPow) {
                coef2 = p2->coef;
                break;
            }
        }

        // Skip if coefficients cancel each other
        int sum = coef1 + coef2;
        if (sum == 0)
            continue;

        // Create a new node for the sum
        Position newNode = (Position)malloc(sizeof(Poly));
        if (!newNode) {
            printf("Memory error\n");
            return EXIT_FAILURE;
        }

        while (temp->next != NULL)
            temp = temp->next;

        newNode->coef = sum;
        newNode->pow = maxPow;
        newNode->next = NULL;
        temp->next = newNode;
    }

    return EXIT_SUCCESS;
}

/* Reads a polynomial from a file into a sorted linked list */
int readFile(Position start, int fileNum) {
    Position temp = start;
    FILE* f;

    // Choose which file to open
    if (fileNum == 1)
        f = fopen("prvi.txt", "r");
    else
        f = fopen("drugi.txt", "r");

    if (!f)
        return EXIT_FAILURE;

    char* buffer = (char*)malloc(sizeof(char) * 10);
    int coefTemp, powTemp;

    // Read file until end
    while (!feof(f)) {
        Position newNode = (Position)malloc(sizeof(Poly));
        if (!newNode) {
            printf("Memory error\n");
            return EXIT_FAILURE;
        }

        // Read coefficient
        fscanf(f, "%s", buffer);
        if (sscanf(buffer, "%d", &coefTemp) == 0)
            return EXIT_FAILURE;

        // Read power
        fscanf(f, "%s", buffer);
        if (sscanf(buffer, "%d", &powTemp) == 0)
            return EXIT_FAILURE;

        // Insert in descending order by power
        Position p;
        for (p = temp; p->next != NULL; p = p->next) {
            if (powTemp > p->next->pow)
                break;
        }

        newNode->coef = coefTemp;
        newNode->pow = powTemp;
        newNode->next = p->next;
        p->next = newNode;
    }

    free(buffer);
    fclose(f);
    return EXIT_SUCCESS;
}

/* Prints a polynomial and frees its allocated nodes */
int printPoly(Position start) {
    Position p = start->next;

    // Print all terms
    while (p != NULL) {
        printf("%dx%d ", p->coef, p->pow);
        p = p->next;
    }
    printf("\n");

    // Delete all nodes (except head)
    p = start;
    Position temp;
    while (p->next != NULL) {
        temp = p->next;
        p->next = temp->next;
        free(temp);
    }

    return EXIT_SUCCESS;
}

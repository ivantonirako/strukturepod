#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SIZE 10   // Maximum number of characters for names

// Define a struct for a person and a pointer type for it
typedef struct person* Ptr;
typedef struct person {
    char firstName[SIZE];
    char lastName[SIZE];
    int year;
    Ptr next;     // Pointer to the next person in the linked list
} Person;

// Function declarations
int addPerson(Ptr);
int addAtStart(Ptr);
int addAtEnd(Ptr);
int printList(Ptr);
Ptr findPerson(char*, Ptr);
Ptr findBeforePerson(char*, Ptr);
int removePerson(Ptr);
int insertAfter(Ptr);
int insertBefore(Ptr);
int sortList(Ptr);
int saveToFile(Ptr);
int readFromFile(Ptr);

int main() {
    // Create a "head" node that serves as the start of the linked list
    Person head = { "", "", 0, NULL };
    int choice = 0;

    // Ask the user how they want to input data
    printf("Choose input method:\n1 - Manual input\n2 - Read from file\n");
    scanf("%d", &choice);

    // Option 1: manually add people
    if (choice == 1) {
        addAtStart(&head);
        addAtStart(&head);
        addAtEnd(&head);

        // Delete a person and print the list
        removePerson(&head);
        printList(&head);

        // Add one person after and one before a specific last name
        insertAfter(&head);
        insertBefore(&head);
    }
    // Option 2: read people from a text file
    else if (choice == 2) {
        readFromFile(&head);
    }
    // Invalid input
    else {
        printf("Invalid option!\n");
        return -1;
    }

    // Display the list
    printList(&head);

    // Sort the list alphabetically by last name
    printf("Sorting...\n");
    sortList(&head);

    // Display again after sorting
    printList(&head);

    // Save final list to file
    saveToFile(head.next);

    return 0;
}

// ---------------------- Save to file ----------------------
int saveToFile(Ptr start) {
    // Open output file for writing
    FILE* file = fopen("Ispis.txt", "w");
    if (!file) return -1;

    // Write all people to the file
    while (start != NULL) {
        fprintf(file, "%s %s %d %p\n", start->firstName, start->lastName, start->year, start);
        start = start->next;
    }

    fclose(file); // Close file
    return 0;
}

// ---------------------- Read from file ----------------------
int readFromFile(Ptr start) {
    // Open input file for reading
    FILE* file = fopen("Upis.txt", "r");
    if (!file) return -1;

    char buffer[SIZE];
    int temp = 0;

    // Read until end of file
    while (!feof(file)) {
        // Allocate memory for a new person node
        Ptr node = (Ptr)malloc(sizeof(Person));
        if (!node) return -1;
        node->next = NULL;

        // Read name, last name, and year from the file
        fscanf(file, "%s", buffer);
        strcpy(node->firstName, buffer);

        fscanf(file, "%s", buffer);
        strcpy(node->lastName, buffer);

        fscanf(file, "%d", &temp);
        node->year = temp;

        // Move to the end of the list
        while (start->next != NULL)
            start = start->next;

        // Attach new node to the end
        start->next = node;
    }

    fclose(file);
    return 0;
}

// ---------------------- Sort list alphabetically ----------------------
int sortList(Ptr start) {
    Ptr prev, curr, nextNode;
    int swapped;

    // Keep looping until no swaps are needed
    do {
        swapped = 0;
        prev = start;

        // Traverse the list and compare adjacent last names
        while (prev->next != NULL && prev->next->next != NULL) {
            curr = prev->next;
            nextNode = curr->next;

            // If two last names are out of order, swap their links
            if (strcmp(curr->lastName, nextNode->lastName) > 0) {
                curr->next = nextNode->next;
                nextNode->next = curr;
                prev->next = nextNode;
                swapped = 1; // Mark that a swap occurred
            }
            prev = prev->next;
        }
    } while (swapped); // Repeat until fully sorted

    return 0;
}

// ---------------------- Insert after a given last name ----------------------
int insertAfter(Ptr start) {
    // Allocate memory for a new node
    Ptr newNode = (Ptr)malloc(sizeof(Person));
    if (!newNode) return -1;

    char input[100];
    // Ask for the last name to insert after
    do {
        printf("Enter last name to insert AFTER: ");
        scanf("%s", input);
        while (getchar() != '\n');
    } while (strlen(input) > 9);

    // Find that person in the list
    start = findPerson(input, start);
    if (!start) return -1;

    // Fill in data for the new person
    addPerson(newNode);

    // Insert new node after the found one
    newNode->next = start->next;
    start->next = newNode;

    return 0;
}

// ---------------------- Insert before a given last name ----------------------
int insertBefore(Ptr start) {
    Ptr newNode = (Ptr)malloc(sizeof(Person));
    if (!newNode) return -1;

    char input[100];
    // Ask for the last name to insert before
    do {
        printf("Enter last name to insert BEFORE: ");
        scanf("%s", input);
        while (getchar() != '\n');
    } while (strlen(input) > 9);

    // Find the node before the one with that last name
    start = findBeforePerson(input, start);
    if (!start) return -1;

    // Fill in data for the new person
    addPerson(newNode);

    // Insert it before the chosen person
    newNode->next = start->next;
    start->next = newNode;

    return 0;
}

// ---------------------- Add person to the start ----------------------
int addAtStart(Ptr start) {
    Ptr node = (Ptr)malloc(sizeof(Person));
    if (!node) return -1;

    addPerson(node); // Fill data

    // Insert node immediately after the head
    node->next = start->next;
    start->next = node;
    return 0;
}

// ---------------------- Add person to the end ----------------------
int addAtEnd(Ptr start) {
    // Move to last node
    while (start->next != NULL)
        start = start->next;

    Ptr node = (Ptr)malloc(sizeof(Person));
    if (!node) return -1;

    addPerson(node);
    node->next = NULL;
    start->next = node;
    return 0;
}

// ---------------------- Print all people ----------------------
int printList(Ptr start) {
    // Skip dummy head node
    start = start->next;

    // Print until end of list
    while (start != NULL) {
        printf("%s %s %d %p\n", start->firstName, start->lastName, start->year, start);
        start = start->next;
    }
    return 0;
}

// ---------------------- Find a person by last name ----------------------
Ptr findPerson(char* key, Ptr start) {
    // Go through list until match is found or list ends
    while (start != NULL && strcmp(start->lastName, key) != 0)
        start = start->next;

    if (start == NULL)
        printf("Not found.\n");
    else
        printf("Found at address: %p\n", start);

    return start;
}

// ---------------------- Find node before a specific last name ----------------------
Ptr findBeforePerson(char* key, Ptr start) {
    Ptr prev = NULL;

    // Move through list, keeping track of the previous node
    while (start != NULL && strcmp(start->lastName, key) != 0) {
        prev = start;
        start = start->next;
    }

    // Return the node before the match, or NULL if not found
    return (start == NULL) ? NULL : prev;
}

// ---------------------- Remove a person by last name ----------------------
int removePerson(Ptr start) {
    char input[100];

    // Ask user which person to remove
    do {
        printf("Enter last name to delete: ");
        scanf("%s", input);
        while (getchar() != '\n');
    } while (strlen(input) > 9);

    // Find the node before the one we want to delete
    start = findBeforePerson(input, start);
    if (!start || !start->next) return -1;

    // Delete the node
    Ptr toDelete = start->next;
    start->next = toDelete->next;

    free(toDelete);
    return 0;
}

// ---------------------- Fill in details for a new person ----------------------
int addPerson(Ptr node) {
    char input[100];
    int yearValue;

    // Get first name
    do {
        printf("Enter first name: ");
        scanf("%s", input);
        while (getchar() != '\n');
    } while (strlen(input) > 9);
    strcpy(node->firstName, input);

    // Get last name
    do {
        printf("Enter last name: ");
        scanf("%s", input);
        while (getchar() != '\n');
    } while (strlen(input) > 9);
    strcpy(node->lastName, input);

    // Get birth year (must be between 0 and 2025)
    do {
        printf("Enter birth year: ");
        scanf("%d", &yearValue);
        while (getchar() != '\n');
    } while (yearValue < 0 || yearValue > 2025);
    node->year = yearValue;

    return 0;
}

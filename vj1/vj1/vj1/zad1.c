#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h> 

#define N 10  


typedef struct {
    char name[N];
    char surname[N];
    int points;
} Student;


int countRows();
int loadData(Student* list);
int printStudents(Student* list, int rows);

int main() {
    // find out how many rows (students) are in the file
    int rows = countRows();

    // allocate enough memory for all students
    Student* list = malloc(rows * sizeof(Student));
    if (list == NULL) {
        printf("Memory allocation failed.\n");
        return 1;
    }

    // load students from file into array
    loadData(list);

    // print all students
    printStudents(list, rows);

    free(list);

    return 0;
}


// Counts how many lines are in the file
int countRows() {
    FILE* file = fopen("C:\\Users\\ivant\\OneDrive\\Desktop\\struktf\\vj1\\vj1\\vj1\\studenti.txt", "r");

    // if file failed to open, return 0
    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int rowCount = 1;  // start from 1 since the last line ends with EOF, not newline
    char c;

    // read every character until end of file
    while (!feof(file)) {
        c = fgetc(file);

        // if the character is a newline, count one more row
        if (c == '\n') {
            rowCount++;
        }
    }

    fclose(file);
    return rowCount;
}

// Reads all students from the file and stores them in an array
int loadData(Student* list) {
    FILE* file = fopen("C:\\Users\\ivant\\OneDrive\\Desktop\\struktf\\vj1\\vj1\\vj1\\studenti.txt", "r");

    if (file == NULL) {
        printf("Error opening file.\n");
        return 0;
    }

    int index = 0;

    // while not end of file, read data for each student
    while (!feof(file)) {
        fscanf(file, "%s", (list + index)->name);
        fscanf(file, "%s", (list + index)->surname);
        fscanf(file, "%d", &(list + index)->points);
        index++;
    }

    fclose(file);
    return 0;
}


// Prints all student data with calculated relative points
int printStudents(Student* list, int rows) {
    int maxPoints = 100;  // assume full points = 100

    for (int i = 0; i < rows; i++) {
        // calculate relative points as percentage
        double relPoints = (double)(list + i)->points * maxPoints / 100;

        printf("Name: %s  Surname: %s  Absolute: %d  Relative: %.2f\n",
            (list + i)->name,
            (list + i)->surname,
            (list + i)->points,
            relPoints);
    }

    return 0;
}

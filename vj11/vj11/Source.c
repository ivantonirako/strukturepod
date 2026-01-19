#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TABLE_SIZE 11


typedef struct CityTree {
    char name[50];
    int pop;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

typedef struct Country {
    char name[50];
    CityTree* cities;
    struct Country* next;
} Country;



int hash(char* name);

int insertCityTree(CityTree** root, char* name, int pop);
int readCities(CityTree** root, char* file);

int insertCountryHash(Country* table[], char* name, char* file);
Country* findCountry(Country* table[], char* name);

int printCityTree(CityTree* root, int minPop);


int main()
{
    FILE* f = fopen("drzave.txt", "r");
    char country[50], file[50];
    int minPop;

    Country* hashTable[TABLE_SIZE] = { NULL };

    if (!f) {
        printf("greska pri otvaranju drzave.txt\n");
        return 1;
    }

    /* ucitavanje drzava u hash tablicu */
    while (fscanf(f, "%s %s", country, file) == 2)
        insertCountryHash(hashTable, country, file);

    fclose(f);

    /* unos podataka za pretragu */
    printf("unesi drzavu: ");
    scanf("%s", country);

    printf("unesi minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    /* trazenje drzave pomocu hash tablice */
    Country* c = findCountry(hashTable, country);
    if (c)
        printCityTree(c->cities, minPop);
    else
        printf("drzava ne postoji\n");

    return 0;
}


/*
    hash funkcija
    racuna indeks na temelju prvih nekoliko znakova imena drzave
*/
int hash(char* name)
{
    int sum = 0;

    for (int i = 0; i < 5 && name[i] != '\0'; i++)
        sum += name[i];

    return sum % TABLE_SIZE;
}


/*
    dodavanje grada u binarno stablo
    stablo je sortirano po populaciji
*/
int insertCityTree(CityTree** root, char* name, int pop)
{
    if (!(*root)) {
        *root = malloc(sizeof(CityTree));
        if (!(*root)) return -1;

        strcpy((*root)->name, name);
        (*root)->pop = pop;
        (*root)->left = (*root)->right = NULL;
        return 0;
    }

    if (pop > (*root)->pop ||
        (pop == (*root)->pop && strcmp(name, (*root)->name) < 0))
        return insertCityTree(&(*root)->left, name, pop);
    else
        return insertCityTree(&(*root)->right, name, pop);
}

/* ucitavanje gradova iz datoteke u stablo */
int readCities(CityTree** root, char* file)
{
    FILE* f = fopen(file, "r");
    char name[50];
    int pop;

    if (!f) return -1;

    while (fscanf(f, " %[^,],%d", name, &pop) == 2)
        insertCityTree(root, name, pop);

    fclose(f);
    return 0;
}


/*
    dodavanje drzave u hash tablicu
    kolizije se rjesavaju ulanacanjem
*/
int insertCountryHash(Country* table[], char* name, char* file)
{
    int key = hash(name);

    Country* new = malloc(sizeof(Country));
    if (!new) return -1;

    strcpy(new->name, name);
    new->cities = NULL;
    new->next = NULL;

    /* ucitavanje gradova za drzavu */
    readCities(&new->cities, file);

    Country* curr = table[key];
    Country* prev = NULL;

    /* umetanje u sortiranu listu po nazivu drzave */
    while (curr && strcmp(curr->name, name) < 0) {
        prev = curr;
        curr = curr->next;
    }

    if (!prev) {
        new->next = table[key];
        table[key] = new;
    }
    else {
        new->next = curr;
        prev->next = new;
    }

    return 0;
}

/* trazenje drzave u hash tablici */
Country* findCountry(Country* table[], char* name)
{
    int key = hash(name);
    Country* curr = table[key];

    while (curr) {
        if (strcmp(curr->name, name) == 0)
            return curr;
        curr = curr->next;
    }
    return NULL;
}


/* inorder ispis gradova s populacijom vecom od zadanog minimuma */
int printCityTree(CityTree* root, int minPop)
{
    if (!root) return 0;

    printCityTree(root->left, minPop);

    if (root->pop > minPop)
        printf("%s (%d)\n", root->name, root->pop);

    printCityTree(root->right, minPop);

    return 0;
}

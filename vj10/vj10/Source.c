#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct CityTree {
    char name[50];
    int pop;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

typedef struct CityList {
    char name[50];
    int pop;
    struct CityList* next;
} CityList;

typedef struct CountryList {
    char name[50];
    CityTree* cities;
    struct CountryList* next;
} CountryList;

typedef struct CountryTree {
    char name[50];
    CityList* cities;
    struct CountryTree* left;
    struct CountryTree* right;
} CountryTree;


/* funkcije za rad s gradovima */
int insertCityTree(CityTree** root, char* name, int pop);
int insertCityList(CityList** head, char* name, int pop);

/* funkcije za ucitavanje podataka iz datoteka */
int readCitiesTree(CityTree** root, char* file);
int readCitiesList(CityList** head, char* file);

/* funkcije za rad s drzavama */
int insertCountryList(CountryList** head, char* name, char* file);
int insertCountryTree(CountryTree** root, char* name, char* file);

/* ispis i pretrazivanje */
int printCityTree(CityTree* root, int minPop);
int printCityList(CityList* head, int minPop);
CountryList* findCountryList(CountryList* head, char* name);


int main() {

    FILE* f = fopen("drzave.txt", "r");
    char country[50], file[50];
    int minPop;

    CountryList* list = NULL;
    CountryTree* tree = NULL;

    if (!f) {
        printf("greska pri otvaranju drzave.txt\n");
        return 1;
    }

    /* ucitavanje drzava i pripadajucih gradova */
    while (fscanf(f, "%s %s", country, file) == 2) {
        insertCountryList(&list, country, file);
        insertCountryTree(&tree, country, file);
    }
    fclose(f);

    /* unos drzave i minimalnog broja stanovnika */
    printf("unesi drzavu: ");
    scanf("%s", country);

    printf("unesi minimalan broj stanovnika: ");
    scanf("%d", &minPop);

    /* trazenje drzave u listi */
    CountryList* c = findCountryList(list, country);
    if (c)
        printCityTree(c->cities, minPop);
    else
        printf("drzava ne postoji\n");

    return 0;
}


/*
    dodavanje grada u binarno stablo
    stablo je sortirano po broju stanovnika
*/
int insertCityTree(CityTree** root, char* name, int pop) {

    if (!(*root)) {
        *root = malloc(sizeof(CityTree));
        if (!(*root)) return -1;

        strcpy((*root)->name, name);
        (*root)->pop = pop;
        (*root)->left = (*root)->right = NULL;
        return 0;
    }

    if (pop > (*root)->pop)
        return insertCityTree(&(*root)->left, name, pop);
    else
        return insertCityTree(&(*root)->right, name, pop);
}

/* dodavanje grada na pocetak liste */
int insertCityList(CityList** head, char* name, int pop) {

    CityList* n = malloc(sizeof(CityList));
    if (!n) return -1;

    strcpy(n->name, name);
    n->pop = pop;
    n->next = *head;
    *head = n;

    return 0;
}


/* ucitavanje gradova u stablo iz datoteke */
int readCitiesTree(CityTree** root, char* file) {

    FILE* f = fopen(file, "r");
    char name[50];
    int pop;

    if (!f) return -1;

    while (fscanf(f, " %[^,],%d", name, &pop) == 2)
        insertCityTree(root, name, pop);

    fclose(f);
    return 0;
}

/* ucitavanje gradova u listu iz datoteke */
int readCitiesList(CityList** head, char* file) {

    FILE* f = fopen(file, "r");
    char name[50];
    int pop;

    if (!f) return -1;

    while (fscanf(f, " %[^,],%d", name, &pop) == 2)
        insertCityList(head, name, pop);

    fclose(f);
    return 0;
}


/*
    dodavanje drzave u listu
    uz drzavu se ucitava i stablo gradova
*/
int insertCountryList(CountryList** head, char* name, char* file) {

    CountryList* n = malloc(sizeof(CountryList));
    if (!n) return -1;

    strcpy(n->name, name);
    n->cities = NULL;
    n->next = *head;
    *head = n;

    return readCitiesTree(&n->cities, file);
}

/*
    dodavanje drzave u binarno stablo
    drzave su sortirane po nazivu
*/
int insertCountryTree(CountryTree** root, char* name, char* file) {

    if (!(*root)) {
        *root = malloc(sizeof(CountryTree));
        if (!(*root)) return -1;

        strcpy((*root)->name, name);
        (*root)->cities = NULL;
        (*root)->left = (*root)->right = NULL;
        return readCitiesList(&(*root)->cities, file);
    }

    if (strcmp(name, (*root)->name) < 0)
        return insertCountryTree(&(*root)->left, name, file);
    else
        return insertCountryTree(&(*root)->right, name, file);
}


/* inorder ispis gradova iz stabla uz provjeru populacije */
int printCityTree(CityTree* root, int minPop) {

    if (!root) return 0;

    printCityTree(root->left, minPop);

    if (root->pop > minPop)
        printf("%s (%d)\n", root->name, root->pop);

    printCityTree(root->right, minPop);
    return 0;
}

/* ispis gradova iz liste */
int printCityList(CityList* head, int minPop) {

    while (head) {
        if (head->pop > minPop)
            printf("%s (%d)\n", head->name, head->pop);
        head = head->next;
    }
    return 0;
}


/* linearna pretraga drzave u listi */
CountryList* findCountryList(CountryList* head, char* name) {

    while (head) {
        if (strcmp(head->name, name) == 0)
            return head;
        head = head->next;
    }
    return NULL;
}

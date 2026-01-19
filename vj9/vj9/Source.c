#include <stdio.h>
#include <stdlib.h>
#include <time.h>


typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;


Node* insert(Node* root, int x);
int replace(Node* root);
int inorder(Node* root, FILE* f);

int main() {

    Node* root = NULL;
    int i;
    int n = 10;
    int number;

    /* otvaranje datoteke za upis rezultata */
    FILE* f = fopen("stablo.txt", "w");
    if (f == NULL) {
        printf("greska pri otvaranju datoteke\n");
        return 1;
    }

    /* inicijalizacija generatora slucajnih brojeva */
    srand((unsigned)time(NULL));

    /* generiranje slucajnih brojeva i izgradnja bst */
    for (i = 0; i < n; i++) {
        number = rand() % 81 + 10;
        root = insert(root, number);
    }

    /* ispis stabla prije zamjene vrijednosti */
    fprintf(f, "inorder nakon insert funkcije:\n");
    inorder(root, f);

    /* zamjena vrijednosti cvorova zbrojem potomaka */
    replace(root);

    /* ispis stabla nakon replace funkcije */
    fprintf(f, "\ninorder nakon replace funkcije:\n");
    inorder(root, f);

    fclose(f);
    return 0;
}

/*
    funkcija za dodavanje elementa u binarno stablo pretrazivanja
    ako je stablo prazno stvara se novi cvor
    manji elementi idu lijevo, veci desno
*/
Node* insert(Node* root, int x) {

    if (root == NULL) {
        Node* newNode = (Node*)malloc(sizeof(Node));
        newNode->value = x;
        newNode->left = NULL;
        newNode->right = NULL;
        return newNode;
    }

    if (x < root->value)
        root->left = insert(root->left, x);
    else
        root->right = insert(root->right, x);

    return root;
}

/*
    funkcija rekurzivno prolazi stablo
    svaki cvor dobiva vrijednost koja je zbroj svih njegovih potomaka
    povratna vrijednost funkcije je ukupna suma podstabla
*/
int replace(Node* root) {

    if (root == NULL)
        return 0;

    int leftSum = replace(root->left);
    int rightSum = replace(root->right);

    int oldValue = root->value;
    root->value = leftSum + rightSum;

    return oldValue + leftSum + rightSum;
}

/*
    inorder obilazak stabla
    lijevo dijete -> cvor -> desno dijete
*/
int inorder(Node* root, FILE* f) {

    if (root == NULL)
        return 0;

    inorder(root->left, f);
    fprintf(f, "%d ", root->value);
    inorder(root->right, f);

    return 0;
}

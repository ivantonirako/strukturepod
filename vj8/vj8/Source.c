#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int value;
    struct Node* left;
    struct Node* right;
} Node;

int insert(Node** root, int value);
int find(Node* root, int value);
int inorder(Node* root);
int preorder(Node* root);
int postorder(Node* root);
int levelOrder(Node* root);
int deleteNode(Node** root, int value);
int freeTree(Node* root);

int main() {
    Node* root = NULL;
    int choice = -1;
    int value;

    do {
        printf("\n===== izbornik =====\n");
        printf("1 - dodaj element\n");
        printf("2 - ispis inorder\n");
        printf("3 - ispis preorder\n");
        printf("4 - ispis postorder\n");
        printf("5 - ispis level order\n");
        printf("6 - pretrazi element\n");
        printf("7 - obrisi element\n");
        printf("0 - izlaz iz programa\n");
        printf("odabir: ");
        scanf("%d", &choice);

        switch (choice) {

        case 1:
            printf("unesi vrijednost: ");
            scanf("%d", &value);
            insert(&root, value);
            break;

        case 2:
            printf("inorder: ");
            inorder(root);
            printf("\n");
            break;

        case 3:
            printf("preorder: ");
            preorder(root);
            printf("\n");
            break;

        case 4:
            printf("postorder: ");
            postorder(root);
            printf("\n");
            break;

        case 5:
            printf("level order: ");
            levelOrder(root);
            printf("\n");
            break;

        case 6:
            printf("unesi vrijednost za pretragu: ");
            scanf("%d", &value);
            if (find(root, value))
                printf("element je pronaden u stablu\n");
            else
                printf("element ne postoji u stablu\n");
            break;

        case 7:
            printf("unesi vrijednost za brisanje: ");
            scanf("%d", &value);
            deleteNode(&root, value);
            break;

        case 0:
            printf("zatvaranje programa...\n");
            break;

        default:
            printf("neispravan unos\n");
        }

    } while (choice != 0);

    freeTree(root);
    return 0;
}

/*
   funkcija za dodavanje novog elementa u bst
   ako je stablo prazno, stvara se novi korijen
   manji elementi idu lijevo, veci desno
*/
int insert(Node** root, int value) {
    if (*root == NULL) {
        *root = (Node*)malloc(sizeof(Node));
        (*root)->value = value;
        (*root)->left = NULL;
        (*root)->right = NULL;
        return 1;
    }

    if (value < (*root)->value)
        return insert(&(*root)->left, value);
    else if (value > (*root)->value)
        return insert(&(*root)->right, value);

    /* duplikati se ne dodaju */
    return 0;
}

/* rekurzivna pretraga elementa u stablu */
int find(Node* root, int value) {
    if (root == NULL)
        return 0;

    if (value == root->value)
        return 1;

    if (value < root->value)
        return find(root->left, value);

    return find(root->right, value);
}

/* inorder obilazak: lijevo - korijen - desno */
int inorder(Node* root) {
    if (root == NULL)
        return 0;

    inorder(root->left);
    printf("%d ", root->value);
    inorder(root->right);

    return 1;
}

/* preorder obilazak: korijen - lijevo - desno */
int preorder(Node* root) {
    if (root == NULL)
        return 0;

    printf("%d ", root->value);
    preorder(root->left);
    preorder(root->right);

    return 1;
}

/* postorder obilazak: lijevo - desno - korijen */
int postorder(Node* root) {
    if (root == NULL)
        return 0;

    postorder(root->left);
    postorder(root->right);
    printf("%d ", root->value);

    return 1;
}

/* level order obilazak pomocu reda*/
int levelOrder(Node* root) {
    if (root == NULL)
        return 0;

    Node* queue[100];
    int front = 0, rear = 0;

    queue[rear++] = root;

    while (front < rear) {
        Node* current = queue[front++];
        printf("%d ", current->value);

        if (current->left)
            queue[rear++] = current->left;

        if (current->right)
            queue[rear++] = current->right;
    }
    return 1;
}

/* brisanje elementa iz bst */
int deleteNode(Node** root, int value) {
    if (*root == NULL)
        return 0;

    if (value < (*root)->value)
        return deleteNode(&(*root)->left, value);

    if (value > (*root)->value)
        return deleteNode(&(*root)->right, value);

    /* pronaden cvor za brisanje */
    Node* temp;

    /* slucaj bez lijevog djeteta */
    if ((*root)->left == NULL) {
        temp = (*root)->right;
        free(*root);
        *root = temp;
    }
    /* slucaj bez desnog djeteta */
    else if ((*root)->right == NULL) {
        temp = (*root)->left;
        free(*root);
        *root = temp;
    }
    /* slucaj s dva djeteta */
    else {
        /* trazenje najmanjeg u desnom podstablu */
        temp = (*root)->right;
        while (temp->left)
            temp = temp->left;

        (*root)->value = temp->value;
        deleteNode(&(*root)->right, temp->value);
    }
    return 1;
}

/* rekurzivno oslobadanje cijelog stabla */
int freeTree(Node* root) {
    if (root == NULL)
        return 0;

    freeTree(root->left);
    freeTree(root->right);
    free(root);

    return 1;
}

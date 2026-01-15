#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 50 

// struktura za item
typedef struct Item {
    char name[MAX];
    int quantity;
    float price;
    struct Item* next;
} Item;

// struktura za datum
typedef struct Date {
    int y, m, d;
} Date;

// struktura za racun
typedef struct Bill {
    Date date;
    Item* items;
    struct Bill* next;
} Bill;

/*provjera datuma */
int dateBetween(Date d, Date from, Date to) {
    // provjeri je li datum izmedu from i to
    if (d.y < from.y || d.y > to.y) return 0;
    if (d.y == from.y && d.m < from.m) return 0;
    if (d.y == to.y && d.m > to.m) return 0;
    if (d.y == from.y && d.m == from.m && d.d < from.d) return 0;
    if (d.y == to.y && d.m == to.m && d.d > to.d) return 0;
    return 1;
}

/*umetni racun sortirano po datumu*/
void insertBillSorted(Bill** head, Bill* b) {
    // provjeri pocetak liste ili manji datum
    if (*head == NULL ||
        b->date.y < (*head)->date.y ||
        (b->date.y == (*head)->date.y && b->date.m < (*head)->date.m) ||
        (b->date.y == (*head)->date.y && b->date.m == (*head)->date.m && b->date.d < (*head)->date.d)) {
        b->next = *head;
        *head = b;
        return;
    }
    // pronadi mjesto za umetanje
    Bill* p = *head;
    while (p->next &&
        (p->next->date.y < b->date.y ||
            (p->next->date.y == b->date.y && p->next->date.m < b->date.m) ||
            (p->next->date.y == b->date.y && p->next->date.m == b->date.m && p->next->date.d < b->date.d))) {
        p = p->next;
    }
    b->next = p->next;
    p->next = b;
}

/*ucitaj jedan racun (jedna linija stavki) */
Bill* loadBill(char* filename) {
    printf("opening file: %s\n", filename);

    FILE* f = fopen(filename, "r");
    if (!f) { // provjeri je li file otvoren
        printf("FAILED to open file: %s\n", filename);
        return NULL;
    }

    Bill* b = malloc(sizeof(Bill));
    b->items = NULL;
    b->next = NULL;

    char line[500];
    if (!fgets(line, sizeof(line), f)) { // ucitaj liniju
        fclose(f);
        free(b);
        return NULL;
    }

    // ukloni \r\n
    line[strcspn(line, "\r\n")] = 0;

    int offset = 0;
    // citanje datuma
    if (sscanf(line, "%d-%d-%d%n", &b->date.y, &b->date.m, &b->date.d, &offset) != 3) {
        fclose(f);
        free(b);
        printf("INVALID DATE in file: %s\n", filename);
        return NULL;
    }

    char* ptr = line + offset;
    char name[MAX];
    int qty;
    float price;

    // citaj sve stavke u liniji
    while (sscanf(ptr, "%s %d %f%n", name, &qty, &price, &offset) == 3) {
        // ukloni moguci \r
        name[strcspn(name, "\r\n")] = 0;

        Item* it = malloc(sizeof(Item));
        strcpy(it->name, name);
        it->quantity = qty;
        it->price = price;
        it->next = NULL;

        // umetni stavku sortirano po imenu
        if (b->items == NULL || strcmp(it->name, b->items->name) < 0) {
            it->next = b->items;
            b->items = it;
        }
        else {
            Item* p = b->items;
            while (p->next && strcmp(it->name, p->next->name) > 0)
                p = p->next;
            it->next = p->next;
            p->next = it;
        }

        ptr += offset;
    }

    fclose(f);
    return b;
}

/*upit za stavku*/
void query(Bill* head) {
    char item[MAX];
    Date from, to;
    int totalQty = 0;
    float totalMoney = 0;

    printf("Item name: ");
    scanf("%s", item);
    item[strcspn(item, "\r\n")] = 0; // ukloni moguce \r

    printf("From date (YYYY MM DD): ");
    scanf("%d %d %d", &from.y, &from.m, &from.d);

    printf("To date (YYYY MM DD): ");
    scanf("%d %d %d", &to.y, &to.m, &to.d);

    // prolaz kroz sve racune
    while (head) {
        if (dateBetween(head->date, from, to)) {
            Item* it = head->items;
            while (it) {
                if (strcmp(it->name, item) == 0) {
                    totalQty += it->quantity; // ukupna kolicina
                    totalMoney += it->quantity * it->price; // ukupna cijena
                }
                it = it->next;
            }
        }
        head = head->next;
    }

    printf("\nTotal quantity: %d\n", totalQty);
    printf("Total spent: %.2f\n", totalMoney);
}

int main() {
    FILE* f = fopen("racuni.txt", "r");
    if (!f) { // provjera filea sa listom racuna
        printf("cannot open racuni.txt\n");
        return 1;
    }

    Bill* bills = NULL;
    char filename[300];

    // citaj sve fajlove racuna iz liste
    while (fgets(filename, sizeof(filename), f)) {
        filename[strcspn(filename, "\r\n")] = 0; // ukloni \r\n
        if (strlen(filename) == 0) continue;

        Bill* b = loadBill(filename);
        if (b) insertBillSorted(&bills, b); // umetni racun
    }

    fclose(f);

    query(bills); // pokreni upit

    return 0;
}

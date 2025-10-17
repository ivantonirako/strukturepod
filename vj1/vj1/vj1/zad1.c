#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE 1024

typedef struct {
    char ime[50];
    char prezime[50];
    int bodovi;
} Student;

// Funkcija za brojanje redaka u datoteci
int brojRedakaUDatoteci(const char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r");
    if (fp == NULL) {
        printf("Greska: ne mogu otvoriti datoteku %s.\n", imeDatoteke);
        return -1;
    }

    int brojac = 0;
    char buffer[MAX_LINE];
    while (fgets(buffer, MAX_LINE, fp) != NULL)
        brojac++;

    fclose(fp);
    return brojac;
}

int main() {
    const char* imeDatoteke = "studenti.txt";
    int brojStudenata = brojRedakaUDatoteci(imeDatoteke);

    if (brojStudenata <= 0) {
        printf("Datoteka je prazna ili ne postoji.\n");
        return 1;
    }

    // Dinamièka alokacija memorije
    Student* studenti = (Student*)malloc(brojStudenata * sizeof(Student));
    if (studenti == NULL) {
        printf("Greska pri alokaciji memorije.\n");
        return 1;
    }

    FILE* fp = fopen(imeDatoteke, "r");
    if (fp == NULL) {
        printf("Greska: ne mogu otvoriti datoteku %s.\n", imeDatoteke);
        free(studenti);
        return 1;
    }

    // Uèitavanje podataka i pronalazak maksimuma
    int maxBodovi = 0;
    for (int i = 0; i < brojStudenata; i++) {
        fscanf(fp, "%s %s %d", studenti[i].ime, studenti[i].prezime, &studenti[i].bodovi);
        if (studenti[i].bodovi > maxBodovi)
            maxBodovi = studenti[i].bodovi;
    }

    fclose(fp);

    // Ispis podataka
    printf("\n%-10s %-10s %-15s %-20s\n", "IME", "PREZIME", "APS. BODOVI", "REL. BODOVI (%)");
    printf("------------------------------------------------------------\n");

    for (int i = 0; i < brojStudenata; i++) {
        double relativni = ((double)studenti[i].bodovi / maxBodovi) * 100.0;
        printf("%-10s %-10s %-15d %-20.2f\n",
            studenti[i].ime,
            studenti[i].prezime,
            studenti[i].bodovi,
            relativni);
    }

    // Oslobaðanje memorije
    free(studenti);

    printf("\nMaksimalan broj bodova: %d\n", maxBodovi);

    return 0;
}

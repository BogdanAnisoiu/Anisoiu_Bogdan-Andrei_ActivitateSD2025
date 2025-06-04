#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<malloc.h>
#include<string.h>

typedef struct Doctor Doctor;
typedef struct Nod Nod;

struct Doctor {
    char* nume;
    int varsta;
    float salariu;
    char* specializare;
    float aniVechime;
    char* numeSpital;
    int nrPacienti;
};

struct Nod {
    Doctor info;
    Nod* next;
    Nod* prev;
};

Doctor citireDoctorFisier(FILE* file) {
    char buffer[100];
    char sep[3] = ",\n";
    char* aux;
    fgets(buffer, 100, file);
    Doctor d1;
    aux = strtok(buffer, sep);
    d1.nume = malloc(strlen(aux) + 1);
    strcpy(d1.nume, aux);
    aux = strtok(NULL, sep);
    d1.varsta = atoi(aux);
    aux = strtok(NULL, sep);
    d1.salariu = atof(aux);
    aux = strtok(NULL, sep);
    d1.specializare = malloc(strlen(aux) + 1);
    strcpy(d1.specializare, aux);
    aux = strtok(NULL, sep);
    d1.aniVechime = atof(aux);
    aux = strtok(NULL, sep);
    d1.numeSpital = malloc(strlen(aux) + 1);
    strcpy(d1.numeSpital, aux);
    aux = strtok(NULL, sep);
    d1.nrPacienti = atoi(aux);

    return d1;
}


void citireVectorDoctori(FILE* file, Doctor** vectorDoctori, int* nrDoctori) {
    if (file == NULL) {
        return;
    }
    while (!feof(file)) {
        vectorDoctori = (Doctor)realloc(*vectorDoctori, ((*nrDoctori) + 1) * sizeof(Doctor));
        (*vectorDoctori)[(*nrDoctori)] = citireDoctorFisier(file);
        (*nrDoctori)++;
    }
}

void inserareLaInceput(Nod** cap, Nod** coada, Doctor doctor) {
    Nod* nou = (Nod*)malloc(sizeof(Nod));
    nou->info = doctor;
    nou->next = *cap;
    nou->prev = NULL;

    if ((*cap) != NULL) {
        (*cap)->prev = nou;
        *cap = nou;
    }
    else {
        *cap = *coada = nou;
    }
}

void traversareLista(Nod* cap) {
    while (cap != NULL) {
        printf("Doctor: %s, Specializare: %s\n", cap->info.nume, cap->info.specializare);
        cap = cap->next;
    }
}

void traversareListaCirculara(Nod* cap) {
    if (cap == NULL) {
        printf("Lista este goala!\n");
        return;
    }

    Nod* curent = cap;
    Nod* copie = cap;

    do {
        printf("Doctor: %s, Specializare: %s\n", curent->info.nume, curent->info.specializare);
        curent = curent->next;
    } while (curent != copie);
}


void transformaListaCirculara(Nod* cap) {
    if (cap == NULL) return;
    Nod* coada = cap;
    while (coada->next != NULL) {
        coada = coada->next;
    }
    coada->next = cap;
    cap->prev = coada;
}


void traversareListaSimpluInlantuita(Nod* cap) {
    while (cap != NULL) {
        printf("Doctor: %s, Specializare: %s\n", cap->info.nume, cap->info.specializare);
        cap = cap->next;
    }
}


void convertireListaDubluInlantuitaInListaSimpluInlantuita(Nod** cap, int nrDoctori) {
    if (*cap == NULL) return;
    Nod* curent = *cap;
    while (curent != NULL && nrDoctori - 1 != 0) {
        curent->prev = NULL;
        curent = curent->next;
        nrDoctori--;
    }
    curent->next = NULL;
}

int main() {
    FILE* file = fopen("doctori.txt", "r");
    Doctor* vectorDoctori = NULL;
    int nrDoctori = 0;


    citireVectorDoctori(file, &vectorDoctori, &nrDoctori);
    fclose(file);


    Nod* cap = NULL;
    Nod* coada = NULL;


    for (int i = 0; i < nrDoctori; i++) {
        inserareLaInceput(&cap, &coada, vectorDoctori[i]);
    }

    printf("Lista dublu inlantuita:\n");
    traversareLista(cap);


    transformaListaCirculara(cap);
    printf("\nLista dublu inlantuita cireculara:\n");
    traversareListaCirculara(cap);

    convertireListaDubluInlantuitaInListaSimpluInlantuita(&cap, nrDoctori);
    printf("\nLista simplu inlantuita:\n");
    traversareListaSimpluInlantuita(cap);


    Nod* temp;
    while (cap != NULL) {
        temp = cap;
        cap = cap->next;
        free(temp->info.nume);
        free(temp->info.specializare);
        free(temp->info.numeSpital);
        free(temp);
    }

    free(vectorDoctori);

    return 0;
}
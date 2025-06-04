#define _CRT_SECURE_NO_WARNINGS
#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct Telefon telefon;
typedef struct nod nod;
typedef struct HashTable HashTable;

struct Telefon
{
    char* marca;
    char* model;
    char* sistemOperare;
    int anLansare;
    float pret;
    int memorieRAM;
    int stoc;

};

struct nod
{
    telefon info;
    nod* next;


};
struct HashTable
{
    int dimensiune;
    nod** vector;
};

telefon citireTelefonFisier(FILE* file) {
    char buffer[200];
    char sep[3] = ",\n";
    char* aux;
    fgets(buffer, 200, file);
    telefon t;
    aux = strtok(buffer, sep);
    t.marca = malloc(strlen(aux) + 1);
    strcpy(t.marca, aux);
    aux = strtok(NULL, sep);
    t.model = malloc(strlen(aux) + 1);
    strcpy(t.model, aux);
    aux = strtok(NULL, sep);
    t.sistemOperare = malloc(strlen(aux) + 1);
    strcpy(t.sistemOperare, aux);
    aux = strtok(NULL, sep);
    t.anLansare = atoi(aux);
    aux = strtok(NULL, sep);
    t.pret = atof(aux);
    aux = strtok(NULL, sep);
    t.memorieRAM = atoi(aux);
    aux = strtok(NULL, sep);
    t.stoc = atoi(aux);
    return t;
}
void put(nod** coada, telefon t) {
    nod* nou = (nod*)malloc(sizeof(nod));
    nou->info = t;
    nou->next = NULL;
    if (*coada == NULL) {
        *coada = nou;
    }
    else
    {
        nod* temp = *coada;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = nou;
    }
}



nod* citireCoada(const char* fisier)
{
    FILE* file = fopen(fisier, "r");
    nod* coada = NULL;
    while (!feof(file))
        put(&coada, citireTelefonFisier(file));
    return coada;
}

void afisareTelefon(telefon t) {
    printf("Marca: %s\n", t.marca);
    printf("Model: %s\n", t.model);
    printf("Sistem de operare: %s\n", t.sistemOperare);
    printf("An lansare: %d\n", t.anLansare);
    printf("Pret: %.2f RON\n", t.pret);
    printf("Memorie RAM: %d GB\n", t.memorieRAM);
    printf("Stoc disponibil: %d bucati\n", t.stoc);
    printf("-------------------------\n");
}

void afisareCoada(nod* coada) {
    if (coada == NULL) {
        printf("Coada este goala.\n");
        return;
    }

    printf("Continutul cozii de telefoane:\n");
    printf("===============================\n");

    nod* temp = coada;
    while (temp != NULL) {
        afisareTelefon(temp->info);
        temp = temp->next;
    }
}
telefon TelefonPretMic(nod* coada, float pret)
{
    nod* temp = coada;
    telefon t1;
    int gasit = 0;
    while (temp != NULL) {
        if (temp->info.pret < pret) {
            t1 = temp->info;
            gasit = 1;
        }
        temp = temp->next;
    }
    if (!gasit) {
        printf("Niciun telefon gasit\n");
        return t1;
    }
    return t1;
}

HashTable initHashTable(int dimensiune) {
    HashTable ht;
    ht.dimensiune = dimensiune;
    ht.vector = (nod*)malloc(sizeof(nod) * dimensiune);
    for (int i = 0; i < dimensiune; i++) {
        ht.vector[i] = NULL;
    }
    return ht;
}
int hash(telefon t1, int dimensiune) {
    int suma = 0;
    for (int i = 0; i < strlen(t1.marca) + 1; i++)
        suma += t1.marca[i];
    return suma % dimensiune;

}
void inserareInHashTable(HashTable* ht, telefon t) {
    int cheie = hash(t, ht->dimensiune);
    nod* nou = (nod*)malloc(sizeof(nod));
    nou->info = t;
    nou->next = ht->vector[cheie];
    ht->vector[cheie] = nou;
}

void inserareCoadaInHashTable(nod* coada, HashTable* ht) {
    nod* temp = coada;
    while (temp != NULL) {
        inserareInHashTable(ht, temp->info);
        temp = temp->next;
    }
}
void afisareHashTable(HashTable ht) {
    for (int i = 0; i < ht.dimensiune; i++) {
        if (ht.vector[i] != NULL) {
            printf("--- Cheia %d ---\n", i);
            afisareCoada(ht.vector[i]);
        }
    }
}
telefon** ElemDupaCheie(HashTable ht, int cheie, int* nrElemente) {
    *nrElemente = 0;
    if (cheie < 0 || cheie >= ht.dimensiune || ht.vector[cheie] == NULL) {
        return NULL;
    }

    nod* temp = ht.vector[cheie];
    telefon** vector = NULL;
    while (temp != NULL) {
        vector = (telefon*)realloc(vector, (*nrElemente + 1) * sizeof(telefon));
        vector[*nrElemente] = &temp->info;
        (*nrElemente)++;
        temp = temp->next;
    }
    return vector;
}

void dezalocareTelefon(telefon t) {
    free(t.marca);
    free(t.model);
    free(t.sistemOperare);
}

void dezalocareCoada(nod** coada) {
    nod* temp;
    while (*coada != NULL) {
        temp = *coada;
        *coada = (*coada)->next;
        dezalocareTelefon(temp->info);
        free(temp);
    }
}

void dezalocareHashTable(HashTable* ht) {

    free(ht->vector);
    ht->vector = NULL;
    ht->dimensiune = 0;
}



int main()
{
    nod* coada = citireCoada("telefoane.txt");
    int nrElemente = 0;
    afisareCoada(coada);
    telefon t3 = TelefonPretMic(coada, 3000);
    printf("\nTelefonul gasit: \n\n\n");
    afisareTelefon(t3);
    HashTable ht = initHashTable(10);
    inserareCoadaInHashTable(coada, &ht);
    afisareHashTable(ht);
    telefon** vector = ElemDupaCheie(ht, 4, &nrElemente);

    printf("\nElementele de la cheia cautata sunt:\n");
    for (int i = 0; i < nrElemente; i++) {
        afisareTelefon(*vector[i]);
    }
    free(*vector);
    dezalocareCoada(&coada);
    dezalocareHashTable(&ht);
}
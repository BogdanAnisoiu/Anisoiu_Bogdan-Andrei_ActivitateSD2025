#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


typedef struct Eveniment {
    int id;
    char* denumire;
    char* locatie;
    char* organizator;
    float durataOre;
    int nrParticipanti;
    int an;
} Eveniment;


typedef struct NodPrincipal NodPrincipal;
typedef struct NodSecundar NodSecundar;

struct NodSecundar {
    NodPrincipal* ref;
    NodSecundar* next;
};

struct NodPrincipal {
    Eveniment info;
    NodPrincipal* next;
    NodSecundar* vecini;
};

typedef struct NodAVL {
    Eveniment info;
    struct NodAVL* st;
    struct NodAVL* dr;
} NodAVL;

Eveniment initEveniment(int id, const char* denumire, const char* locatie, const char* organizator, float durata, int participanti, int an) {
    Eveniment e;
    e.id = id;
    e.denumire = (char*)malloc(strlen(denumire) + 1);
    strcpy(e.denumire, denumire);
    e.locatie = (char*)malloc(strlen(locatie) + 1);
    strcpy(e.locatie, locatie);
    e.organizator = (char*)malloc(strlen(organizator) + 1);
    strcpy(e.organizator, organizator);
    e.durataOre = durata;
    e.nrParticipanti = participanti;
    e.an = an;
    return e;
}

void inserareNodPrincipal(NodPrincipal** graf, Eveniment e) {
    NodPrincipal* nou = (NodPrincipal*)malloc(sizeof(NodPrincipal));
    nou->info = e;
    nou->next = NULL;
    nou->vecini = NULL;
    if (*graf == NULL) {
        *graf = nou;
    }
    else {
        NodPrincipal* temp = *graf;
        while (temp->next) temp = temp->next;
        temp->next = nou;
    }
}

NodPrincipal* cautareNodDupaId(NodPrincipal* graf, int id) {
    while (graf && graf->info.id != id) graf = graf->next;
    return graf;
}

void inserareNodSecundar(NodSecundar** cap, NodPrincipal* ref) {
    NodSecundar* nou = (NodSecundar*)malloc(sizeof(NodSecundar));
    nou->ref = ref;
    nou->next = NULL;
    if (*cap == NULL) *cap = nou;
    else {
        NodSecundar* temp = *cap;
        while (temp->next) temp = temp->next;
        temp->next = nou;
    }
}

void adaugaMuchie(NodPrincipal* graf, int id1, int id2) {
    NodPrincipal* n1 = cautareNodDupaId(graf, id1);
    NodPrincipal* n2 = cautareNodDupaId(graf, id2);
    if (n1 && n2) {
        inserareNodSecundar(&n1->vecini, n2);
        inserareNodSecundar(&n2->vecini, n1);
    }
}

void afisareEveniment(Eveniment e) {
    printf("\n%d. %s (%s, %s), durata %.1f h, %d participanti, an %d", e.id, e.denumire, e.locatie, e.organizator, e.durataOre, e.nrParticipanti, e.an);
}

float medieParticipanti(NodPrincipal* graf) {
    int suma = 0, nr = 0;
    while (graf) {
        suma += graf->info.nrParticipanti;
        nr++;
        graf = graf->next;
    }
    return nr ? (float)suma / nr : 0;
}

void inserareAVL(NodAVL** rad, Eveniment e) {
    if (*rad == NULL) {
        NodAVL* nou = (NodAVL*)malloc(sizeof(NodAVL));
        nou->info = initEveniment(e.id, e.denumire, e.locatie, e.organizator, e.durataOre, e.nrParticipanti, e.an);
        nou->st = nou->dr = NULL;
        *rad = nou;
    }
    else if (e.id < (*rad)->info.id) {
        inserareAVL(&(*rad)->st, e);
    }
    else {
        inserareAVL(&(*rad)->dr, e);
    }
}

void parcurgereInordineAVL(NodAVL* rad) {
    if (rad) {
        parcurgereInordineAVL(rad->st);
        afisareEveniment(rad->info);
        parcurgereInordineAVL(rad->dr);
    }
}

void selectieLocatie(NodAVL* rad, const char* locatie, Eveniment* vec[], int* index) {
    if (rad) {
        selectieLocatie(rad->st, locatie, vec, index);
        if (strcmp(rad->info.locatie, locatie) == 0 && *index < 10) {
            vec[*index] = &rad->info;
            (*index)++;
        }
        selectieLocatie(rad->dr, locatie, vec, index);
    }
}

int sumaParticipanti(NodAVL* rad, float minDurata, int an) {
    if (!rad) return 0;
    int suma = 0;
    if (rad->info.durataOre > minDurata && rad->info.an == an)
        suma += rad->info.nrParticipanti;
    return suma + sumaParticipanti(rad->st, minDurata, an) + sumaParticipanti(rad->dr, minDurata, an);
}

void parcurgereLatime(NodPrincipal* graf) {
    if (!graf) return;
    int maxId = 0;
    NodPrincipal* temp = graf;
    while (temp) {
        if (temp->info.id > maxId) maxId = temp->info.id;
        temp = temp->next;
    }
    int* vizitat = (int*)calloc(maxId + 1, sizeof(int));
    int* coada = (int*)malloc((maxId + 1) * sizeof(int));
    if (!vizitat || !coada) return;
    int start = 0, end = 0;

    if (graf->info.id > maxId) {
        free(vizitat);
        free(coada);
        return;
    }
    coada[end++] = graf->info.id;
    vizitat[graf->info.id] = 1;

    while (start < end) {
        int idCurent = coada[start++];
        NodPrincipal* nod = cautareNodDupaId(graf, idCurent);
        if (nod) {
            afisareEveniment(nod->info);
            NodSecundar* vecin = nod->vecini;
            while (vecin) {
                int idVecin = vecin->ref->info.id;
                if (idVecin <= maxId && !vizitat[idVecin]) {
                    coada[end++] = idVecin;
                    vizitat[idVecin] = 1;
                }
                vecin = vecin->next;
            }
        }
    }
    free(vizitat);
    free(coada);
}

NodAVL* minNode(NodAVL* rad) {
    while (rad && rad->st) rad = rad->st;
    return rad;
}

NodAVL* stergeAVL(NodAVL* rad, int id) {
    if (!rad) return NULL;
    if (id < rad->info.id) rad->st = stergeAVL(rad->st, id);
    else if (id > rad->info.id) rad->dr = stergeAVL(rad->dr, id);
    else {
        if (!rad->st) {
            NodAVL* temp = rad->dr;
            free(rad->info.denumire);
            free(rad->info.locatie);
            free(rad->info.organizator);
            free(rad);
            return temp;
        }
        else if (!rad->dr) {
            NodAVL* temp = rad->st;
            free(rad->info.denumire);
            free(rad->info.locatie);
            free(rad->info.organizator);
            free(rad);
            return temp;
        }
        else {
            NodAVL* temp = minNode(rad->dr);
            free(rad->info.denumire);
            free(rad->info.locatie);
            free(rad->info.organizator);
            rad->info = initEveniment(temp->info.id, temp->info.denumire, temp->info.locatie, temp->info.organizator, temp->info.durataOre, temp->info.nrParticipanti, temp->info.an);
            rad->dr = stergeAVL(rad->dr, temp->info.id);
        }
    }
    return rad;
}

void dezalocareAVL(NodAVL** rad) {
    if (*rad) {
        dezalocareAVL(&(*rad)->st);
        dezalocareAVL(&(*rad)->dr);
        free((*rad)->info.denumire);
        free((*rad)->info.locatie);
        free((*rad)->info.organizator);
        free(*rad);
    }
}

void dezalocareGraf(NodPrincipal** graf) {
    while (*graf) {
        NodPrincipal* temp = *graf;
        *graf = (*graf)->next;
        free(temp->info.denumire);
        free(temp->info.locatie);
        free(temp->info.organizator);
        NodSecundar* v = temp->vecini;
        while (v) {
            NodSecundar* aux = v;
            v = v->next;
            free(aux);
        }
        free(temp);
    }
}

int main() {
    NodPrincipal* graf = NULL;
    NodAVL* arbore = NULL;

    FILE* f = fopen("evenimente.txt", "r");
    if (!f) return -1;

    int n;
    fscanf(f, "%d", &n);
    for (int i = 0; i < n; i++) {
        int id, part, an;
        float durata;
        char den[50], loc[50], org[50];
        fscanf(f, "%d %s %s %s %f %d %d", &id, den, loc, org, &durata, &part, &an);
        Eveniment e = initEveniment(id, den, loc, org, durata, part, an);
        inserareNodPrincipal(&graf, e);
        inserareAVL(&arbore, e);
    }
    int m;
    fscanf(f, "%d", &m);
    for (int i = 0; i < m; i++) {
        int x, y;
        fscanf(f, "%d %d", &x, &y);
        adaugaMuchie(graf, x, y);
    }
    fclose(f);

    printf("\nMedie participanti: %.2f", medieParticipanti(graf));

    Eveniment* vec[10]; int index = 0;
    selectieLocatie(arbore, "Cluj", vec, &index);
    printf("\n\nEvenimente in Cluj:");
    for (int i = 0; i < index; i++) afisareEveniment(*vec[i]);

    printf("\n\nSuma participanti (durata > 5 si an = 2023): %d", sumaParticipanti(arbore, 5.0, 2023));

    printf("\n\nParcurgere graf (latime):\n");
    parcurgereLatime(graf);

    arbore = stergeAVL(arbore, 2);
    printf("\n\nAVL dupa stergere nod cu id 2:\n");
    parcurgereInordineAVL(arbore);

    dezalocareGraf(&graf);
    dezalocareAVL(&arbore);
    return 0;
}
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <malloc.h>
#include <string.h>

typedef struct Film Film;
struct Film {
	int id;
	char* nume;
	char* regizor;
	float durata;
	int an;
	char* gen;
	float* rating;
};

typedef struct Nod Nod;
struct Nod {
	Film info;
	Nod* stanga;
	Nod* dreapta;
};

typedef struct Heap Heap;
struct Heap {
	Film* vector;
	int dim;
};


Film initFilm(int id, char* nume, char* regizor, float durata, int an, char* gen, float rating) {
	Film f;
	f.id = id;
	f.nume = (char*)malloc(strlen(nume) + 1);
	strcpy(f.nume, nume);
	f.regizor = (char*)malloc(strlen(regizor) + 1);
	strcpy(f.regizor, regizor);
	f.durata = durata;
	f.an = an;
	f.gen = (char*)malloc(strlen(gen) + 1);
	strcpy(f.gen, gen);
	f.rating = (float*)malloc(sizeof(float));
	*f.rating = rating;
	return f;
}

void inserareBST(Nod** rad, Film f) {
	if (!(*rad)) {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = f;
		nou->stanga = nou->dreapta = NULL;
		*rad = nou;
	}
	else if (f.id < (*rad)->info.id)
		inserareBST(&(*rad)->stanga, f);
	else
		inserareBST(&(*rad)->dreapta, f);
}


void citireFisier(const char* fisier, Nod** rad) {
	FILE* f = fopen(fisier, "r");
	if (!f) {
		printf("Eroare la deschiderea fisierului %s!\n", fisier);
		return;
	}
	int id, an;
	float durata, rating;
	char nume[50], regizor[50], gen[50];
	while (fscanf(f, "%d %s %s %f %d %s %f", &id, nume, regizor, &durata, &an, gen, &rating) == 7)
		inserareBST(rad, initFilm(id, nume, regizor, durata, an, gen, rating));
	fclose(f);
}


int numarFrunze(Nod* rad) {
	if (!rad) return 0;
	if (!rad->stanga && !rad->dreapta) return 1;
	return numarFrunze(rad->stanga) + numarFrunze(rad->dreapta);
}


void inordineBST(Nod* rad) {
	if (rad) {
		inordineBST(rad->stanga);
		printf("%d %s\n", rad->info.id, rad->info.nume);
		inordineBST(rad->dreapta);
	}
}

void creareHeapDinBST(Nod* rad, Heap* h) {
	if (rad) {
		h->vector[h->dim++] = initFilm(rad->info.id, rad->info.nume, rad->info.regizor, rad->info.durata, rad->info.an, rad->info.gen, *(rad->info.rating));
		creareHeapDinBST(rad->stanga, h);
		creareHeapDinBST(rad->dreapta, h);
	}
}


void filtrareHeap(Heap h, int poz) {
	int min = poz, st = 2 * poz + 1, dr = 2 * poz + 2;
	if (st < h.dim && h.vector[st].durata < h.vector[min].durata)
		min = st;
	if (dr < h.dim && h.vector[dr].durata < h.vector[min].durata)
		min = dr;
	if (min != poz) {
		Film aux = h.vector[poz];
		h.vector[poz] = h.vector[min];
		h.vector[min] = aux;
		filtrareHeap(h, min);
	}
}

void afisareFrunzeHeap(Heap h) {
	for (int i = 0; i < h.dim; i++)
		if (2 * i + 1 >= h.dim)
			printf("%d %s\n", h.vector[i].id, h.vector[i].nume);
}

void stergereDinHeap(Heap* h, float pragDurata) {
	for (int i = 0; i < h->dim; i++)
		if (h->vector[i].durata > pragDurata) {
			free(h->vector[i].nume);
			free(h->vector[i].regizor);
			free(h->vector[i].gen);
			free(h->vector[i].rating);
			h->vector[i] = h->vector[--h->dim];
			filtrareHeap(*h, i);
			i--;
		}
}
void dezalocareBST(Nod** rad) {
	if (*rad) {
		dezalocareBST(&(*rad)->stanga);
		dezalocareBST(&(*rad)->dreapta);
		free((*rad)->info.nume);
		free((*rad)->info.regizor);
		free((*rad)->info.gen);
		free((*rad)->info.rating);
		free(*rad);
	}
}


void main() {
	Nod* radacina = NULL;
	citireFisier("filme.txt", &radacina);
	printf("Inordine BST:\n");
	inordineBST(radacina);
	printf("Frunze arbore: %d\n", numarFrunze(radacina));
	printf("Frunze subarbore drept: %d\n", numarFrunze(radacina->dreapta));
	Heap h;
	h.dim = 0;
	h.vector = (Film*)malloc(100 * sizeof(Film));
	creareHeapDinBST(radacina, &h);
	for (int i = (h.dim - 1) / 2; i >= 0; i--)
		filtrareHeap(h, i);
	printf("Frunze Heap:\n");
	afisareFrunzeHeap(h);
	stergereDinHeap(&h, 120.0);
	printf("Heap dupa stergere:\n");
	for (int i = 0; i < h.dim; i++)
		printf("%d %s\n", h.vector[i].id, h.vector[i].nume);
	for (int i = 0; i < h.dim; i++) {
		free(h.vector[i].nume);
		free(h.vector[i].regizor);
		free(h.vector[i].gen);
		free(h.vector[i].rating);
	}
	free(h.vector);
	dezalocareBST(&radacina);
}
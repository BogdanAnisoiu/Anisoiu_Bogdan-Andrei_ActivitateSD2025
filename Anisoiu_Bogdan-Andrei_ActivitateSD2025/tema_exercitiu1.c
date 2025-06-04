#define _CRT_SECURE_NO_WARNINGS

#include<stdio.h>
#include<malloc.h>
#include<string.h>
#include<stdlib.h>

typedef struct AnimalZoo AnimalZoo;
typedef struct nod nod;




struct AnimalZoo {
	char* numeAnimal;
	int varsta;
	float inaltime;
	char* categorie;
	float greutate;
	char* numeZoo;
	int nrHabitat;
};

struct nod {
	AnimalZoo info;
	nod* urmator;
};

AnimalZoo citireAnimalFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	char* aux;
	fgets(buffer, 100, file);
	AnimalZoo a1;
	aux = strtok(buffer, sep);
	a1.numeAnimal = malloc(strlen(aux) + 1);
	strcpy(a1.numeAnimal, aux);
	aux = strtok(NULL, sep);
	a1.varsta = atoi(aux);
	aux = strtok(NULL, sep);
	a1.inaltime = atof(aux);
	aux = strtok(NULL, sep);
	a1.categorie = malloc(strlen(aux) + 1);
	strcpy(a1.categorie, aux);
	aux = strtok(NULL, sep);
	a1.greutate = atof(aux);
	aux = strtok(NULL, sep);
	a1.numeZoo = malloc(strlen(aux) + 1);
	strcpy(a1.numeZoo, aux);
	aux = strtok(NULL, sep);
	a1.nrHabitat = atoi(aux);

	return a1;



}


void citireVectorAnimale(FILE* file, AnimalZoo* vectorAnimale, int nrAnimale) {
	if (file == NULL) {
		return;
	}
	while (!feof(file)) {
		vectorAnimale = (AnimalZoo)realloc(*vectorAnimale, ((*nrAnimale) + 1) * sizeof(AnimalZoo));
		(*vectorAnimale)[(*nrAnimale)] = citireAnimalFisier(file);
		(*nrAnimale)++;
	}
}

void afisareAnimal(AnimalZoo animal) {
	printf("numeAnimal: %s, varsta: %d, inaltime: %.2f, categorie: %s, greutate: %.2f, numeZoo: %s, nrHabitat: %d\n\n", animal.numeAnimal, animal.varsta, animal.inaltime, animal.categorie, animal.greutate, animal.numeZoo, animal.nrHabitat);

}

void afisareVector(AnimalZoo* animale, int nrAnimale) {
	for (int i = 0; i < nrAnimale; i++)
		afisareAnimal(animale[i]);
}

void InserareLaSfarsit(nod** cap, AnimalZoo animal)
{
	nod* nou;
	nou = (nod*)malloc(sizeof(nod));
	nou->info = animal;
	nou->urmator = NULL;
	if ((*cap) == NULL)
		*cap = nou;
	else
	{
		nod* temp = *cap;
		while (temp->urmator != NULL)
			temp = temp->urmator;
		temp->urmator = nou;
	}
}

void SortareLista(nod** cap)
{

	int sortat;
	nod* temp;
	nod* ultimul = NULL;

	do {
		sortat = 1;
		temp = *cap;

		while (temp->urmator != ultimul) {
			if (temp->info.varsta > temp->urmator->info.varsta) {
				AnimalZoo aux = temp->info;
				temp->info = temp->urmator->info;
				temp->urmator->info = aux;

				sortat = 0;
			}
			temp = temp->urmator;
		}
		ultimul = temp;
	} while (!sortat);
}

void traversareLista(nod* cap) {
	while (cap) {
		afisareAnimal(cap->info);
		cap = cap->urmator;
	}
}



void StergePrimul(nod** cap)
{
	nod* temp = *cap;
	*cap = (*cap)->urmator;
}


void StergereDupaVarsta(nod** cap, int varsta) {
	while (*cap != NULL && (*cap)->info.varsta == varsta) {
		*cap = (*cap)->urmator;
	}

	nod* temp = *cap;

	while (temp != NULL && temp->urmator != NULL) {
		if (temp->urmator->info.varsta == varsta) {
			temp->urmator = temp->urmator->urmator;
		}
		else {
			temp = temp->urmator;
		}
	}
}

void main() {
	FILE* file = fopen("animale.txt", "r");
	nod* cap = NULL;
	AnimalZoo* animale = NULL;
	int nrAnimale = 0;
	citireVectorAnimale(file, &animale, &nrAnimale);
	afisareVector(animale, nrAnimale);

	for (int i = 0; i < nrAnimale; i++)
		InserareLaSfarsit(&cap, animale[i]);
	SortareLista(&cap);

	StergePrimul(&cap);

	traversareLista(cap);

	StergereDupaVarsta(&cap, 5);
	traversareLista(cap);

	free(cap);
	free(animale);
	cap = NULL;
	animale = NULL;

}
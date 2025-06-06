#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//trebuie sa folositi fisierul masini.txt
//sau va creati un alt fisier cu alte date

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;
typedef struct nod nod;
struct nod {
	Masina info;
	nod* urm;

};

struct HashTable {
	int dim;
	nod** v;


};
typedef struct HashTable HashTable;

Masina citireMasinaDinFisier(FILE* file) {
	char buffer[100];
	char sep[3] = ",\n";
	fgets(buffer, 100, file);
	char* aux;
	Masina m1;
	aux = strtok(buffer, sep);
	m1.id = atoi(aux);
	m1.nrUsi = atoi(strtok(NULL, sep));
	m1.pret = atof(strtok(NULL, sep));
	aux = strtok(NULL, sep);
	m1.model = malloc(strlen(aux) + 1);
	strcpy_s(m1.model, strlen(aux) + 1, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy_s(m1.numeSofer, strlen(aux) + 1, aux);

	m1.serie = *strtok(NULL, sep);
	return m1;
}

void afisareMasina(Masina masina) {
	printf("Id: %d\n", masina.id);
	printf("Nr. usi : %d\n", masina.nrUsi);
	printf("Pret: %.2f\n", masina.pret);
	printf("Model: %s\n", masina.model);
	printf("Nume sofer: %s\n", masina.numeSofer);
	printf("Serie: %c\n\n", masina.serie);
}

void afisareListaMasini(nod* cap) {
	while (cap)
	{
		afisareMasina(cap->info);
		cap = cap->urm;
	}

}

void adaugaMasinaInLista(nod* cap, Masina masinaNoua) {

	nod* nou = (nod*)malloc(sizeof(nod));
	nou->info = masinaNoua;
	nou->urm = NULL;

	nod* temp = cap;

	while (temp->urm)
	{
		temp = temp->urm;
	}

	temp->urm = nou;




}


HashTable initializareHashTable(int dimensiune) {
	HashTable ht;
	ht.dim = dimensiune;
	ht.v = (nod**)malloc(sizeof(nod*) * dimensiune);
	for (int i = 0; i < dimensiune; i++)
	{
		ht.v[i] = NULL;
	}
	//initializeaza vectorul de liste si seteaza fiecare lista ca fiind NULL;
	return ht;
}

int calculeazaHash(const char* nume, int dimensiune) {
	// este calculat hash-ul in functie de dimensiunea tabelei si un atribut al masinii
	if (dimensiune == 0)
		return 0;
	int s = 0;
	for (int i = 0; i < strlen(nume); i++)
		s += nume[i];
	return s % dimensiune;
}

void inserareMasinaInTabela(HashTable hash, Masina masina) {
	int poz = calculeazaHash(masina.numeSofer, hash.dim);

	if (hash.v[poz] == NULL)//nu avem coliziune
	{
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->urm = NULL;
		nou->info = masina;
		hash.v[poz] = nou;
	}
	else//avem coliziune

	{
		adaugaMasinaInLista(hash.v[poz], masina);
	}



}

HashTable citireMasiniDinFisier(const char* numeFisier, int dim) {

	FILE* file = fopen(numeFisier, "r");
	HashTable ht = initializareHashTable(dim);

	while (!feof(file))
	{
		inserareMasinaInTabela(ht, citireMasinaDinFisier(file));
	}
	fclose(file);
	return ht;

	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	// aceste masini sunt inserate intr-o tabela de dispersie initializata aici
	//ATENTIE - la final inchidem fisierul/stream-ul
}

void afisareTabelaDeMasini(HashTable ht) {
	for (int i = 0; i < ht.dim; i++)
	{
		printf("Clusterul: %d \n", i);
		afisareListaMasini(ht.v[i]);
	}


	//sunt afisate toate masinile cu evidentierea clusterelor realizate
}




void dezalocareTabelaDeMasini(HashTable* ht) {
	//sunt dezalocate toate masinile din tabela de dispersie
}

float* calculeazaPreturiMediiPerClustere(HashTable ht, int* nrClustere) {
	//calculeaza pretul mediu al masinilor din fiecare cluster.
	//trebuie sa returnam un vector cu valorile medii per cluster.
	//lungimea vectorului este data de numarul de clustere care contin masini
	return NULL;
}

Masina getMasinaDupaCheie(HashTable ht, const char* nume) {
	Masina m;
	int hash = calculeazaHash(nume, ht.dim);
	if (ht.v[hash])
	{
		nod* p = ht.v[hash];
		while (p && strcmp(nume, p->info.numeSofer) != 0)
		{
			p = p->urm;
		}
		if (p)
		{
			m = p->info;
			m.model = malloc(strlen(p->info.model) + 1);
			m.numeSofer = malloc(strlen(p->info.numeSofer) + 1);
			strcpy_s(m.model, strlen(p->info.model) + 1, p->info.model);
			strcpy_s(m.numeSofer, strlen(p->info.numeSofer) + 1, p->info.numeSofer);

		}
	}

	//cauta masina dupa valoarea atributului cheie folosit in calcularea hash-ului
	//trebuie sa modificam numele functiei 
	return m;
}

int main() {

	HashTable ht = citireMasiniDinFisier("masini.txt", 5);
	afisareTabelaDeMasini(ht);

	Masina m = getMasinaDupaCheie(ht, "Gigel");
	afisareMasina(m);
	if (m.id != -1)

	{
		afisareMasina(m);
		free(m.model);
		free(m.numeSofer);
	}
	return 0;
}
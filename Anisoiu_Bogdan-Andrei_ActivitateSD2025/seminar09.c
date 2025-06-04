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
//creare structura pentru un nod dintr-un arbore binar de cautare
struct nod {
	Masina info;
	nod* dr;
	nod* st;

};

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


void adaugaMasinaInArbore(nod** radacina, Masina masinaNoua) {

	if (!(*radacina))
	{
		nod* nou = (nod*)malloc(sizeof(nod));
		nou->info = masinaNoua;
		(*radacina) = nou;
		nou->st = NULL;
		nou->dr = NULL;
	}
	else
	{
		if ((*radacina)->info.id > masinaNoua.id)
		{
			adaugaMasinaInArbore((*radacina)->st, masinaNoua);
		}
		else if ((*radacina)->info.id < masinaNoua.id)
		{
			adaugaMasinaInArbore((*radacina)->dr, masinaNoua);
		}

	}



	//adauga o noua masina pe care o primim ca parametru in arbore,
	//astfel incat sa respecte princiippile de arbore binar de cautare
	//dupa o anumita cheie pe care o decideti - poate fi ID
}

nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	nod* arbore = NULL;
	while (!feof(f))
	{
		adaugaMasinaInArbore(&arbore, citireMasinaDinFisier(f));
	}
	return arbore;


	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
}
void afisareINOrdine(nod* radacina)
{
	if (radacina)
	{
		afisareINOrdine(radacina->st);
		afisareMasina(radacina->info);
		afisareINOrdine(radacina->dr);
	}

}
void afisarePREOrdine(nod* radacina)
{
	if (radacina)
	{
		afisareMasina(radacina->info);
		afisarePREOrdine(radacina->st);
		afisarePREOrdine(radacina->dr);
	}

}

void afisarePOSTOrdine(nod* radacina)
{
	if (radacina)
	{

		afisarePOSTOrdine(radacina->st);
		afisarePOSTOrdine(radacina->dr);
		afisareMasina(radacina->info);
	}

}


void afisareMasiniDinArbore(/*arbore de masini*/) {
	//afiseaza toate elemente de tip masina din arborele creat
	//prin apelarea functiei afisareMasina()
	//parcurgerea arborelui poate fi realizata in TREI moduri
	//folositi toate cele TREI moduri de parcurgere
}

void dezalocareArboreDeMasini(nod** arbore) {


	if (*arbore)
	{
		dezalocareArboreDeMasini(&(*arbore)->st);
		dezalocareArboreDeMasini(&(*arbore)->dr);
		free((*arbore)->info.model);
		free((*arbore)->info.numeSofer);
		free((*arbore));
		*arbore = NULL;
	}



	//sunt dezalocate toate masinile si arborele de elemente
}

Masina getMasinaByID(nod* arbore, int id) {
	Masina m;

	if (arbore)
	{
		if (arbore->info.id < id)
		{
			return getMasinaByID(arbore->dr, id);
		}
		else if (arbore->info.id > id)
		{
			return getMasinaByID(arbore->st, id);
		}
		else
		{
			return arbore->info;
		}
	}


	return m;
}

int determinaNumarNoduri(/*arborele de masini*/) {
	//calculeaza numarul total de noduri din arborele binar de cautare
	return 0;
}

int calculeazaInaltimeArbore(/*arbore de masini*/) {
	//calculeaza inaltimea arborelui care este data de 
	//lungimea maxima de la radacina pana la cel mai indepartat nod frunza
	return 0;
}

float calculeazaPretTotal(nod* arbore) {


	if (arbore == NULL)
	{
		return 0;

	}
	float sumast = calculeazaPretTotal(arbore->st);
	float sumadr = calculeazaPretTotal(arbore->dr);

	return arbore->info.pret + sumast + sumadr;




	//calculeaza pretul tuturor masinilor din arbore.
	return 0;
}

float calculeazaPretulMasinilorUnuiSofer(/*arbore de masini*/ const char* numeSofer) {
	//calculeaza pretul tuturor masinilor unui sofer.
	return 0;
}

int main()
{
	nod* radacina = NULL;
	radacina = citireArboreDeMasiniDinFisier("masini.txt");
	afisareINOrdine(radacina);
	Masina m = getMasinaByID(radacina, 5);
	float pret = calculeazaPretTotal(radacina);
	printf("%d", pret);

	dezalocareArboreDeMasini(&radacina);


	return 0;
}
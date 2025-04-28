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
typedef struct Nod Nod;
typedef struct Lista Lista;
struct Nod {
	Masina info;
	Nod* urm;
	Nod* prec;
};

struct Lista {
	Nod* prim;
	Nod* ultimul;
};


//creare structura pentru un nod dintr-o lista dublu inlantuita

//creare structura pentru Lista Dubla 

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

void afisareListaMasiniInceput(Lista list) {
	Nod* p = list.prim;
	while (p)
	{
		afisareMasina(p->info);
		p = p->urm;

	}


}

void adaugaMasinaInLista(Lista* list, Masina masinaNoua) {

	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urm = NULL;
	p->prec = NULL;
	p->prec = list->ultimul;
	if (list->ultimul)
		list->ultimul->urm = p;
	else
		list->prim = p;
	list->ultimul = p;

	//adauga la final in lista primita o noua masina pe care o primim ca parametru
}

void adaugaLaInceputInLista(Lista* list, Masina masinaNoua) {

	Nod* p = (Nod*)malloc(sizeof(Nod));
	p->info = masinaNoua;
	p->urm = list->prim;
	p->prec = NULL;

	if (list->prim)
		list->prim->prec = p;
	else
		list->ultimul = p;
	list->prim = p;



	//adauga la inceputul listei dublu inlantuite o noua masina pe care o primim ca parametru
}

Lista citireLDMasiniDinFisier(const char* numeFisier) {

	FILE* f = fopen(numeFisier, "r");
	Lista list;
	list.prim = NULL;
	list.ultimul = NULL;
	while (!feof(f))
	{
		adaugaLaInceputInLista(&list, citireMasinaDinFisier(f));
	}
	fclose(f);

	return list;
}

void dezalocareLDMasini(Lista* list) {

	Nod* p = list->prim;
	while (p->urm != NULL)
	{
		free(p->info.numeSofer);
		free(p->info.model);
		p = p->urm;
		free(p->prec);

	}
	free(p->info.numeSofer);
	free(p->info.model);
	free(p);
	list->prim = NULL;
	list->ultimul = NULL;


}

float calculeazaPretMediu(Lista list) {

	Nod* temp = list.prim;
	float suma = 0;
	float cont = 0;
	while (temp)
	{
		suma += temp->info.pret;
		temp = temp->urm;
		cont++;
	}


	//calculeaza pretul mediu al masinilor din lista.
	return suma / cont;
}

void stergeMasinaDupaID(/*lista masini*/ int id) {
	//sterge masina cu id-ul primit.
	//tratati situatia ca masina se afla si pe prima pozitie, si pe ultima pozitie
}

char* getNumeSoferMasinaScumpa(/*lista dublu inlantuita*/) {
	//cauta masina cea mai scumpa si 
	//returneaza numele soferului acestei maasini.
	return NULL;
}

void afisareSfarsit(Lista list)
{
	Nod* p = list.ultimul;
	while (p)
	{
		afisareMasina(p->info);
	}
}

int main() {

	Lista list;
	FILE* f = "masini.txt";
	list = citireLDMasiniDinFisier(f);
	afisareListaMasiniInceput(list);
	float pretMediu = calculeazaPretMediu(list);
	printf("pret %.2f ", pretMediu);

	dezalocareLDMasini(&list);



}
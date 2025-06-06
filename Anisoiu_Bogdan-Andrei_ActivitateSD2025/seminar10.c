#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct StructuraMasina {
	int id;
	int nrUsi;
	float pret;
	char* model;
	char* numeSofer;
	unsigned char serie;
};
typedef struct StructuraMasina Masina;

//creare structura pentru un nod dintr-un arbore binar de cautare
typedef struct Nod Nod;
struct Nod {
	Masina info;
	Nod* st;
	Nod* dr;
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
	strcpy(m1.model, aux);

	aux = strtok(NULL, sep);
	m1.numeSofer = malloc(strlen(aux) + 1);
	strcpy(m1.numeSofer, aux);

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

int calculeazaInaltimeArbore(Nod* radacina) {
	if (radacina != NULL) {
		int stanga = calculeazaInaltimeArbore(radacina->st);
		int d = calculeazaInaltimeArbore(radacina->dr);
		int max = 0;

		return 1 + (stanga > d ? stanga : d);
	}
	else {
		return 0;
	}



}

void rotireStanga(Nod** radacina) {
	Nod* nou;
	nou = (*radacina)->dr;
	(*radacina)->dr = nou->st;
	nou->st = *radacina;

	*radacina = nou;

}

//dezechilibru in stanga
void rotireDreapta(Nod** radacina) {
	Nod* nou;
	nou = (*radacina)->st;
	(*radacina)->st = nou->dr;
	nou->dr = (*radacina);
	*radacina = nou;
}



int gradEchilibru(Nod* radacina) {
	if (radacina != NULL)
		return (calculeazaInaltimeArbore(radacina->st)) - calculeazaInaltimeArbore(radacina->dr);

	else
		return 0;
}

void adaugaMasinaInArboreEchilibrat(Nod** radacina, Masina masinaNoua) {

	if ((*radacina) != NULL) {
		if ((*radacina)->info.id > masinaNoua.id) {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->st), masinaNoua);
		}
		else {
			adaugaMasinaInArboreEchilibrat(&((*radacina)->dr), masinaNoua);
		}
		int grad = gradEchilibru(*radacina);
		//grad==2 ins dezechilibru in stanga
		if (grad == 2) {
			if (gradEchilibru((*radacina)->st) == 1) {

				rotireDreapta(radacina);
				//radacina ca noi deja primim Nod** radacina
			}
			else {
				rotireStanga(&((*radacina)->st));
				rotireDreapta(radacina);
			}
		}
		else if (grad == -2) {
			if (gradEchilibru((*radacina)->dr) == 1) {
				//daca e 1 avem situatia complexa
				rotireDreapta(&((*radacina)->dr));
			}
			rotireStanga(radacina);
		}
	}
	else {
		Nod* nou = (Nod*)malloc(sizeof(Nod));
		nou->info = masinaNoua;
		nou->dr = NULL;
		nou->st = NULL;
		(*radacina) = nou;

	}


}




Nod* citireArboreDeMasiniDinFisier(const char* numeFisier) {
	//functia primeste numele fisierului, il deschide si citeste toate masinile din fisier
	//prin apelul repetat al functiei citireMasinaDinFisier()
	//ATENTIE - la final inchidem fisierul/stream-ul
	FILE* file = fopen(numeFisier, "r");
	Nod* radacina = NULL;
	while (!feof(file))
	{
		Masina m = citireMasinaDinFisier(file);
		adaugaMasinaInArboreEchilibrat(&radacina, m);
	}
	fclose(file);
	return radacina;
}

void afisareMasiniDinArborePREORDINE(Nod* radacina) {


	if (radacina != NULL) {
		afisareMasina(radacina->info);
		afisareMasiniDinArborePREORDINE(radacina->st);
		afisareMasiniDinArborePREORDINE(radacina->dr);
	}
}

void dezalocareArboreDeMasini(Nod** radacina) {
	
	if ((*radacina) != NULL)
	{
		dezalocareArboreDeMasini(&(*radacina)->st);
		dezalocareArboreDeMasini(&(*radacina)->dr);
		free((*radacina)->info.model);
		free((*radacina)->info.numeSofer);
		free(*radacina);
		*radacina = NULL;
	}
}



Masina getMasinaByID(Nod* radacina, int id) {
	Masina m;
	m.id = -1;

	if (radacina->info.id == id) {
		Masina m;
		m = radacina->info;
		m.numeSofer = (char*)malloc(strlen(radacina->info.numeSofer) + 1);
		strcpy(m.numeSofer, radacina->info.numeSofer);
		m.model = (char*)malloc(strlen(radacina->info.model) + 1);
		strcpy(m.model, radacina->info.model);
		return m;

	}
	else if (radacina->info.id > id) {
		return getMasinaByID(radacina->dr, id);
	}
	else
		return getMasinaByID(radacina->st, id);

}

int determinaNumarNoduri(Nod* radacina) {

	if (radacina != NULL)
		return 1 + determinaNumarNoduri(radacina->st) + determinaNumarNoduri(radacina->dr);

	return 0;
}


float calculeazaPretTotal(Nod* radacina) {
	if (radacina == NULL)
		return 0;

	return radacina->info.pret + calculeazaPretTotal(radacina->dr) + calculeazaPretTotal(radacina->st);
}

float calculeazaPretulMasinilorUnuiSofer(Nod* radacina, const char* numeSofer) {
	if (radacina != NULL) {
		float suma = 0;
		if (strcmp(radacina->info.numeSofer, numeSofer) == 0) {
			suma += radacina->info.pret;
		}
		suma += calculeazaPretulMasinilorUnuiSofer(radacina->dr, numeSofer) + calculeazaPretulMasinilorUnuiSofer(radacina->st, numeSofer);
		//suma+=calculeazaPretulMasinilorUnuiSofer(radacina->st,numeSofer);
		return suma;
	}
	return 0;
}


int main() {

	Nod* arbore = citireArboreDeMasiniDinFisier("masini.txt");
	afisareMasiniDinArborePREORDINE(arbore);

	Masina m = getMasinaByID(arbore, 4);
	printf("\n\n\n");
	afisareMasina(m);

	int p = determinaNumarNoduri(arbore);
	printf("\n\n nr noduri: %d", p);


	float sof = calculeazaPretulMasinilorUnuiSofer(arbore, "Ionescu");
	printf("\n\nval flota: %.2f", sof);


	return 0;
}
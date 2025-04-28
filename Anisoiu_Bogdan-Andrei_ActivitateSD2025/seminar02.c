#include<stdio.h>
#include<malloc.h>

struct Masina {
		int id;
		int nrLocuri;
		char* marca;
		float capacitateC;
		char normaPoluare;
	};
struct Masina initializare(int id, int nrLocuri, const char* marca, float capacitateC, char normaPoluare) {
		struct Masina m;
		m.id = id;
		m.nrLocuri = nrLocuri;
		m.capacitateC = capacitateC;
		m.normaPoluare = normaPoluare;
		m.marca = (char*)malloc(strlen(marca) + 1);
		strcpy_s(m.marca, strlen(marca) + 1, marca);
	
	
		return m;
	}

	void afisare(struct Masina m) {
			printf("Id masina: %d nrLocuri: %d Capacitate: %f Marca: %s Norma Poluare: %c \n", m.id, m.nrLocuri, m.capacitateC, m.marca, m.normaPoluare);
		
		}

void afisareVector(struct Masina* vector, int nrElemente) {
	//afisarea elementelor din vector apeland functia afisare
}

struct Masina* copiazaPrimeleNElemente(struct Masina* vector, int nrElemente, int nrElementeCopiate) {
	//copiem intr-un vector nou pe care il vom returna primele nrElementeCopiate
	struct Masina *vectorNou=NULL;

	return vectorNou;
}


void dezalocare(struct Masina** vector, int* nrElemente) {
	
	for (int i = 0; i < (*nrElemente); i++)
	{
		free((*vector)[i].marca);
	}
	free(*vector);
	*vector = NULL;


}
void copiazaAnumiteElemente(struct Masina* vector, char nrElemente, float prag, struct Masina** vectorNou, int* dimensiune) {
	//parametrul prag poate fi modificat in functie de 
	// tipul atributului ales pentru a indeplini o conditie
	//este creat un nou vector cu elementele care indeplinesc acea conditie
}

struct Masina getPrimulElementConditionat(struct Masina* vector, int nrElemente, const char* conditie) {
	//trebuie cautat elementul care indeplineste o conditie
	//dupa atributul de tip char*. Acesta este returnat.
	struct Masina m;
	m.id = 1;

	return m;
}
	


int main() {

	struct Masina* vector;
	int nrElemente = 3;
		vector = (struct Masina*)malloc(nrElemente * sizeof(struct Masina));
		vector[0] = initializare(1, 3, "Dacia", 40, '5');
		vector[1] = initializare(2, 5, "Honda", 30, '6');
		vector[2] = initializare(3, 5, "Dacia", 50, '3');
	
}
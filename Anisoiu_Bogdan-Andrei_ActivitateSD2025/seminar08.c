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

//creare structura pentru Heap
//un vector de elemente, lungimea vectorului si numarul de elemente din vector
struct Heap {
	int lungime;
	Masina* masini;
	int nrMasini;
};
typedef struct Heap Heap;

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

Heap initializareHeap(int lungime) {
	Heap heap;
	heap.lungime = lungime;
	heap.masini = (Masina*)malloc(sizeof(Masina) * lungime);
	heap.nrMasini = 0;
	return heap;
}

void filtreazaHeap(Heap heap, int pozitieNod) {
	if (pozitieNod >= 0 && pozitieNod < heap.nrMasini)
	{
		int pozst = 2 * pozitieNod + 1;
		int pozdr = 2 * pozitieNod + 2;

		int pozmaxim = pozitieNod;
		if (pozst<heap.nrMasini && heap.masini[pozst].id > heap.masini[pozmaxim].id)
		{
			pozmaxim = pozst;
		}
		if (pozdr<heap.nrMasini && heap.masini[pozdr].id > heap.masini[pozmaxim].id)
		{
			pozmaxim = pozdr;
		}
		if (pozmaxim != pozitieNod)
		{
			Masina aux;
			aux = heap.masini[pozmaxim];
			heap.masini[pozmaxim] = heap.masini[pozitieNod];
			heap.masini[pozitieNod] = aux;
			if (pozmaxim < (heap.nrMasini - 1) / 2)
			{
				filtreazaHeap(heap, pozmaxim);
			}

		}



	}

}

Heap citireHeapDeMasiniDinFisier(const char* numeFisier) {

	Heap heap = initializareHeap(10);
	FILE* file = fopen(numeFisier, "r");
	int i = 0;
	while (!feof(file))
	{
		heap.masini[i] = citireMasinaDinFisier(file);
		i++;
	}
	heap.nrMasini = i;
	fclose(file);

	for (int i = (heap.nrMasini - 1) / 2; i >= 0; i--)
	{
		filtreazaHeap(heap, i);
	}

	return heap;

	//citim toate masinile din fisier si le stocam intr-un heap 
	// pe care trebuie sa il filtram astfel incat sa respecte 
	// principiul de MAX-HEAP sau MIN-HEAP dupa un anumit criteriu
	// sunt citite toate elementele si abia apoi este filtrat vectorul
}

void afisareHeap(Heap heap) {
	for (int i = 0; i < heap.nrMasini; i++)
		afisareMasina(heap.masini[i]);
}

void afiseazaHeapAscuns(Heap heap) {
	for (int i = heap.nrMasini + 1; i < heap.lungime - 1; i++)
	{
		afisareMasina(heap.masini[i]);

	}
}

Masina extrageMasina(Heap* heap) {

	Masina aux = heap->masini[0];
	heap->masini[0] = heap->masini[heap->nrMasini - 1];
	heap->masini[heap->nrMasini] = aux;
	heap->nrMasini--;
	for (int i = (heap->nrMasini - 1) / 2; i >= 0; i--)
		filtreazaHeap(*heap, i);

	//extrage si returneaza masina de pe prima pozitie
	//elementul extras nu il stergem...doar il ascundem
}


void dezalocareHeap(Heap* heap) {
	for (int i = 0; i < heap->lungime; i++)
	{
		free(heap->masini[i].numeSofer);
		free(heap->masini[i].model);
	}
	free(heap->masini);
	heap->nrMasini = 0;
	heap->masini = NULL;
}

int main() {

	Heap heap = citireHeapDeMasiniDinFisier("masini.txt");
	//afisareHeap(heap);
	//afisareHeapAscuns(heap);
	extrageMasina(&heap);
	extrageMasina(&heap);
	extrageMasina(&heap);
	extrageMasina(&heap);
	extrageMasina(&heap);
	extrageMasina(&heap);
	afiseazaHeapAscuns(heap);


	return 0;
}
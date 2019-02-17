/*GHILESCU TEODOR-ANDREI 314CB*/
#include <stdio.h>
#include <stdlib.h>
#include "info.h"
#include "functions.h"

int main(int argc, char *argv[]){
	FILE *f = fopen(argv[1], "r+");
	FILE *g = fopen(argv[3], "w+");
	FILE *h = fopen(argv[2], "r+");
	if(!f){
		printf("error\n");
		return 0;
	}
	//creeaza o sala
	Sala sala;
	sala = malloc(sizeof(sala));
	fscanf(f, "%d", &(sala->nrMese));
	sala->masa = NULL;
	sala->masa = CreateListofTables(f, sala->nrMese);
	fclose(f);
	//citeste din fisierul de evenimente
	char eveniment[20], masa[20], jucator[20]; 
	uint nr = 0;
	while(fscanf(h, "%s", eveniment) != EOF){
		if(strcmp(eveniment, "print") == 0){
			if(sala->masa == NULL){
				fprintf(g, "Sala este inchisa!\n");
			}else{
				PrintSala(g, sala->masa, sala->nrMese);
				//printf("sala2\n");
			}
		}
		if(strcmp(eveniment, "noroc") == 0){
			fscanf(h, "%s", masa);
			fscanf(h, "%s", jucator);
			fscanf(h, "%d", &nr);
			CresteNoroc(g, sala->masa, masa, jucator, nr, 1);
			//printf("sala3\n");
		}
		if(strcmp(eveniment, "ghinion") == 0){
			fscanf(h, "%s", masa);
			fscanf(h, "%s", jucator);
			fscanf(h, "%d", &nr);
			CresteNoroc(g, sala->masa, masa, jucator, nr, -1);
		}
		if(strcmp(eveniment, "inchide") == 0){
			fscanf(h, "%s", masa);
			InchideMasa(g, &sala, masa);
		}
		if(strcmp(eveniment, "tura") == 0){
			fscanf(h, "%s", masa);
			TuraMasa(g, sala, masa);
		}
		if(strcmp(eveniment, "tura_completa") == 0){
			TuraCompleta(g, sala);
		}
		if(strcmp(eveniment, "clasament") == 0){
			fscanf(h, "%s", masa);
			Clasament(g, sala, masa);
		}
	}
	Elibereaza(sala);
	fclose(g);
	fclose(h);
	return 0;
}
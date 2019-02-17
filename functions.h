/*GHILESCU TEODOR-ANDREI 314CB*/
#include "info.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//headerele functiilor
TLista Alloc(void *x, size_t d);
int Insert(void *x, ALista L, size_t d);
TLista CreateListofPlayers(FILE *f, int nr_Players);
void AfiJucator(FILE *f, void *ae);
void PrintMasa(FILE *f, TLista jucatori);
void AfiTable(FILE *f, void *ae);
void PrintSala(FILE *f, TLista sala, int nr_Tables);
TLista CreateListofTables(FILE *f, int nr_Tables);
void CresteNoroc(FILE *f, TLista sala, char *masa, 
				char *jucator, uint nr, signed char ghinor);
void EliminaJucator(TLista L, char *nume);
void InchideMasa(FILE *f, Sala *sala, char *nume);
void MutaJucator(TLista lj1, TLista lj2);
void TuraMasa(FILE *f, Sala sala, char *masa);
void RotesteJucatori(TLista L);
void TuraCompleta(FILE *f, Sala sala);
void Clasament(FILE *f, Sala sala, char *masa);
TLista Claseaza(TLista L);

//aloca memorie pentru o celula generica
TLista Alloc(void *x, size_t d){
	TLista aux;
	aux = malloc(sizeof(Celula));
	if(!aux)
		return NULL;
	aux->info = malloc(d);
	if(!aux->info){
		free(aux);
		return NULL;
	}
	memcpy(aux->info, x, d);
	aux->urm = NULL;
	return aux;
}
//insereaza o celula generica intr-o lista generica
int Insert(void *x, ALista L, size_t d){
	TLista aux, p;
	aux = Alloc(x, d);
	if(!aux) return 0;
	if(*L == NULL){
		aux->urm = NULL;
		*L = aux;
	}else{
		p = *L;
		while(p->urm != NULL){
			p = p->urm;
		}
		p->urm = aux;
		aux->urm = NULL;
	}
	return 1;
}
//creeaza o lista de jucatori
TLista CreateListofPlayers(FILE *f, int nr_Players){
	TLista L = NULL;
	Jucator aux;
	int i;
	for(i = 0;i < nr_Players;i++){
		aux = malloc(sizeof(struct jucator));
		aux->nume = malloc(25*sizeof(char));
		fscanf(f, "%s%d", aux->nume, &(aux->nrMaini));
		if(!Insert(aux, &L, sizeof(struct jucator))){
			return L;
				TLista p = L;
			while(p->urm != NULL){
				p = p->urm;
			}		
			p->urm = L;
		}
	}
	//introduce santinela
	TLista s;
	s = malloc(sizeof(Celula));
	s->urm = L;
	L = s;
	//face lista circulara
	TLista p = L;
	while(p->urm != NULL){
		p = p->urm;
	}
	p->urm = L;
	return L;
}
//afiseaza un jucator
void AfiJucator(FILE *f, void *ae){
	Jucator p = (Jucator)ae;
	fprintf(f, "%s - %d", p->nume, p->nrMaini);
}
//afiseaza o lista de jucatori
void PrintMasa(FILE *f, TLista jucatori){
	TLista p = jucatori->urm;
	while(p != jucatori){
		AfiJucator(f, p->info);
		if(p->urm == jucatori){
			fprintf(f, ".\n");
		}else{
			fprintf(f, "; ");
		}
		p = p->urm;
	}
}
//afiseaza elementele unei mese
void AfiTable(FILE *f, void *ae){
  Masa p = (Masa)ae;
  if(p->nrCrtJucatori > 0){
  	fprintf(f, "%s: ", p->numeMasa);
  	PrintMasa(f, p->jucatori);
  }
}
//afiseaza o lista de mese
void PrintSala(FILE *f, TLista sala, int nr_Tables){
    int i;
    TLista p = sala;
    for (i = 0;i < nr_Tables;i++){
    	AfiTable(f, p->info);
    	p = p->urm;
    }
}
//creeaza o lista de mese
TLista CreateListofTables(FILE *f, int nr_Tables){
	int i;
	TLista L = NULL;
	Masa aux;
	for(i = 0;i < nr_Tables;i++){
		aux = malloc(sizeof(struct masa));
		aux->numeMasa = malloc(25*sizeof(char));
		fscanf(f, "%s%d%d", aux->numeMasa, 
				&(aux->nrCrtJucatori), &(aux->nrMaxJucatori));
		//insereaza jucatorii in celula masa
		aux->jucatori = CreateListofPlayers(f, aux->nrCrtJucatori);
		if(!Insert(aux, &L, sizeof(struct masa))){
			return L;
		}
	}
	return L;
}
//compara doua nume de masa
int strmcomp(void *ae, char *masa){
	Masa p = (Masa)ae;
	return strcmp(p->numeMasa, masa);
}
//compara doua nume de jucatori
int strjcomp(void *ae, char *jucator){
	Jucator p = (Jucator)ae;
	return strcmp(p->nume, jucator);
}
//elimina jucatorul [nume] de la masa lui
void EliminaJucator(TLista L, char *nume){
	TLista p = L->urm;
	TLista prec = L;
	//cauta jucatorul
	while(p != L && strjcomp(p->info, nume) != 0){
		p = p->urm;
		prec = prec->urm;
	}
	//reface legaturile
	prec->urm = p->urm;
	//sterge celula
	if(p){
		free(p->info);
		p->info = NULL;
		free(p);
		p = NULL;
	}
}
//creste sau scade norocul unui jucator
void CresteNoroc(FILE *f, TLista sala, char *masa, 
				char *jucator, uint nr, signed char ghinor){
	TLista p = sala;
	//cauta masa
	while(p != NULL && (strmcomp(p->info, masa) != 0)){
		p = p->urm;
	}
	if(p != NULL){
		Masa mas = (Masa)(p->info);
		//cauta jucatorul
		TLista juc = mas->jucatori->urm;
		while(juc != mas->jucatori && (strjcomp(juc->info, jucator)) != 0){
			juc = juc->urm;
		}
		if(juc != mas->jucatori){
			Jucator c = (Jucator)(juc->info);
			if(ghinor > 0){
				c->nrMaini += nr;
			}
			else{
				c->nrMaini -= nr;
				if(c->nrMaini <= 0){
					EliminaJucator(mas->jucatori, c->nume);
					mas->nrCrtJucatori--;
				}
			}
		}else{
			fprintf(f,"Jucatorul %s nu exista la masa %s!\n", jucator, masa);
		}
	}else{
		fprintf(f,"Masa %s nu exista!\n", masa);
	}
}
//numara locurile libere
int LocuriLibere(TLista L){
	int nrLocuri = 0;
	TLista p = L;
	Masa masa;
	p = L;
	while(p != NULL){
		masa = (Masa)(p->info);
		nrLocuri += masa->nrMaxJucatori - masa->nrCrtJucatori;
		p = p->urm;
	}
	return nrLocuri;
}
//muta un jucator
void MutaJucator(TLista lj1, TLista lj2){
	TLista p1 = lj1->urm, p2 = lj2->urm;
	while(p2->urm != lj2){
		p2 = p2->urm;
	}
	//muta primul jucator din lista lj1 in lj2
	lj1->urm = p1->urm;
	p1->urm = lj2;
	p2->urm = p1;
}
//desfiinteaza masa
void StergeMasa(Sala *sala, TLista m, char *nume){
	TLista L = (*sala)->masa;
	TLista p = L;
	if(p != m){
		while(strmcomp(p->urm->info, nume) != 0){
			p = p->urm;
		}
	}else{
		(*sala)->masa = m->urm;
	}
	p->urm = m->urm;
	//elibereaza memoria mesei
	free(((Masa)(m->info))->jucatori);
	((Masa)(m->info))->jucatori = NULL;
	free(m);
	m = NULL;
	(*sala)->nrMese--;
}
//inchide o masa
void InchideMasa(FILE *f, Sala *sala, char *nume){
	TLista L = (*sala)->masa;
	int locuri = LocuriLibere(L);
	TLista p = L;
	//cauta masa
	while(strmcomp(p->info, nume) != 0){
		p = p->urm;
	}
	if(p == NULL){
		fprintf(f, "Masa %s nu exista!\n", nume);
		return;
	}
	Masa masa = (Masa)(p->info);
	if(masa->nrCrtJucatori > locuri){
		fprintf(f, "Nu exista suficiente locuri in sala!\n");
		return;
	}
	TLista m = p;
	p = L;
	int mutati = 0, i;
	while(mutati < masa->nrCrtJucatori){
		if(strmcomp(p->info, nume) != 0){
			locuri = ((Masa)(p->info))->nrMaxJucatori - 
					((Masa)(p->info))->nrCrtJucatori;
			for(i = 0;i < locuri && mutati < masa->nrCrtJucatori;i++){
				MutaJucator(((Masa)(m->info))->jucatori, 
							((Masa)(p->info))->jucatori);
				mutati ++;
				(((Masa)(p->info))->nrCrtJucatori)++;
			}
		}
		p = p->urm;
	}
	//desfiinteaza masa
	StergeMasa(sala, m, nume);
}
//roteste jucatorii de la o masa
void RotesteJucatori(TLista L){
	TLista p = L->urm;
	while(p->urm != L){
		p = p->urm;
	}
	//muta primul pe ultimul loc
	p->urm = L->urm;
	L->urm = L->urm->urm;
	p->urm->urm = L;
}
//tura unei mese
void TuraMasa(FILE *f, Sala sala, char *masa){
	TLista L = sala->masa;
	TLista p = L;
	while(strmcomp(p->info, masa) != 0){
		p = p->urm;
	}
	if(p == NULL){
		fprintf(f, "Masa %s nu exista!", masa);
	}
	Masa mas = (Masa)(p->info);
	TLista m = p;
	RotesteJucatori(mas->jucatori);
	//decrementeaza turele
	TLista juc = mas->jucatori;
	p = juc->urm;
	while(p != juc){
		(((Jucator)(p->info))->nrMaini)--;
		if(((Jucator)(p->info))->nrMaini <= 0){
					EliminaJucator(juc, ((Jucator)(p->info))->nume);
					(mas->nrCrtJucatori)--;
		}
		p = p->urm;
	}
	if(mas->nrCrtJucatori == 0){
		StergeMasa(&sala, m, masa);
	}
}
//tura completa
void TuraCompleta(FILE *f, Sala sala){
	TLista p = sala->masa;
	while(p != NULL){
		TuraMasa(f, sala, ((Masa)(p->info))->numeMasa);
		p = p->urm;
	}
}
//compara 2 jucatori
int ComparaJucatori(Jucator a, Jucator b){
	if(a->nrMaini == b->nrMaini){
		return strcmp(a->nume, b->nume);
	}
	return a->nrMaini - b->nrMaini;
}
//insereaza ordonat
int InsereazaOrdonat(void *x, ALista L, size_t d){
	TLista aux, p, q;
	aux = Alloc(x, d);
	Jucator juc = (Jucator)x;
	if(!aux){
		return 0;
	}
	if(*L == NULL){
		*L = aux;
		aux->urm = NULL;
		return 1;
	}
	p = *L;
	//insereaza la inceput
	if(ComparaJucatori(juc, (Jucator)(p->info)) > 0){
		aux->urm = *L;
		*L = aux;
		return 1;
	}
	q = p;
	p = p->urm;
	//insereaza la mijloc sau sfarsit
	while(p != NULL && (ComparaJucatori(juc, (Jucator)(p->info)) <= 0)){
		q = p;
		p = p->urm;
	}
	aux->urm = q->urm;
	q->urm = aux;
	return 1;
}
//creeaza lista de clasament
TLista Claseaza(TLista L){
	TLista Clasament = NULL;
	Jucator aux;
	TLista p = L->urm;
	while(p != L){
		aux = malloc(sizeof(struct jucator));
		aux->nume = malloc(25 * sizeof(char));
		aux->nume = ((Jucator)(p->info))->nume;
		aux->nrMaini = ((Jucator)(p->info))->nrMaini;
		InsereazaOrdonat(aux, &Clasament, sizeof(struct jucator));
		p = p->urm;
	}
	return Clasament;
}
void AfiClasament(FILE *f, void *ae){
	Jucator p = (Jucator)ae;
	fprintf(f, "%s %d\n", p->nume, p->nrMaini);
}
//clasament
void Clasament(FILE *f, Sala sala, char *masa){
	TLista L = sala->masa;
	TLista p = L;
	while(p != NULL && strmcomp(p->info, masa) != 0){
		p = p->urm;
	}
	if(p == NULL){
		fprintf(f, "Masa %s nu exista!\n", masa);
		return;
	}
	TLista clasament = Claseaza(((Masa)(p->info))->jucatori);
	p = clasament;
	fprintf(f, "Clasament %s:\n", masa);
	while(p != NULL){
		AfiClasament(f, p->info);
		p = p->urm;
	}
}
void Elibereaza(Sala sala){
	TLista pm = sala->masa, pj, auxm, auxj;
	while(pm != NULL){
		pj = ((Masa)(pm->info))->jucatori->urm;
		while(pj != ((Masa)(pm->info))->jucatori){
			auxj = pj;
			pj = auxj->urm;
			free(((Jucator)(auxj->info))->nume);
			free(auxj);
		}
		auxm = pm;
		pm = pm->urm;
		free(((Masa)(auxm->info))->jucatori);
		free(((Masa)(auxm->info))->numeMasa);
		free(((Masa)(auxm->info)));
		free(auxm);
	}
	free(sala);
}





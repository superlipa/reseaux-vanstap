#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "data.h"

void AProposServeurFJ(char *Version,char *Nom1,char* Nom2) ;
int RechercheFJ(char* NomFichier,int Reference ,struct VehiculeFJ *UnRecord);
int reservationFJ(char* NomFichier,int Reference,int quantite);
int facturationFJ(char* NomFichier,char NomClient[40],time_t Date,int quantite,int Reference);

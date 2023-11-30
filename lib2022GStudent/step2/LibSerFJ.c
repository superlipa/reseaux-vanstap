#include "LibSerFJ.h"

void AProposServeurFJ(char *Version,char *Nom1,char* Nom2)
{
 printf("Version : %s \n",Version ) ;
 printf("Nom1 : %s \n",Nom1 ) ;
 printf("Nom2 : %s \n",Nom2 ) ;
}
int RechercheFJ(char* NomFichier,int Reference ,struct VehiculeFJ *UnRecord)
{
struct VehiculeFJ   temp ;
 FILE *fp ;

 fp = fopen(NomFichier,"r") ; /* Si le fichier existe, on le cree sinon on ajoute */
 if ( fp == NULL )
    {
     fprintf(stderr,"Echec Ouverture\n") ;
     exit(0) ;
    }
 else
    fprintf(stderr,"Ouverture reussie \n") ;
 while (fread(&temp,sizeof(temp),1,fp) )
 {
  if ((temp.Reference)==Reference)
    {
        UnRecord->Reference =temp.Reference;
        strcpy(UnRecord->Constructeur,temp.Constructeur);
        strcpy(UnRecord->Modele,temp.Modele);
        UnRecord->puissance  =temp.puissance;
        UnRecord->Quantite =temp.Quantite;
        fclose(fp);
        return 1;
    }
 }
 fclose(fp);
 return 0;
}


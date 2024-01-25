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
int compteur = 0;
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
  compteur++;
  if ((temp.Reference)==Reference)
    {
        UnRecord->Reference =temp.Reference;
        strcpy(UnRecord->Constructeur,temp.Constructeur);
        strcpy(UnRecord->Modele,temp.Modele);
        UnRecord->puissance  =temp.puissance;
        UnRecord->Quantite =temp.Quantite;
        fclose(fp);
        return compteur + 1;
    }
 }
 fclose(fp);
 return 0;
}
int reservationFJ(char* NomFichier,int Reference,int quantite)
{
  struct VehiculeFJ   temp ;
 FILE *fp ;

 fp = fopen(NomFichier,"r+") ; /* Si le fichier existe, on le cree sinon on ajoute */
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
      printf("VehiculeFJ trouver\n");
      fseek(fp, -sizeof(temp), SEEK_CUR);

         // Modifier la quantité
         if (temp.Quantite >= quantite) 
         {
            temp.Quantite= temp.Quantite-quantite;
            // Écrire la structure modifiée dans le fichier
            fwrite(&temp, sizeof(temp), 1, fp);
            printf("voiture dispo : %s %s %d \n",temp.Constructeur,temp.Modele,temp.Quantite);
            fclose(fp);
            return 1;
         }
         else
         {
            printf("vehicule en trop faible quantite");
            return 0;
         }
    }
 }
 printf("vehicule non trouvable ref inconnue");
 fclose(fp);
 return 0; 
}
int facturationFJ(char* NomFichier,char NomClient[40],time_t Date,int quantite,int Reference)
{
   // Ouvrir le fichier en mode ajout (a+)
   FILE *file = fopen(NomFichier, "a+");
 struct FactureFJ temp;
    
   if (file == NULL) {
      fprintf(stderr,"Echec Ouverture\n") ;
      exit(0) ;
   }
   // Lire le dernier numéro de facturation existant dans le fichier
   int dernierNumeroFacturation = 0;
   fseek(file, -sizeof(struct FactureFJ), SEEK_END);
   fread(&temp, sizeof(struct FactureFJ), 1, file);
struct FactureFJ nouvelleFacture;
 nouvelleFacture.NumeroFacturation = temp.NumeroFacturation + 1;
   // Incrémenter le numéro de facturation
           printf("constr facture\n");
   // Construire une nouvelle facture avec les données fournies
             printf("constr avant nom facture\n");
   strncpy(nouvelleFacture.Acheteur, NomClient, sizeof(nouvelleFacture.Acheteur) - 1);
   nouvelleFacture.Acheteur[sizeof(nouvelleFacture.Acheteur) - 1] = '\0';
   nouvelleFacture.DateFacturation = Date;
   nouvelleFacture.Quantite = quantite;
   nouvelleFacture.Reference = Reference;
           printf("write facture\n");
   // Écrire la nouvelle facture à la fin du fichier
   fwrite(&nouvelleFacture, sizeof(struct FactureFJ), 1, file);

   // Fermer le fichier
   fclose(file);

   return nouvelleFacture.NumeroFacturation;
}
int testdoublonserv(char *FileFacture, time_t* Date, char* NomClient, int *NumeroFacture)
{
    FILE* fd;
    char temps[30], temps1[30];
    if ((fd = (fopen(FileFacture, "rb"))) == NULL)
    {
        printf("Erreur ouverture fichier dans la fonction RechercheFJ");
        return-1;
    }
    struct FactureFJ *Facture = (struct FactureFJ*) malloc(sizeof(struct FactureFJ));
    while(fread(Facture, sizeof(struct FactureFJ), 1, fd) && (*Date) != Facture->DateFacturation);
    while(fread(Facture, sizeof(struct FactureFJ), 1, fd) && (*Date) == Facture->DateFacturation && (strcmp(NomClient, Facture->Acheteur)));
    fclose(fd);
    strftime(temps,sizeof(temps), "%Y-%m-%d %H:%M",localtime(&Facture->DateFacturation));
    strftime(temps1,sizeof(temps), "%Y-%m-%d %H:%M",localtime(Date));
    if((strcmp(temps1, temps)) == 0 && (strcmp(NomClient, Facture->Acheteur)) == 0)
    {
        *NumeroFacture = Facture->NumeroFacturation;
        *Date = Facture->DateFacturation;
        free(Facture);
        return 1;
    }
    free(Facture);
    return 0;
}


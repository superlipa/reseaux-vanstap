/*---------------------------------------------------------------
   Vanstapel Herman
   ex02\cli.c

 Le client dit bonjour en utilisant un structure  et
 le serveur fait de même
------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>
#include "../udplib/udplib.h"
#include "requeteFJ.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>
#include <time.h>

void handlerSIGALRM(int signal);

void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc, char *argv[])
{
   int choix;
 int rc ;
 int Desc ;
 int tm ; 

    struct sigaction actionAlarme;
    sigfillset(&actionAlarme.sa_mask);
    sigdelset(&actionAlarme.sa_mask, SIGALRM);
    actionAlarme.sa_handler = handlerSIGALRM;
    actionAlarme.sa_flags = 0;

    if ((sigaction(SIGALRM, &actionAlarme, NULL) == -1))
    {
	    perror("Erreur armement signal\n");
	    exit(1);
    }
    
   
 u_long  IpSocket , IpServer;
 u_short PortSocket, PortServer ; 
 
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
 struct RequeteFJ UneRequete ;

 memset(&sthis,0,sizeof(struct sockaddr_in)) ;
 memset(&sos,0,sizeof(struct sockaddr_in)) ; 
 memset(&sor,0,sizeof(struct sockaddr_in)) ; 
 
 if (argc!=5)
  
{
    printf("cli client portc serveur ports\n") ;
    exit(1) ;
 }
 
 /* Récupération IP & port   */
 IpSocket= inet_addr(argv[1]);
 PortSocket = atoi(argv[2]);
 
 IpServer = inet_addr(argv[3]) ;
 PortServer = atoi(argv[4]);
 
 // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4])) ;
 Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);
 
 if ( Desc == -1 )
    die("CreateSockets:") ; 
 else
    fprintf(stderr,"CreateSockets %d\n",Desc) ;
 
  sos.sin_family = AF_INET ;
  sos.sin_addr.s_addr= IpServer ;
  sos.sin_port = htons(PortServer) ;

compteur = 1; 

do
{
   memset(&UneRequete,0,sizeof(struct RequeteMS));
   memset(&RequeteReponse,0,sizeof(struct RequeteMS));
   UneRequete.Type = Question;
   UneRequete.Numero = compteur; 

  printf("-------2023-----------\n") ;
  printf("1) Demander une reference\n") ;
  printf("2) acheter une voiture\n") ;
  printf("3) quitter            \n") ;  
  printf("----------------------\n") ; 
  printf(">>") ;
  scanf("%d",&choix);
  printf("\n") ;
  switch(choix)
  {
   case 1: 
   UneRequete.TypeReponse = Question ; 
   //strncpy(UneRequete.Message , "Avec une structure: Bonjour" , sizeof(UneRequete.Message)) ;
   int ref;
         printf("Quelle est la Référence du modèle recherché?\n");
         scanf("%d",&ref);
         UneRequete.Reference=ref; 
   break ;
   case 2:
   {
      UneRequete.TypeReponse = Achat ;
      char nom[40];
      int ref,quantite;
      printf("Nom client:\n");
      scanf("%s",&nom);
      printf("Quelle est la Référence du modèle:\n");
      scanf("%d",&ref);
      printf("Quelle est la Quantité du modèle:\n");
      scanf("%d",&quantite);
            UneRequete.Reference = ref;
            UneRequete.Quantite = quantite;
            // Assigner le nom du client
            strcpy(UneRequete.NomClient, nom);

   }

   break;
   case 3:
      exit(0) ; 
      close(Desc) ;
  }
   sigset_t mask, ancienMask;
   sigaddset(&mask,SIGALRM);
   rc = -1;
    do
    {
        alarm(5);
        if (rc == -1)
        {
            rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteMS) ,&sos ) ;

            if ( rc == -1 )
                die("SendDatagram") ;
            else
                fprintf(stderr,"Envoi de %d bytes\n",rc ) ;
        }
     
        tm = sizeof(struct RequeteMS) ;
        memset(&RequeteReponse,0,sizeof(struct RequeteMS)) ;
     
        rc = ReceiveDatagram( Desc, &RequeteReponse,tm, &sor ) ;
        sigprocmask(SIG_SETMASK,&mask, &ancienMask);
        if ( rc == -1 && errno != EINTR)
            die("ReceiveDatagram") ;

        else if (errno == EINTR)
        {
            perror("Interruption Systeme après SIGALRM pas de message\n");
        }
        else
        {
            fprintf(stderr,"bytes recus:%d\n",rc) ;
            if (RequeteReponse.Numero != compteur)
            {
                printf("Doublon %d\n", RequeteReponse.Numero);
            }           
        }
        sigprocmask(SIG_SETMASK,&ancienMask, NULL);
        errno = 0;
    }
    while(RequeteReponse.Numero != compteur);

    alarm(0);
    compteur ++;

    if (RequeteReponse.Type == Reponse)
    {

        switch(RequeteReponse.TypeReponse)
        {
            case Recherche:
                    fprintf(stdout, "Constructeur : %s \t Modele: %s \t Carrosserie: %s \t Quantité : %d\n", RequeteReponse.Constructeur, RequeteReponse.Modele, RequeteReponse.Carrosserie, RequeteReponse.Quantite);
                    viderStdin();
                    break;

            case Achat:

                    if (RequeteReponse.NumeroFacture > 0)
                    {
                        printf("Achat effectué, Facture %d\n", RequeteReponse.NumeroFacture);
                    }
                    else
                    {
                        printf("Achat Echoué\n");
                    }
                    break;

            default:
                    printf("Erreur Type Reponse\n");

        }
    }
        AfficheRequeteFJ(fichierPTR, UneRequete);
        sleep(5);
}
while(c!=3);
 close(Desc) ;
 close(fdDup) ;
 fclose(fichierPTR);
 return 0;
}
void handlerSIGALRM(int signal)
{
    printf("\nRéception du signal %d SIGALRM\n", signal);
}


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

 

while(1)
 {
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
   UneRequete.Type = Question ; 
   //strncpy(UneRequete.Message , "Avec une structure: Bonjour" , sizeof(UneRequete.Message)) ;
   int ref;
         printf("Quelle est la Référence du modèle recherché?\n");
         scanf("%d",&ref);
         UneRequete.Reference=ref;
   rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteFJ) ,&sos ) ;

   if ( rc == -1 )
      die("SendDatagram") ;
   else
      fprintf(stderr,"Envoi de %d bytes\n",rc ) ;
 
   memset(&UneRequete,0,sizeof(struct RequeteFJ)) ;
   tm = sizeof(struct RequeteFJ) ;
 
   rc = ReceiveDatagram( Desc, &UneRequete,tm, &sor ) ;
   if ( rc == -1 )
      die("ReceiveDatagram") ;
   else

   fprintf(stderr,"ref:%d modele:%s quantite:%d puissance:%d constructeur:%s\n",UneRequete.Reference,UneRequete.Modele,UneRequete.Quantite,UneRequete.puissance,UneRequete.Constructeur) ;
   
   break ;
   case 2:
   {
      UneRequete.Type = Achat ;
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
      rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteFJ) ,&sos ) ;
       if ( rc == -1 )
          die("SendDatagram") ;
       else
          fprintf(stderr,"Envoi de %d bytes\n",rc ) ;

      memset(&UneRequete,0,sizeof(struct RequeteFJ)) ;
      tm = sizeof(struct RequeteFJ) ;

   rc = ReceiveDatagram( Desc, &UneRequete,tm, &sor ) ;
   if ( rc == -1 )
      die("ReceiveDatagram") ;
   else
   fprintf(stderr,"achat num facture : %d\n",UneRequete.NumeroFacture);

   }

   break;
   case 3: exit(0) ; 
      close(Desc) ;
  }
}
}

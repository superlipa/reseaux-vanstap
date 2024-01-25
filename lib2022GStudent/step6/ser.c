/*--------------------------------------
  Herman Vanstapel
  
  ex02\ser.c 
  
  Un serveur recevant une structure 
----------------------------------------*/

#include <stdio.h>
#include <string.h>
#include "../udplib/udplib.h"
#include "requeteFJ.h"
#include "LibSerFJ.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>
#include <signal.h>


void die(char *s)
{
    perror(s);
    exit(1);
}

void handlerSIGINT(int signal);
void handlerSIGSTP(int signal);

 int Desc ;

int main(int argc,char *argv[])
{
 int rc ;
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
  //armement signal
  struct sigaction actionSignal, actionSignal1;
  sigfillset(&actionSignal.sa_mask);
  actionSignal.sa_flags = 0;
  actionSignal.sa_handler = handlerSIGINT;

  sigfillset(&actionSignal1.sa_mask);
  actionSignal1.sa_flags = SA_RESTART;
  actionSignal1.sa_handler = handlerSIGSTP;

  if ((sigaction(SIGINT, &actionSignal, NULL) == -1))
  {
    perror("Erreur armement signal\n");
    exit(1);
  }

  if ((sigaction(SIGTSTP, &actionSignal1, NULL) == -1))
  {
    perror("Erreur armement signal\n");
    exit(1);
  }
  
 u_long  IpSocket ;
 u_short PortSocket ;
 
 int tm ;
 struct RequeteFJ UneRequete ;

 memset(&sthis,0,sizeof(struct sockaddr_in)) ;
 memset(&sos,0,sizeof(struct sockaddr_in)) ; 
 memset(&sor,0,sizeof(struct sockaddr_in)) ; 
 
 printf("Ceci est le serveur\n") ;
 if ( argc!=3)
 {
  printf("ser ser port cli\n") ;
  exit(1) ;
 }
 
 /* Récupération IP & port   */
 IpSocket= inet_addr(argv[1]);
 PortSocket = atoi(argv[2]);
 // Desc = CreateSockets(&psoo,&psos,,atoi(argv[2]),argv[3],atoi(argv[4])) ;
 Desc=creer_socket(SOCK_DGRAM,&IpSocket,PortSocket,&sthis);
 
 if ( Desc == -1 )
    die("CreateSockets:") ;
 else
    fprintf(stderr,"CreateSockets %d\n",Desc) ;

struct VehiculeFJ UnRecord;

do
{
    tm = sizeof(struct RequeteFJ) ;
    rc = ReceiveDatagram( Desc,&UneRequete ,tm, &sor ) ;
    if ( rc == -1 && errno != EINTR)
        die("ReceiveDatagram") ;
    else
        fprintf(stderr,"bytes recus:%d:\n",rc) ;
        printf("recu de ip:%s port:%d\n",inet_ntoa(sor.sin_addr),ntohs(sor.sin_port));
        printf("Type recu %d\n", UneRequete.Type) ;
        UneRequete.Type = Reponse;
 switch(UneRequete.TypeReponse)
 {
    case Recherche:
        if(RechercheFJ("VehiculesFJ",UneRequete.Reference,&UnRecord)>0)
            {

            UneRequete.Quantite = UnRecord.Quantite; 
            UneRequete.puissance = UnRecord.puissance;
            strcpy (UneRequete.Constructeur, UnRecord.Constructeur) ;
            strcpy(UneRequete.Modele, UnRecord.Modele) ;
            printf("Vehicule trouvé %d,%d,%s,%s\n",UnRecord.Quantite,UnRecord.puissance,UnRecord.Constructeur,UnRecord.Modele);
            }
            else
            {
                printf("Vehicule %d introuvable\n",UneRequete.Reference);
            }
            UneRequete.Type = Reponse ; 
            rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteFJ) ,&sor ) ;
            if ( rc == -1 )
            die("SendDatagram:") ;
            else
            fprintf(stderr,"bytes envoyes:%d\n",rc ) ;
            break;
    case Achat:
    {
        int numfact=0;
      if(testdoubloserv(FACTURE, &UneRequete.Date, UneRequete.NomClient, &UneRequete.NumeroFacture) == 1)
          {
            printf("Doublon Présent pour la facture\n");
          }
      else
      {
        if (reservationFJ("VehiculesFJ",UneRequete.Reference,UneRequete.Quantite)==1)
        {
          if(numfact=(facturationFJ("FactureFJ",UneRequete.NomClient,dateFacturation,UneRequete.Quantite,UneRequete.Reference))>0)
          {
            printf("maj facturation reussie\n");
          }
         }
            printf("fact:%d\n",numfact);
            UneRequete.NumeroFacture = numfact;
            UneRequete.Type = Reponse ; 
            rc = SendDatagram(Desc,&UneRequete,sizeof(struct RequeteFJ) ,&sor ) ;
            if ( rc == -1 )
            die("SendDatagram:") ;
            else
            fprintf(stderr,"bytes envoyes:%d\n",rc ) ;
        }
    }
    break;
   
    default:
    break;
 }
}
while(1);
 /* attention l'enum peut être codé en short */
 /* reponse avec psos */
 close(Desc) ;
}
void handlerSIGINT(int signal)
{
  printf("\nReception du signal %d SIGINT\nFermeture propre\n", signal);
  close(Desc) ;
  exit(0);
}

void handlerSIGSTP(int signal)
{
  printf("\nReception du signal %d SIGSTP\n DODO \n", signal);
  printf("Demarrage du sleep \n") ;
  sleep(30) ;
  printf("Fin du sleep \n") ;
}


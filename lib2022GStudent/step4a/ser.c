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


void die(char *s)
{
    perror(s);
    exit(1);
}

int main(int argc,char *argv[])
{
 int rc ;
 int Desc ;
 struct sockaddr_in sthis ; /* this ce programme */
 struct sockaddr_in sos ; /* s = serveur */
 struct sockaddr_in sor ; /* r = remote */
 
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

 while(1)
{
    tm = sizeof(struct RequeteFJ) ;
    rc = ReceiveDatagram( Desc,&UneRequete ,tm, &sor ) ;
    if ( rc == -1 )
        die("ReceiveDatagram") ;
    else
        fprintf(stderr,"bytes recus:%d:\n",rc) ;
        printf("recu de ip:%s port:%d\n",inet_ntoa(sor.sin_addr),ntohs(sor.sin_port));
        printf("Type recu %d\n", UneRequete.Type) ;
 switch(UneRequete.Type)
 {
    case Question:
        if(RechercheFJ("VehiculesFJ",UneRequete.Reference,&UnRecord)==1)
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
    break;
    case Livraison:
    break;
    case OK:
    break;
    case Fail:
    case Reponse:
    break;
    default:
    break;
 }
}
 /* attention l'enum peut être codé en short */
 /* reponse avec psos */
 close(Desc) ;
}

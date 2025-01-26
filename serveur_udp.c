// CODE DU SERVEUR UDP

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

// pour conversion d'adresses (pour afficher plus de details) (pas demandé dans le tp)
#include <arpa/inet.h>

// pour des couleurs dans le terminal : 
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"


// port local du serveur : 
#define PORT 9600

int main (int argc, char *argv[]) {
    int sockfd;  // socket()
    struct sockaddr_in adds; // adresse du serveur
    struct sockaddr_in addc; // adresse du client
    int addrlen; // taille de l'adresse du client

    /*code du serveur : 
    - ouvrir le socket du serveur
    - remplir la structure d'adresse locale du serveur : 
      - la famille d'adresse 
      - l'adresse IP
      - le port 
    - spécifier l'adresse locale du socket du serveur*/

    /* ouverture du socket : (paramètres : domain, type, protocol)
     - PF_INET (Ipv4), 
     - SOCK_DGRAM (type de socket, mode datagramme pour UDP)
     - 0, le système fait la correspondance automatiquement 
     La fonction retourne un descripteur de fichier (une valeur entière)
     Si la créaction échoue, socket renvoie -1 */

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { // création échouée
      fprintf(stderr, RED "Ouverture du socket à échouer." RESET "\n");
      return 1; // 1 : erreur, 0 : succès
    }
  
    // remplir la structure locale du serveur : 
    adds.sin_family = AF_INET; // pour IPv4
    adds.sin_port = htons(PORT); // le port (converti en ordre réseau)
    adds.sin_addr.s_addr = htonl(INADDR_ANY); // sur toutes les interfaces réseau (INADDR_ANY)
    
    // bind : fonction qui associe un socket à une adresse locale et un port
    int b = bind(sockfd, (struct sockaddr *)&adds, sizeof(adds));
    if (b < 0) {
      fprintf(stderr, RED "Fonction bind a échouer" RESET "\n");
      close(sockfd); // fermeture du socket ! (bonne pratique)
      return 1;
    }

    addrlen = sizeof(addc); // taille en octets
  
    printf("Bienvenue sur le serveur UDP ! :D \n");
    printf("Serveur établi avec succès. En attente de messages. \n");
  
    // boucle générale du serveur (infinie) : 

    int rec; // données reçues
    char buf[1024]; // buffer pour stocker les données (une chaine de charactères)
    struct sockaddr_in from; // adresse source des données client 
    socklen_t fromlen = sizeof(from); // taille de l'adresse; 

    // (variable ajoutée par moi même) compteur des messages
    int cpt = 0;
    while (1) {

      // recfrom()
      /* paramètres : 
      - sockfd (int) le descripteur de socket 
      - buf (*char) pointeur vers les données 
      - len (int) taille des données en octet 
      - flags (ici 0) options de transmission
      - from (*struct sockaddr_in) et fromlen (*int)
        recevoir l'adresse source des données reçues*/

      rec = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&from, &fromlen);
      // pas besoin de return, le client peut à nouveau envoyer un message
      if (rec < 0) printf(RED "Erreur lors de la réception d'un message..." RESET "\n");
      cpt++; 
      // affichage du message reçu par le client 
      printf("Message #%d reçu de %s:%d : %s", 
              cpt, inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
      // + details du message reçu? ou éventuellement demander l'utilisateur quels details à afficher 
    }
    // succès !
    return 0;
}
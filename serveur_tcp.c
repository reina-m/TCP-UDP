// CODE DU SERVEUR 

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>

// pour conversion d'adresses (pour afficher plus de details)
#include <arpa/inet.h>

// pour des couleurs dans le terminal : 
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

#define SIZE 1024 // taille du buffer 
#define PORT 9600 // port local du serveur
#define BACKLOG 5 // taille de la file d'attente des tentatives de connexions


int main() {
    // newsockfd : socket spécifique pour le client après acceptation de la connexion
    int sockfd, newsockfd; // descipteur de socket
    struct sockaddr_in adds, addc; // adresses du serveur et du client
    /*socklen_t : type standardisé pour les tailles d'adresses (cf. bibliothèque socket.h)*/
    socklen_t addrlen; // taille de l'adresse du client
    char buf[SIZE]; // buffer pour recevoir les messages
    int rec; // taille des données reçues

    // mêmes étapes de UDP en premier
    // ETAPE 1 : création du socket :

    // SOCK_STREAM : contrairement à UDP, communication orientée flux 
    // (reste des variables : comme udp)
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, RED "Ouverture du socket à échouer." RESET "\n");
        return 1;
    }

    // remplir la structure locale du serveur
    memset(&adds, 0, sizeof(adds)); // initialiser à zéro (pour éviter certains comportements)
    adds.sin_family = AF_INET; // pour IPv4
    adds.sin_port = htons(PORT); // le port (converti en ordre réseau)
    adds.sin_addr.s_addr = htonl(INADDR_ANY); // sur toutes les interfaces réseau (INADDR_ANY)

    // ETAPE 2 : BIND
    int b = bind(sockfd, (struct sockaddr *)&adds, sizeof(adds));
    if (b < 0) {
        fprintf(stderr, RED "Fonction bind a échouer" RESET "\n");
        close(sockfd);
        return 1;
    }

    // ETAPE 3 : LISTEN

    /*listen : prend en argument
    - le socket
    - backlog : combien de connexions peuvent être mises en attente avant accept()
    renvoi 0 si succès, -1 si échec */
    int l = listen(sockfd, BACKLOG);
    if (l < 0) {
        fprintf(stderr, RED "Erreur lors de listen" RESET ".\n");
        close(sockfd);
        return 1;
    }

    addrlen = sizeof(addc); // taille en octets

    printf("Bienvenue sur le serveur TCP ! :D \n");
    printf("Serveur établi avec succès. En attente de messages. \n");

    // boucle générale
    while (1) {
        // ETAPE 4 : ACCEPT

        /*accept : sert à accépter une connexio entrentre 
        paramètres :
        - le socket 
        - l'adresse distante de la connexion + sa taille */
    
        // variable initialisée avant l'appel pour éviter les soucis de portabilité
        newsockfd = accept(sockfd, (struct sockaddr *)&addc, &addrlen);
        if (newsockfd < 0) {
            fprintf(stderr, RED "Erreur lors de accept" RESET ".\n");
            continue; // on pourra réessayer faire une autre connexion
        } else {
            // bibliothèque arpa/inet ajoutée (uniquement pour voir les détails du client avec qui la connexion a été établie)
            char ip[INET_ADDRSTRLEN]; // type de l'adresse dans le code
            inet_ntop(AF_INET, &addc.sin_addr, ip, INET_ADDRSTRLEN); // mettre l'adresse correspondante dans la variable
            int port = ntohs(addc.sin_port); // convertir le port
            printf(GREEN "Nouvelle connexion établie avec le client %s:%d." RESET "\n", ip, port);
        }

        // ETAPE 5 : READ
        /*read : lire les données envoyé par le client
        paramètres :
        - le socket (spécifiquement après acceptation de la connection !)
        - buf (pointeur vers le buffeur ou les données reçues seront stockées, comme udp)
        - len la taille maximum des données à lire
        renvoie -1 si échec, le nombre d'octets lus si succcès */
    
        while (1) {
            rec = read(newsockfd, buf, sizeof(buf));
            if (rec < 0) {
                fprintf(stderr, RED "Erreur lors de la réception." RESET "\n");
                break; // quitter la boucle en cas d'erreur
            } else if (rec == 0) {
                printf(YELLOW "Le client a fermé la connexion." RESET "\n");
                break; // quitter la boucle si le client a fermé la connexion
            }
            printf("Message reçu : %s", buf);
        }
        // fermer la connexion avec le client
        close(newsockfd);
    }

    close(sockfd); // fermer la socket principale
    return 0;
}

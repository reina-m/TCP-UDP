// CODE DU CLIENT

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

// pour des couleurs dans le terminal : 
#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"

#define SIZE 1024 // taille du buffer (taille standard courante)
#define PORT 9600 // port du serveur? 
#define h_addr h_addr_list[0] // pour compatibilité 

/*ETAPES DU SCHÉMA : 
- socket () -> bind () -> read() -> sento() -> close()*/

int main (int argc, char *argv[]) {
    /* variables du client : 
    - sockfd le descripteur de socket 
    - structure d'adresse du serveur 
    - pointeur vers la structure descriptive de machine (hostent)
    - zone de mémoire destinée à acceuillir la chaîne entrée du clavier 
    - taille de la chaîne à envoyer */

    int sockfd; // descripteur de socket
    struct sockaddr_in adds; // adresse du serveur
    struct hostent *s; // pointeur vers la structure descriptive de machine
    char buf[SIZE]; // buffer pour stocker les messages
    int len; // taille du message

    // si aucun argument n'est fourni en ligne de code (pour plus tard)
    // le programme nécessite au moins un argument pour le nom du serveur
    if (argc < 2) {
        // + donne la ligne de commande correcte à donner, argv : tableau d'arguments de la liste de commande
        fprintf(stderr, RED "Pas assez d'arguments.\nUtilisation : %s <nom_serveur>" RESET "\n", argv[0]);
        return 1; // 1 : erreur, 0 : succès
    }

    /*code du client : 
    - ouvrir le socket du client 
    - récuperer l'adresse IP du serveur à partir de son nom donné en ligne de commande 
    - remplir la structure d'adresse du serveur 
    - lire une ligne de l'entrée standard 
    - envoyer la chaîne lue au serveur
    */

   // création de la socket :
   // PF_INET pour IPv4 (pf au lieu de af), SOCK_DRAGM pour UDP et 0 pour la correspondance automatique
   sockfd = socket(PF_INET, SOCK_DGRAM, 0);
   if (sockfd < 0) {
        fprintf(stderr, RED "Création du socket a échoué." RESET "\n");
        return 1;
   }

    // récupération de l'adresse IP du serveur : 
    // (avec gethostbyname qui comme son nom l'indique permet d'avoir l'adresse IP associée à un nom de machine)
    s = gethostbyname(argv[1]); // argv : tableau des arguments en ligne de commande
    if (s == NULL) {
        fprintf(stderr, RED "Nom du serveur non reconnu !" RESET "\n");
        close(sockfd); // fermeture du socket
        return 1;
    }

    // remplir la structure d'adresse du serveur (adds):
    adds.sin_family = AF_INET; // pour IPv4
    adds.sin_port = htons(PORT); // le port (converti en ordre réseau)

    /*la struct sin_addr contient un champs s_addr 
    de type uint32_t (entier non signé sur 32 bits) contenant l'adresse du serveur
    memcpy : copie les données de s->h_addr vers adds.sin_addr.s_addr i.e l'adresse*/
    memcpy(&adds.sin_addr.s_addr, s->h_addr, sizeof(adds.sin_addr.s_addr));


    // lecture d'une ligne de l'entrée standard : 
    printf("Tapez un message à envoyer au serveur : ");

    /* fgets (bibliothèque standard de c) 
    char *fgets(char *str, int n, FILE *stream)
    où : *str : pointeur vers le tableau des caractères à lire
    n : la taille maximale des caractères à lire
    FILE *stream : pointeur vers un object file, peut être stdin (standard input)
    renvoie NULL si ça échoue */
    
    if (fgets(buf, SIZE, stdin) != NULL) {
        printf("Vous avez taper : %s", buf);
    } else {
        printf(RED "Erreur lors de la lecture." RESET "\n");
        return 1;
    }

    // envoie du message au serveur : 

    len = strlen(buf);

    /*snedto(int sockfd, char *buf, int len, int flags, struct sockaddr *to, int to len); 
    où sockfd est le descripteur de socket, buf est un pointeur vers les données
    len est la taille des données en octets (qu'on peut récupérer avec strlen),
    flags pour spécifier les options de transmission (ici 0), 
    *to l'adresse à qui envoyer (l'adresse du serveur qu'on a rempli ici), 
    tolen la taille de cette adresse (qu'on peut récupérer avec sizeof) */
    if (sendto(sockfd, buf, len, 0, (struct sockaddr *)&adds, sizeof(adds)) < 0) {
        fprintf(stderr, RED "Sendto a échoué ." RESET "\n");
        close(sockfd);
        return 1;
    }
    // si on arrive à cette ligne, le message a bien été envoyé
    printf(GREEN "Message envoyé au serveur avec succès !" RESET "\n");

    return 0;
}
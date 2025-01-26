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
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

#define SIZE 1024 // taille du buffer
#define PORT 9600 // port du serveur

int main(int argc, char *argv[]) {
    int sockfd; // descripteur de socket
    struct sockaddr_in adds; // adresse du serveur
    struct hostent *s; // pointeur vers la structure descriptive de la machine
    char buf[SIZE]; // buffer pour stocker le message
    int len; // taille du message

    // vérification des arguments (comme udp):
    if (argc < 2) {
        // + donne la ligne de commande correcte à donner, argv : tableau d'arguments de la liste de commande
        fprintf(stderr, RED "Pas assez d'arguments.\nUtilisation : %s <nom_serveur>" RESET "\n", argv[0]);
        return 1;
    }

    // ETAPE 1 : création de la socket 
    // (SOCK_STREAM pour communication orientée flux, i.e. TCP):
    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, RED "Création du socket a échoué." RESET "\n");
        return 1;
    }

    // récupération de l'adresse IP du serveur (comme udp) :
    s = gethostbyname(argv[1]);
    if (s == NULL) {
        fprintf(stderr, RED "Nom du serveur non reconnu !" RESET "\n");
        close(sockfd); // fermeture du socket
        return 1;
    }

    // remplir la structure de l'adresse du serveur (adds) :
    memset(&adds, 0, sizeof(adds)); // pour éviter certain comportements (remplir tout les champs à 0 avec memset)
    adds.sin_family = AF_INET; // pour IPv4
    adds.sin_port = htons(PORT); // convertir en format réseau

    // ETAPE 2 : connexion au serveur : (la différence entre tcp et udp)
    // connect : prend la socket, l'adresse du serveur et sa taille en argument
    // renvoie -1 si échec lors de la connection

    int c = connect(sockfd, (struct sockaddr *)&adds, sizeof(adds));
    if (c < 0) {
        fprintf(stderr, RED "Erreur lors de la connexion au serveur" RESET ".\n");
        close(sockfd);
        return 1;
    } else {
    printf(GREEN "Connexion établie avec le serveur sur le port %d." RESET "\n", PORT);
    }

    // ETAPE 3 : envoyer le message au serveur :
    /* ici, contrairement à udp, j'ai fait en sorte que le client
    peut constamment envoyer des messages et peux faire la décision de fermer 
    la connexion quand il veut. 
    cela m'a permit de vérifier que tcp met bien les messages d'un autre client 
    en attente, jusqu'à ce que la connexion se ferme
    pour implémenter cela, on a besoin d'une boucle*/

    while (1) {
        printf("Tapez un message à envoyer au serveur (ou 'quit' pour fermer la connexion) : ");
        if (fgets(buf, SIZE, stdin) == NULL) {
            printf(RED "Erreur lors de la lecture." RESET "\n");
            break;
        }

        // vérifier si l'utilisateur veut quitter 
        // (strncmp : vérifie si les 4 premiers caractères de buf sont quit)
        if (strncmp(buf, "quit", 4) == 0) {
            printf(YELLOW "Fermeture de la connexion." RESET "\n");
            break; // on sort alors de la boucle
        }

        // sinon, envoyer le message au serveur 
        // (avec write, et non sendto)
        // write est une fonction plus simple qui prend uniquement le descripteur de socket, le message à envoyer et sa taille
        len = strlen(buf);
        if (write(sockfd, buf, len) < 0) {
            fprintf(stderr, RED "Erreur lors de l'envoi du message." RESET "\n");
            break;
        }

        printf(GREEN "Message envoyé au serveur avec succès !" RESET "\n");
    }
    close(sockfd); // fermer la socket
    return 0;
}

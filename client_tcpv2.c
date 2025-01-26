#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

#define SIZE 1024
#define PORT 9600

int main(int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in adds;
    struct hostent *s; 
    char buf[SIZE]; 
    int len;

    if (argc < 2) {
        fprintf(stderr, RED "Pas assez d'arguments.\nUtilisation : %s <nom_serveur>" RESET "\n", argv[0]);
        return 1;
    }

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, RED "Création du socket a échoué." RESET "\n");
        return 1;
    }

    s = gethostbyname(argv[1]);
    if (s == NULL) {
        fprintf(stderr, RED "Nom du serveur non reconnu !" RESET "\n");
        close(sockfd);
        return 1;
    }

    memset(&adds, 0, sizeof(adds));
    adds.sin_family = AF_INET;
    adds.sin_port = htons(PORT);

    int c = connect(sockfd, (struct sockaddr *)&adds, sizeof(adds));
    if (c < 0) {
        fprintf(stderr, RED "Erreur lors de la connexion au serveur" RESET ".\n");
        close(sockfd);
        return 1;
    } else {
    printf(GREEN "Connexion établie avec le serveur sur le port %d." RESET "\n", PORT);
    }

    while (1) {
        printf("Tapez un message à envoyer au serveur (ou 'quit' pour fermer la connexion) : ");
        if (fgets(buf, SIZE, stdin) == NULL) {
            printf(RED "Erreur lors de la lecture." RESET "\n");
            break;
        }

        if (strncmp(buf, "quit", 4) == 0) {
            printf(YELLOW "Fermeture de la connexion." RESET "\n");
            break;
        }

        len = strlen(buf);
        if (write(sockfd, buf, len) < 0) {
            fprintf(stderr, RED "Erreur lors de l'envoi du message." RESET "\n");
            break;
        }

        printf(GREEN "Message envoyé au serveur avec succès !" RESET "\n");
    }
    close(sockfd);
    return 0;
}

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
#define RESET "\033[0m"

#define SIZE 1024
#define PORT 9600
#define h_addr h_addr_list[0]

int main (int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in adds;
    struct hostent *s;
    char buf[SIZE];
    int len;
    if (argc < 2) {
        fprintf(stderr, RED "Pas assez d'arguments.\nUtilisation : %s <nom_serveur>" RESET "\n", argv[0]);
        return 1;
    }

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
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

    adds.sin_family = AF_INET;
    adds.sin_port = htons(PORT);
    memcpy(&adds.sin_addr.s_addr, s->h_addr, sizeof(adds.sin_addr.s_addr));

    printf("Tapez un message à envoyer au serveur : ");

    if (fgets(buf, SIZE, stdin) != NULL) {
        printf("Vous avez taper : %s", buf);
    } else {
        printf(RED "Erreur lors de la lecture." RESET "\n");
        return 1;
    }
    len = strlen(buf);
    if (sendto(sockfd, buf, len, 0, (struct sockaddr *)&adds, sizeof(adds)) < 0) {
        fprintf(stderr, RED "Sendto a échoué ." RESET "\n");
        close(sockfd);
        return 1;
    }
    printf(GREEN "Message envoyé au serveur avec succès !" RESET "\n");

    return 0;
}
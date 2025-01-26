#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define YELLOW "\033[0;33m"
#define RESET "\033[0m"

#define SIZE 1024 
#define PORT 9600
#define BACKLOG 5


int main() {
    int sockfd, newsockfd;
    struct sockaddr_in adds, addc; 
    socklen_t addrlen;
    char buf[SIZE];
    int rec;

    sockfd = socket(PF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        fprintf(stderr, RED "Ouverture du socket à échouer." RESET "\n");
        return 1;
    }

    memset(&adds, 0, sizeof(adds));
    adds.sin_family = AF_INET;
    adds.sin_port = htons(PORT);
    adds.sin_addr.s_addr = htonl(INADDR_ANY);

    int b = bind(sockfd, (struct sockaddr *)&adds, sizeof(adds));
    if (b < 0) {
        fprintf(stderr, RED "Fonction bind a échouer" RESET "\n");
        close(sockfd);
        return 1;
    }

    int l = listen(sockfd, BACKLOG);
    if (l < 0) {
        fprintf(stderr, RED "Erreur lors de listen" RESET ".\n");
        close(sockfd);
        return 1;
    }

    addrlen = sizeof(addc);

    printf("Bienvenue sur le serveur TCP ! :D \n");
    printf("Serveur établi avec succès. En attente de messages. \n");

    while (1) {
        newsockfd = accept(sockfd, (struct sockaddr *)&addc, &addrlen);
        if (newsockfd < 0) {
            fprintf(stderr, RED "Erreur lors de accept" RESET ".\n");
            continue;
        } else {
            char ip[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &addc.sin_addr, ip, INET_ADDRSTRLEN);
            int port = ntohs(addc.sin_port);
            printf(GREEN "Nouvelle connexion établie avec le client %s:%d." RESET "\n", ip, port);
        }
    
        while (1) {
            rec = read(newsockfd, buf, sizeof(buf));
            if (rec < 0) {
                fprintf(stderr, RED "Erreur lors de la réception." RESET "\n");
                break;
            } else if (rec == 0) {
                printf(YELLOW "Le client a fermé la connexion." RESET "\n");
                break;
            }
            printf("Message reçu : %s", buf);
        }
        close(newsockfd);
    }
    close(sockfd);
    return 0;
}

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <arpa/inet.h>

#define RED "\033[0;31m"
#define GREEN "\033[0;32m"
#define RESET "\033[0m"
#define PORT 9600

int main (int argc, char *argv[]) {
    int sockfd;
    struct sockaddr_in adds;
    struct sockaddr_in addc;
    int addrlen;

    sockfd = socket(PF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) {
      fprintf(stderr, RED "Ouverture du socket à échouer." RESET "\n");
      return 1;
    }

    adds.sin_family = AF_INET;
    adds.sin_port = htons(PORT); 
    adds.sin_addr.s_addr = htonl(INADDR_ANY); 

    int b = bind(sockfd, (struct sockaddr *)&adds, sizeof(adds));
    if (b < 0) {
      fprintf(stderr, RED "Fonction bind a échouer" RESET "\n");
      close(sockfd);
      return 1;
    }

    addrlen = sizeof(addc); 
  
    printf("Bienvenue sur le serveur UDP ! :D \n");
    printf("Serveur établi avec succès. En attente de messages. \n");
  
    int rec;
    char buf[1024];
    struct sockaddr_in from;
    socklen_t fromlen = sizeof(from);

    int cpt = 0;
    while (1) {
      rec = recvfrom(sockfd, buf, sizeof(buf), 0, (struct sockaddr*)&from, &fromlen);
      if (rec < 0) printf(RED "Erreur lors de la réception d'un message..." RESET "\n");
      cpt++; 
      printf("Message #%d reçu de %s:%d : %s", 
              cpt, inet_ntoa(from.sin_addr), ntohs(from.sin_port), buf);
    }
    return 0;
}
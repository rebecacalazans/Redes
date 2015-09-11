#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#define MAX 1024

struct message {
  unsigned char type;
  bool last;
  unsigned int len;
};

int main(int argc, char **argv) {
  int sockfd;
  struct sockaddr_in addr;
  unsigned short port;

  if (argc < 2) {
    perror("Não foi encontrado IP de destino");
    return 1;
  }

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd == -1) {
    perror("Erro ao criar socket");
    return 1;
  }

  if (argc > 2) {
    port = (unsigned short) atoi(argv[2]);
  }
  else
    port = 1234;

  addr.sin_family = AF_INET;
  addr.sin_port   = htons(port);
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

  if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr)) != 0)
  {
    printf("Erro ao se conectar!\n");
    return 1;
  }

  printf("Conectado!\n");

  unsigned int packet_size = sizeof(struct message) + MAX;
  char* packet = (char*) malloc(packet_size);

  if (!packet) {
    perror("Falha na alocação da memória");
    return 1;
  }

  struct message* msg =(struct message*) packet;
  char* data =(char*) (packet + sizeof(struct message));
  while(1) {

    memset(packet, 0, packet_size);
    fgets(data, MAX, stdin);

    msg->type = 1;
    msg->last = true;
    msg->len = sizeof(msg) + strlen(data);

    if (send(sockfd, packet, sizeof(struct message) + strlen(data), 0)== -1) {
      printf("Erro ao enviar mensagem");
      return 1;
    };

    memset(packet,0,sizeof(packet));
    int mlen;

    mlen = recv(sockfd, packet, MAX + sizeof(struct message),0);
    if(mlen == -1) {
      perror("Erro ao receber mensagem");
      return 1;
    }

  packet[mlen] = '\0';

  if(msg->len != mlen) {
    printf("Erro ao receber mensagem\n");
  }
  printf("Mensagem: %s\n", data);

  }

  close(sockfd);
  return 0;
}

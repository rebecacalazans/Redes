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
  int sock_client;
  struct sockaddr_in addr;
  unsigned short port;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd == -1) {
    perror("Erro ao criar socket");
    return 1;
  }

  if (argc > 1) {
    port = (unsigned short) atoi(argv[1]);
  }
  else
    port = 1234;

  addr.sin_family = AF_INET;
  addr.sin_port   = htons(port);
  addr.sin_addr.s_addr = INADDR_ANY;

  memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

  if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)) == -1) {
    perror("Erro na funcao bind()\n");
    return 1;
  }

  if(listen(sockfd,1) == -1) {
    printf("Erro na funcao listen()\n");
    return 1;
  }

  printf("Aguardando conexoes...\n");

  sock_client = accept(sockfd,0,0);

  if(sock_client == -1) {
    perror("Erro na funcao accept()\n");
    return 1;
  }

  printf("Pedido de conexao feito!\n");

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
    int mlen;

    mlen = recv(sock_client, packet, MAX + sizeof(struct message),0);
    if(mlen == -1) {
      perror("Erro ao receber mensagem\n");
    }
    packet[mlen] = '\0';

    if(msg->len != mlen) {
      if(send(sock_client, "Mensagem incompleta", 19, 0) == -1);
      printf("Erro ao receber mensagem");
    }
    printf("Mensagem: %s\n", data);

    memset(packet, 0, packet_size);
    fflush(stdin);
    fgets(data, MAX, stdin);

    msg->type = 1;
    msg->last = true;
    msg->len = sizeof(msg) + strlen(data);

    if (send(sock_client, packet, sizeof(struct message) + strlen(data), 0)== -1) {
      printf("Erro ao enviar mensagem");
      return 1;
    };

  }

  close(sock_client);
  close(sockfd);
  return 0;
}

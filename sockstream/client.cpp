#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {

  if (argc < 2) {
    perror("Não foi encontrado IP de destino");
    return 1;
  }

  int sockfd;
  struct sockaddr_in addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd == -1) {
    perror("Erro ao criar socket");
    return 1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port   = htons(1234);
  addr.sin_addr.s_addr = inet_addr(argv[1]);

  memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

  if(connect(sockfd,(struct sockaddr*)&addr,sizeof(addr)) != 0)
  {
    printf("Erro ao se conectar!\n");
    return 1;
  }

  printf("Conectado!\n");

  char message[256];
  int mlen;
  mlen = recv(sockfd, message,256,0);
  if(mlen == -1) {
    perror("Erro ao receber mensagem");
    return 1;
  }

  message[mlen] = '\0';
  printf("recebido: %s\n", message);

  if((send(sockfd,"Enviado pelo cliente",20,0)) == -1) {
    perror("Erro ao enviar dados!\n");
    return 1;
  }

  close(sockfd);
  return 0;
}

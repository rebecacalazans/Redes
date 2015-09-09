#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char **argv) {
  int sockfd;
  int sock_client;
  struct sockaddr_in addr;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if(sockfd == -1) {
    perror("Erro ao criar socket");
    return 1;
  }

  addr.sin_family = AF_INET;
  addr.sin_port   = htons(1234);
  addr.sin_addr.s_addr = INADDR_ANY;

  memset(&addr.sin_zero,0,sizeof(addr.sin_zero));

  if(bind(sockfd,(struct sockaddr*)&addr,sizeof(addr)) == -1) {
    printf("Erro na funcao bind()\n");
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

  if((send(sock_client,"Enviado pelo servidor",21,0)) == -1) {
    perror("Erro ao enviar dados!\n");
    return 1;
  }

  char message[256];
  int mlen;
  mlen = recv(sock_client, message,256,0);
  if(mlen == -1) {
    perror("Erro ao receber mensagem");
    return 1;
  }
  message[mlen] = '\0';
  printf("recebido: %s\n", message);

  close(sock_client);
  close(sockfd);
  return 0;
}

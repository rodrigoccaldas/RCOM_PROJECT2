#include "biblio.h"

#define tamanho_maximo 1024
struct TCP tcp;
struct URL url;


int login(char * username, char * password) {
  char username_cmd[tamanho_maximo];
  char password_cmd[tamanho_maximo];
  char buf[tamanho_maximo];

  sprintf(username_cmd, "USER %s\r\n", username);   
  printf("%s\n", username_cmd);
  if (msg_para_servidor(tcp.socket_controlo, username_cmd) < 0) { 
    printf("Erro na escrita do USER\n");
    return -1;
  }

  if (ler_resposta(tcp.socket_controlo, buf)) {
    printf("Erro na leitura do USER\n");
    return -1;
  }

  sprintf(password_cmd, "PASS %s\r\n", password);   
  printf("%s\n", password_cmd);
  if (msg_para_servidor(tcp.socket_controlo, password_cmd) < 0) { 
    printf("Erro na escrita do PASS \n");
    return -1;
  }

  if (ler_resposta(tcp.socket_controlo, buf)) {
    printf("Erro na leitura do PASS\n");
    return -1;
  }

  return 0;
}

int modopassivo() {
  char buffer[tamanho_maximo];
    int ip1, ip2, ip3, ip4;
    int porta1, porta2;


  if (msg_para_servidor(tcp.socket_controlo, "PASV\r\n") < 0) {
    printf("Erro no comando PASV\n");
    return -1;
  }

  if (ler_resposta(tcp.socket_controlo, buffer)) {
    printf("Erro no comando PASV\n");
    return -1;
  }

  if ((sscanf(buffer, "227 Entering Passive Mode (%d,%d,%d,%d,%d,%d)", &ip1, &ip2, &ip3, &ip4, &porta1, &porta2)) < 0) {
    printf("Erro no modo passivo\n");
    return -1;
  }

  sprintf(tcp.ipnovo, "%d.%d.%d.%d", ip1, ip2, ip3, ip4);
  tcp.portanova = porta1 * 256 + porta2; 

  printf("Novo IP: %s\n", tcp.ipnovo);
  printf("Nova Porta: %d\n", tcp.portanova);

  tcp.socket_dados = conexao(tcp.ipnovo, tcp.portanova);

  if (tcp.socket_dados < 0) {
  		printf("\nNão foi possível conectar ao servido\n");
  		return -1;
  }

  return 0;
}

int enviar_RETR(char * path) { 

  char aux[tamanho_maximo];
  char buffer[tamanho_maximo];

  sprintf(aux, "\nRETR %s\r\n", path);

  printf(">%s",aux);

  if(msg_para_servidor(tcp.socket_controlo, aux) < 0) {
    printf("Erro a escrever o comando RETR\n");
    return -1;
  }

  if(ler_resposta(tcp.socket_controlo, buffer) != 0){
    printf("Erro a ler o comando RETR\n");
    return -1;
  }

  return 0;
}

int download(char * ficheiro){
  char buffer[tamanho_maximo];
  FILE* novo_ficheiro;

  novo_ficheiro = fopen(ficheiro, "w"); 
  
  int aux,res;
  while ((res = read(tcp.socket_dados, buffer, tamanho_maximo))) { 
    if (res < 0) {
      printf("Erro na receção(leitura) dos dados através do socket de dados\n");
      return -1;
    }
    aux = fwrite(buffer, res, 1, novo_ficheiro);
    if (aux< 0) {
      printf("Erro na escrita dos dados no novo ficheiro.\n");
      return -1;
    }
  }

	if(ler_resposta(tcp.socket_controlo, buffer)) {
		printf("Erro na leitura da resposta\n");
		return -1;
	}

  fclose(novo_ficheiro);
  printf("Download terminado com sucesso.TOP\n");
  return 0;
}



int ler_resposta(int socket_controlo, char * buffer) { 
  FILE * f = fdopen(socket_controlo, "r");

	do {
		memset(buffer, 0, tamanho_maximo);
		fgets(buffer, tamanho_maximo, f); 

		printf("%s", buffer);
	} while (!('1' <= buffer[0] && buffer[0] <= '5') || buffer[3] != ' ');

  return buffer[0] >= '4';
}

int msg_para_servidor(int socket_controlo, char * msg) {
  int aux;
  aux = write(socket_controlo, msg, strlen(msg));
  return aux;
}


int conexao(char * ip, int porta) {
	struct	sockaddr_in server_addr;
  int fd;


	bzero((char*)&server_addr,sizeof(server_addr));
	server_addr.sin_family = AF_INET; 
	server_addr.sin_addr.s_addr = inet_addr(ip);
	server_addr.sin_port = htons(porta); 


	if ((fd = socket(AF_INET,SOCK_STREAM,0)) < 0) {
    return -1;
  }


  if(connect(fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0){
    printf("Erro na conexão ao servidor");
    return -1;
  }

  return fd;
}

int desligar_sockets() {
  close(tcp.socket_controlo);
  close(tcp.socket_dados);
	printf("\nSockets fechados com sucesso.TOP\n");
	return 0;
}
///////////////////////////////////////////////////MAIN//////////////////////////////////////
int main(int argc, char *argv[]) {

  if (argc != 2)
  {
    printf("\nNúmero de argumentos inválidos.\n");
    return -1;
  }

  if (ler_url(argv[1]) < 0) {
    printf("\nO URL inserido não é válido.\n");
    return -1;
  }

  printf("Utilizador: %s\n", url.username);
  printf("Password: %s\n", url.password);

  printf("Path: %s\n", url.path);
  printf("Host: %s\n", url.host);
  printf("IP: %s\n\n", url.ip);
  
  printf("Nome do Ficheiro: %s\n", url.ficheiro);


  tcp.socket_controlo = conexao(url.ip, 21);  
  if (tcp.socket_controlo < 0) {
    return -1;
  }

  char buffer[tamanho_maximo];

  if(ler_resposta(tcp.socket_controlo, buffer)) {
    printf("Erro na leitura do socket de controlo.\n");
    return -1;
  }


  if(login(url.username, url.password) < 0) {  
    return -1;
  }


  if(modopassivo() < 0) {  
    return -1;
  }


  if(enviar_RETR(url.path) < 0){  
    return -1;
  }


  if(download(url.ficheiro) < 0) {  
    return -1;
  }


  if(desligar_sockets() < 0) { 
    return -1;
  }


  return 0;
}
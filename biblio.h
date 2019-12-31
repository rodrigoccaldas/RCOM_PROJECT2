#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <signal.h>
#include <unistd.h>
#include <errno.h>
#include <netdb.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <fcntl.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


#define tamanho_maximo 1024

struct TCP {
    int socket_controlo; 
    int socket_dados; 
    char ipnovo[tamanho_maximo];
    int portanova; 
};

struct URL 
                                   
{
    char username[tamanho_maximo];
    char password[tamanho_maximo];
    char host[tamanho_maximo];
    char ip[tamanho_maximo];
    char path[tamanho_maximo];
    char ficheiro[tamanho_maximo];
    int porta; 
};


int conexao(char * ip, int porta); 
int login(char * username, char * password);
int ler_url(const char *str);
int modopassivo(); 
int nomedoficheiro(const char * path); 
int enviar_RETR(char * path);
int ip_host(const char *host);
int download(char * ficheiro); 
int ler_resposta(int socket_controlo, char * buffer);
int msg_para_servidor(int socket_controlo, char * msg); 
int desligar_sockets();





















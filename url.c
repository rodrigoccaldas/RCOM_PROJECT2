#include "biblio.h"
	struct URL url;

int ip_host(const char * host) {

	struct hostent *h;

	if ((h = gethostbyname(host)) == NULL)
	{
		printf("erro a obter ip host com a funçao gethostbyname()");
		return -1;
	}

	char *ip = inet_ntoa(*((struct in_addr *)h->h_addr));
	strcpy(url.ip, ip);

	return 0;
}

int nomedoficheiro(const char * path) { 
	int i;
	int j=0;
	char ficheiro[256];
	size_t str_max = strlen(path);

	for(i=0; i<str_max; i++) {

		if(path[i] == '/') {
			memset(ficheiro, 0, 20);
			j=0;
			continue;
		}

		if(i==str_max-1) { 
			ficheiro[j] = path[i];
			strcpy(url.ficheiro, ficheiro);
			break;
		}

		ficheiro[j] = path[i];
		j++;
	}

	return 0;
}

int ler_url(const char *str) {
	size_t str_max = strlen(str);
	char str_aux[256];
	int i = 6; 
	int j = 0;

	strncpy(str_aux, str, i); 

	memset(str_aux, 0, 20); 

	for(i = 6; i<str_max; i++) {
	
		if(str[i] == ':') {
			strcpy(url.username, str_aux);
			memset(str_aux, 0, 20);
			j = 0;
			continue;
		}
		if(str[i] == '@') {
			strcpy(url.password, str_aux);
			memset(str_aux, 0, 20);
			j = 0;
			continue;
		}
		if(str[i] == '/'){
			if(strcmp(url.host, "") == 0) { 
				strcpy(url.host, str_aux);
				memset(str_aux, 0, 20);
				j = 0;
				continue;
			}
		}
		if(i == str_max - 1) { 
			str_aux[j] = str[i];
			strcpy(url.path, str_aux);
			break;
		}

		str_aux[j] = str[i];
		j++;
	}

	nomedoficheiro(url.path);

	if (ip_host(url.host) < 0) {
		printf("Não encontra ip do host %s.\n", url.host);
		return -1;
	}

	return 0;
}




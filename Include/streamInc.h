#define CHECK(sts,msg) if ((sts) == -1) {perror(msg);exit(-1);}
#include <unistd.h> 
#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include <sys/types.h> 
#include <sys/socket.h> 
#include <netinet/in.h> 
#include <arpa/inet.h> 
#define PORT_SVC 5000
#define INADDR_SVC "127.0.0.1" 
#define MSG "100:Je dis que \"le fond de l’eau est clair par ici ! Où ça ?\"" 
#define BYE "000:Au revoir et à bientôt ..." 
#define ERR "200:Requête ou réponse non reconnue !" 
#define OK "OK" 
#define NOK "Not OK" 
#define MAX_BUFF 1024 
char buffer[MAX_BUFF];
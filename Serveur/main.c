#include "../Include/streamInc.h"
#include "../Include/protocols.h"

#include <time.h>


char *get_ip_str(const struct sockaddr *sa, char *s, size_t maxlen) {
    switch (sa->sa_family) {
        case AF_INET:
            inet_ntop(AF_INET, &(((struct sockaddr_in *) sa)->sin_addr), s, maxlen);
            break;

        case AF_INET6:
            inet_ntop(AF_INET6, &(((struct sockaddr_in6 *) sa)->sin6_addr), s, maxlen);
            break;

        default:
            strncpy(s, "Unknown AF", maxlen);
            return NULL;
    }

    return s;
}


typedef struct client {
    unsigned int id;
    char pseudo[20];
    time_t heureConnexion;
} client_t;

typedef struct annuaire {
    client_t clients[10];
    int nbclients;
} annuaire_t;

annuaire_t annuaire;

void dialogueClt(int sd, struct sockaddr_in clt) {
    char *requete;
    char *reste;
    do {
        read(sd, buffer, sizeof(buffer));
        requete = strtok(buffer, ":");
        reste = strtok(NULL, ":");

        switch (atoi(requete)) {
            case DMD_DECONNEXION :
                printf("Au revoir\n");
                break;
            case CONN_CLIENT_MASTER: {
                write(sd, OK, strlen(OK) + 1);
                printf("OK : message recu %s\n", buffer);
                annuaire.nbclients++;
                client_t client;
                time_t rawtime;
                struct tm *timeinfo;
                time(&rawtime);
                timeinfo = localtime(&rawtime);
                client.id = annuaire.nbclients;
                strncpy(client.pseudo, reste, 20);
                client.heureConnexion = rawtime;
                char *timeAsString = asctime(timeinfo);
                timeAsString[strlen(timeAsString) - 1] = '\0'; // suppr le \n de la fin de la fonction
                char *buf = inet_ntoa(clt.sin_addr);

                printf("Nouveau Client : [%s] connecté à [%s], d'id :[%d], ip = [%s]\n",
                       client.pseudo,
                       timeAsString,
                       client.id,
                       buf);
                annuaire.clients[annuaire.nbclients++] = client;
                fflush(stdout);
            }
                break;

            default:
                write(sd, NOK, strlen(NOK) + 1);
                printf("NOK : message recu %s\n", buffer);
                break;
        }
    } while (atoi(requete) != DMD_DECONNEXION);
}


int main() {
    annuaire.nbclients = 0;
    int se, sd;
    struct sockaddr_in svc, clt;
    socklen_t cltLen;
// Création de la socket de réception d’écoute des appels
    CHECK(se = socket(PF_INET, SOCK_STREAM, 0), "Can't create");
// Préparation de l’adressage du service (d’appel)
    svc.sin_family = PF_INET;
    svc.sin_port = htons(PORT_SVC);
    svc.sin_addr.s_addr = INADDR_ANY;
    memset(&svc.sin_zero, 0, 8);
// Association de l’adressage préparé avec la socket d’écoute
    CHECK(bind(se, (struct sockaddr *) &svc, sizeof svc), "Can't bind");
// Mise en écoute de la socket
    CHECK(listen(se, 5), "Can't calibrate");
// Boucle permanente de service
    while (1) {
        //  Attente d’un appel
        cltLen = sizeof(clt);
        CHECK(sd = accept(se, (struct sockaddr *) &clt, &cltLen), "Can't connect");
        // Dialogue avec le client
        dialogueClt(sd, clt);
        close(sd);
    }
    printf("Fini !!!");
    close(se);
    return 0;
}
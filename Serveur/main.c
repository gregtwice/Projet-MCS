
#include "../Include/streamInc.h"
#include "../Include/protocols.h"
#include "../Include/utils.h"
#include "serveur.h"


typedef struct {
    int sd;
    struct sockaddr_in clt;
} dialogueArg_t;

void *dialogue(void *args) {
    dialogueArg_t *thread_args = args;
    int sd = thread_args->sd;
    struct sockaddr_in clt = thread_args->clt;

    char *requete;
    char *reste;
    do {
        read(sd, buffer, sizeof(buffer));
        requete = strtok(buffer, ":");
        reste = strtok(NULL, ":");

        switch (atoi(requete)) {
            case DMD_DECONNEXION : {
                pthread_t i = pthread_self();
                client_t client = find_client(i);
                printf("Au revoir %s\n", client.pseudo);
                remove_client(i);
                afficher_annuaire();
                break;
            }
            case CONN_CLIENT_MASTER: {
                printf("OK : message recu %s\n", buffer);
                client_t client;
                time_t rawtime;
                // on récupère le temps
                time(&rawtime);

                // on passe en temps local
                struct tm *timeinfo;

                timeinfo = get_local_time();
                char *time_string = time_to_char(timeinfo);
                char *addr = address_to_char(clt.sin_addr);
                client = creer_client(pthread_self(), reste, rawtime);

                printf("Nouveau Client : [%s] connecté à [%s], d'id :[%lu], ip = [%s]\n",
                       client.pseudo,
                       time_string,
                       client.id,
                       addr);
                annuaire_clients.clients[annuaire_clients.nbclients] = client;
                annuaire_clients.nbclients++;
                fflush(stdout);
                write(sd, OK, strlen(OK) + 1);
            }
                break;

            default:
                write(sd, NOK, strlen(NOK) + 1);
                printf("NOK : message recu %s\n", buffer);
                break;
        }
    } while (atoi(requete) != DMD_DECONNEXION);
    close(sd);
    free(thread_args);
    return NULL;
}

/*

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
                printf("OK : message recu %s\n", buffer);
                client_t client;
                time_t rawtime;
                // on récupère le temps
                time(&rawtime);

                // on passe en temps local
                struct tm *timeinfo;

                timeinfo = get_local_time();
                char *time_string = time_to_char(timeinfo);
                char *addr = address_to_char(clt.sin_addr);
                client = creer_client(getpid(), reste, rawtime);

                printf("Nouveau Client : [%s] connecté à [%s], d'id :[%d], ip = [%s]\n",
                       client.pseudo,
                       time_string,
                       client.id,
                       addr);
                annuaire_clients.clients[annuaire_clients.nbclients] = client;
                annuaire_clients.nbclients++;
                fflush(stdout);
                write(sd, OK, strlen(OK) + 1);
            }
                break;

            default:
                write(sd, NOK, strlen(NOK) + 1);
                printf("NOK : message recu %s\n", buffer);
                break;
        }
    } while (atoi(requete) != DMD_DECONNEXION);
    exit(0);
}
*/


int main() {

    int nbthreads = 0;
    annuaire_clients.nbclients = 0;
    int se, sd;
    struct sockaddr_in svc, clt;
    socklen_t cltLen;
// Création de la socket de réception d’écoute des appels
    CHECK(se = socket(PF_INET, SOCK_STREAM, 0), "Can't create");
    int optval = 1;
    setsockopt(se, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));
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

        dialogueArg_t *args = malloc(sizeof *args);
        args->clt = clt;
        args->sd = sd;

//        dialogueClt(sd, clt);
        pthread_t t = ++nbthreads;
        // Dialogue avec le client
        pthread_create(&t, NULL, dialogue, args);
        //close(sd);
    }
    printf("Fini !!!");
    close(se);
    return 0;
}
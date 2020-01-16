
#include "../Include/streamInc.h"
#include "../Include/protocols.h"
#include "serveur.h"


typedef struct {
    int sd;
    struct sockaddr_in clt;
} dialogueArg_t;


dialogueArg_t *creerArgsThread(int sd, struct sockaddr_in *clt);

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
        printf("Requête Reçue ! %s\n", buffer);
        switch (atoi(requete)) {
            case DMD_DECONNEXION :
                deconnexionClient();
                break;
            case CONN_CLIENT_MASTER:
                gererConnexionClient(sd, &clt, reste);
                break;
            case CONN_ENTREPOT_MASTER: {
                char response[4];
                protocol_as_char(SUCC_CONN, response);
                strcat(response, ":");
                write(sd, response, 4);
            }
                break;
            case DMD_PROD_SERV_MASTER: {

            }

            case DMD_CATALOGUE: {
                puts("Demande Catalogue !!");
                FILE *fic = fopen("catalogueGlobal.txt", "r+");
                char c;
                int i = 0;
                if (fic) {
                    while ((c = getc(fic)) != EOF) {
                        buffer[i] = c;
                        i++;
                    }
                    buffer[++i] = '\0';
                    write(sd, buffer, i + 1);
                } else {
                    puts("Erreur Lecture Fichier !!!");
                }
                break;
            }

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
        printf("Connexion Recue !!!\n");

        dialogueArg_t *args = creerArgsThread(sd, &clt);
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


dialogueArg_t *creerArgsThread(int sd, struct sockaddr_in *clt) {
    dialogueArg_t *args = malloc(sizeof *args);
    args->clt = (*clt);
    args->sd = sd;
    return args;
}

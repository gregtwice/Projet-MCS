
#include <signal.h>
#include "../Include/streamInc.h"
#include "../Include/protocols.h"
#include "serveur.h"


typedef struct {
    int sd;
    struct sockaddr_in clt;
} dialogueArg_t;

typedef struct {
    client_t client;
    int numprod;
    int qte;
    int ack;
} commande_t;

typedef struct {
    commande_t commandes[10];
    int nbCommandes;
} carnet_t;
carnet_t carnet;

pthread_mutex_t mutex;

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
                serveur_t serveur;
                serveur.sd = sd;
                serveur.id = pthread_self();
                liste_serveurs.serveurs[liste_serveurs.nbServeurs++] = serveur;
                write(sd, response, 4);
            }
                break;
            case DMD_PROD_SERV_MASTER: {
                char *split[2];
                printf("Reste : %s", reste);
                parser(split, reste, "-", 2);
                for (int j = 0; j < 2; ++j) {
                    printf("%s", split[j]);
                }
                int nbProduit = atoi(split[0]);
                int quantite = atoi(split[1]);
                printf("%d\n", nbProduit);
                commande_t commande;
                commande.client = find_client(pthread_self());
                commande.qte = quantite;
                commande.numprod = nbProduit;
                commande.ack = 0;
                carnet.commandes[carnet.nbCommandes] = commande;
                carnet.nbCommandes++;
                for (int i = 0; i < liste_serveurs.nbServeurs; ++i) {
                    printf("Serveur %d\n", i);
                    int sde = liste_serveurs.serveurs[i].sd;
                    char request[50];
                    char prot[4];
                    protocol_as_char(DMD_PROD_ENTREPOT, prot);
                    sprintf(request, "%s%d:%d:%lu", prot, nbProduit, quantite, pthread_self());
                    printf("%s", request);
                    write(sde, request, strlen(request));
                }
                break;
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

            case PROD_DISPO_QTE: {

                pthread_mutex_lock(&mutex);
                printf("MUTEX PRISE !!!\n");
                printf("%s\n", reste);
                char *ptr;
                for (int i = 0; i < carnet.nbCommandes; ++i) {
                    commande_t curCommmande = carnet.commandes[i];
                    if (curCommmande.client.id == strtoul(reste, &ptr, 10)) {
                        if (curCommmande.ack == 0) {
                            char prot[4];
                            printf("Commande pour %s\n", curCommmande.client.pseudo);
                            protocol_as_char(CMD_PROD_ENTREPOT, prot);
                            char request[100];
                            curCommmande.ack = 1;
                            sprintf(request, "%s%d%d", prot, curCommmande.numprod, curCommmande.qte);
                            write(sd, request, strlen(request));
                        }
                    }
                }
                pthread_mutex_unlock(&mutex);
                printf("MUTEX RELACHÉE !!!\n");
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

void printstate() {
    afficher_annuaire();
    for (int i = 0; i < liste_serveurs.nbServeurs; ++i) {
        printf("%d", liste_serveurs.serveurs[i].sd);
    }
}


int main() {
    int nbthreads = 0;
    carnet.nbCommandes = 0;
    annuaire_clients.nbclients = 0;
    liste_serveurs.nbServeurs = 0;
    int se, sd;
    struct sockaddr_in svc, clt;
    socklen_t cltLen;
    signal(SIGUSR1, printstate);
    pthread_mutex_init(&mutex, NULL);
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

//
// Created by gdefoy on 14/01/2020.
//

#include "../Include/utils.h"
#include "../Include/streamInc.h"
#include "serveur.h"


client_t creer_client(unsigned long id, char *pseudo, time_t heure_conn, int sd) {
    client_t result;
    result.id = id;
    result.sd = sd;
    strncpy(result.pseudo, pseudo, MAX_PSEUDO_CLIENT);
    result.heureConnexion = heure_conn;
    return result;
}

client_t find_client(unsigned long id) {
    client_t result;
    result.id = -1;
    for (int i = 0; i < annuaire_clients.nbclients; ++i) {
        if (annuaire_clients.clients[i].id == id) {
            return annuaire_clients.clients[i];
        }
    }
    return result;
}

void remove_client(unsigned long id) {
    for (int i = 0; i < annuaire_clients.nbclients; ++i) {
        if (annuaire_clients.clients[i].id == id) {
            for (int j = i; j < annuaire_clients.nbclients - 1; ++j) {
                annuaire_clients.clients[j] = annuaire_clients.clients[j + 1];
            }
        }
    }
    annuaire_clients.nbclients--;
}

void afficher_annuaire() {
    printf(" \n\nAffichage de l'annuaire avec %d clients ! \n", annuaire_clients.nbclients);
    for (int i = 0; i < annuaire_clients.nbclients; ++i) {
        client_t clt = annuaire_clients.clients[i];
        printf("Client [%s] | id [%lu]\n", clt.pseudo, clt.id);
    }
    printf("Terminé \n");
    fflush(stdout);
}

void gererConnexionClient(int sd, struct sockaddr_in *clt, char *reste) {
    printf("OK : message recu %s\n", buffer);
    client_t client;
    time_t rawtime;
    // on récupère le temps
    time(&rawtime);
    // on passe en temps local
    struct tm *timeinfo;
    timeinfo = get_local_time();
    char *time_string = time_to_char(timeinfo);
    char *addr = address_to_char((*clt).sin_addr);
    client = creer_client(pthread_self(), reste, rawtime, sd);

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

void deconnexionClient() {
    pthread_t i = pthread_self();
    client_t client = find_client(i);
    if (client.id != -1) {
        printf("Au revoir %s\n", client.pseudo);
        remove_client(i);
        afficher_annuaire();
    }
}
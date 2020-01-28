//
// Created by gdefoy on 14/01/2020.
//

#ifndef PROJET_SERVEUR_H
#define PROJET_SERVEUR_H

#define MAX_PSEUDO_CLIENT 20

#include "../Include/utils.h"
#include <pthread.h>

typedef struct client {
    unsigned long id;
    char pseudo[20];
    time_t heureConnexion;
    int sd;
} client_t;

typedef struct annuaire {
    client_t clients[10];
    int nbclients;
} annuaire_t;


typedef struct serveur {
    int sd;
    char addresse[30];
    unsigned long id;
} serveur_t;

typedef struct {
    serveur_t serveurs[10];
    int nbServeurs;
} serveurList_t;


annuaire_t annuaire_clients;
serveurList_t liste_serveurs;

client_t creer_client(unsigned long id, char *pseudo, time_t heure_conn, int sd);

client_t find_client(unsigned long);

void remove_client(unsigned long id);

void afficher_annuaire();

void gererConnexionClient(int sd, struct sockaddr_in *clt, char *reste);

void deconnexionClient();

#endif //PROJET_SERVEUR_H

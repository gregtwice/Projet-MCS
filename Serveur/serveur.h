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
} client_t;

typedef struct annuaire {
    client_t clients[10];
    int nbclients;
} annuaire_t;

annuaire_t annuaire_clients;

client_t creer_client(unsigned long id, char *pseudo, time_t heure_conn);

client_t find_client(unsigned long);

void remove_client(unsigned long id);

void afficher_annuaire();

#endif //PROJET_SERVEUR_H

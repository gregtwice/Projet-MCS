//
// Created by gdefoy on 14/01/2020.
//

#include "serveur.h"

client_t creer_client(unsigned long id, char *pseudo, time_t heure_conn) {
    client_t result;
    result.id = id;
    strncpy(result.pseudo, pseudo, MAX_PSEUDO_CLIENT);
    result.heureConnexion = heure_conn;
    return result;
}

client_t find_client(unsigned long id) {
    client_t result;
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

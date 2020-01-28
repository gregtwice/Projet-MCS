//
// Created by gdefoy on 14/01/2020.
//
/**
 * \file Serveur/serveur.h
 */

#ifndef PROJET_SERVEUR_H
#define PROJET_SERVEUR_H

#define MAX_PSEUDO_CLIENT 20

#include "../Include/utils.h"
#include <pthread.h>

/**
 * @struct client_t serveur.h
 * @brief représente un client
 */
typedef struct client {
    unsigned long id;
    char pseudo[20];
    time_t heureConnexion;
    int sd;
} client_t;

/**
 * @struct annuaire_t serveur.h
 * @brief liste des clients
 */
typedef struct annuaire {
    client_t clients[10];
    int nbclients;
} annuaire_t;


/**
 * @struct serveur_t serveur.h
 * @brief représente un entrepot
 */
typedef struct serveur {
    int sd;
    char addresse[30];
    unsigned long id;
} serveur_t;

/**
 * @struct serveurList_t serveur.h
 * @brief liste des entrepot
 */
typedef struct {
    serveur_t serveurs[10];
    int nbServeurs;
} serveurList_t;


annuaire_t annuaire_clients;
serveurList_t liste_serveurs;
/**
 * @brief crée un client avec les données passées en paramètre
 * @param id l'id du client
 * @param pseudo le pseudo du client
 * @param heure_conn l'heure de connexion du client
 * @param sd la socket de dialogue pour communiquer avec le client
 * @return une structure client
 */
client_t creer_client(unsigned long id, char *pseudo, time_t heure_conn, int sd);

/**
 * @brief trouve le client via son id dans l'annuaire
 * @param id l'id a trouver
 * @return le client
 */
client_t find_client(unsigned long id);

/**
 * @brief retire le client correspondant à l'id de la liste des clients
 * @param id l'id du client
 */
void remove_client(unsigned long id);

/**
 * Affiche la liste des clients
 */
void afficher_annuaire();

/**
 * @brief s'occupe de créer un client
 * @param sd la socket de dialogue du client
 * @param clt l'adresse du client
 * @param reste le reste des infos du client
 */
void gererConnexionClient(int sd, struct sockaddr_in *clt, char *reste);

/**
 * @brief gère la deconnexion d'un client
 */
void deconnexionClient();

#endif //PROJET_SERVEUR_H

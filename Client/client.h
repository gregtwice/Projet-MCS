//
// Created by gdefoy on 12/01/2020.
//

#ifndef PROJET_CLIENT_H
#define PROJET_CLIENT_H

#include "../Include/protocols.h"
#include "../Include/streamInc.h"


enum Etat {
    NC, // Non Connecté
    CO, // Connnecté
};


/**
 * Envoie une requête au serveur
 * @param code le code de la requete @see protocols.h
 * @param sock la socket
 * @param msg le message à envoyer au serveur
 */
void sendReq(int code, int sock, char *msg);

#endif //PROJET_CLIENT_H

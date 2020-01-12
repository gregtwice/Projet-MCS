//
// Created by gdefoy on 11/01/2020.
//

#ifndef PROJET_PROTOCOLS_H
#define PROJET_PROTOCOLS_H

// Définition des codes de connexion

// Codes de demande

#define CONN_CLIENT_MASTER 100
#define CONN_ENTREPOT_MASTER 101
#define DMD_DECONNEXION 102

#define DMD_CATALOGUE 110
#define DMD_PROD_SERV_MASTER 111
#define CMD_PROD_SERV_MASTER 112

// Codes de succès

#define SUCC_CONN 200
#define SUCC_ACC_CATALOGUE 201
#define SUCC_ACC_PROD 202
#define SUCC_CMD_PROD 203
#define SUCC_DECONNEXION 204

#define PROD_DISPO_QTE 210
#define PROD_DISPO_NON_QTE 211
#define SUCC_CMD_ENTREPOT 212

// Codes d'erreur Client

#define ERR_NOM_PROD 402
#define ERR_QTE 403

// Codes d'erreur Serveur

#define ERR_PROD_ENTREPOT 503

#endif //PROJET_PROTOCOLS_H


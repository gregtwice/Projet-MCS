//
// Created by gdefoy on 12/01/2020.
//

#include "client.h"



void sendReq(int code, int sock, char *msg) {
    switch (code) {
        case CONN_CLIENT_MASTER: {
            char msgTotal[24] = "\0";
            // On envoie le pseudo de l'utilisateur
            // Msg contient donc le pseudo
            sprintf(msgTotal, "%d:", CONN_CLIENT_MASTER);
            msg[strlen(msg)-1] = '\0';
            strcat(msgTotal, msg);
            printf("msgTotal %s\n", msgTotal);
            CHECK(write(sock, msgTotal, strlen(msgTotal) + 1), "Can't send");
            break;
        }
        case DMD_DECONNEXION: {
            char msgT[5] = "\0";
            sprintf(msgT, "%d:", DMD_DECONNEXION);
            CHECK(write(sock, msgT, 5), "Error Deco");
            break;
        }
        default:
            break;
    }
}
//
// Created by gdefoy on 11/01/2020.
//

#include "client.h"


int afficherMenu(enum Etat e);

void quit(int sock);

/*void dialogueSrv(int sd, struct sockaddr_in *srv) {
    char reponse[MAX_BUFF];
    // Envoi du message MSG au serveur : la réponse sera OK
    CHECK(write(sd, MSG, strlen(MSG) + 1), "Can't send");
    CHECK(read(sd, reponse, sizeof(reponse)), "Can't send");
    // Envoi du message ERR au serveur : la réponse sera NOK
    CHECK(write(sd, ERR, strlen(ERR) + 1), "Can't send");
    CHECK(read(sd, reponse, sizeof(reponse)), "Can't send");
    // Envoi du message BYE au serveur : la réponse sera la fin du dialogue
    CHECK(write(sd, BYE, strlen(BYE) + 1), "Can't send");
    CHECK(read(sd, reponse, sizeof(reponse)), "Can't send");
}*/




int main() {
    int sock;

    struct sockaddr_in svc;
    // Création de la socket d’appel et de dialogue
    CHECK(sock = socket(PF_INET, SOCK_STREAM, 0), "Can't create")
    // Préparation de l’adressage du service à contacter
    svc.sin_family = PF_INET;
    svc.sin_port = htons(PORT_SVC);
    svc.sin_addr.s_addr = inet_addr(INADDR_SVC);
    memset(&svc.sin_zero, 0, 8);
    // Demande d’une connexion au service
    CHECK(connect(sock, (struct sockaddr *) &svc, sizeof svc), "Can't connect")
    // Dialogue avec le serveur

    printf("App Projet MCS Defoy-Lengle\n\n");
    enum Etat e = NC;
    while (1) {
        int choix = afficherMenu(e);
        switch (e) {
            case NC:
                switch (choix) {
                    case 1: {
                        printf("Entrez votre pseudonyme (max 20 caractères):\n");
                        char pseudo[20];
                        fgets(pseudo, 20, stdin);
                        sendReq(CONN_CLIENT_MASTER, sock, pseudo);
                        e = CO;
                        break;
                    }
                    case 2:
                        quit(sock);
                        return 0;
                    default:
                        printf("Choix incorrect");
                        break;
                }
                break;
            case CO:
                switch (choix) {
                    case 1:

                        break;
                    case 2:
                        break;
                    case 3:
                        quit(sock);
                        return 0;
                    default:
                        printf("Choix incorrect");
                        break;
                }
                break;
            default:
                break;
        }
    }
    close(sock);
    return 0;

}

void quit(int sock) {
    puts("Au revoir !");
    sendReq(DMD_DECONNEXION, sock, "");
    close(sock);
}

int afficherMenu(enum Etat e) {
    switch (e) {
        case NC:
            puts("\t1) Connexion au serveur");
            puts("\t2) Quitter");
            break;
        case CO:
            puts("\t1) Demander le catalogue");
            puts("\t2) Commander Un Produit");
            puts("\t3) Deconnexion");
            break;
        default:
            break;
    }
    printf("Votre choix ?");
    int choix;
    scanf("%d", &choix); //NOLINT
    getchar();
    return choix;
}

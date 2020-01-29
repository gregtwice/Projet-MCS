//
// Created by gdefoy on 11/01/2020.
//

/**
 * \file Client/main.c
 * \brief gerer le client
 */

#include <signal.h>
#include "client.h"
#include "../Include/utils.h"


enum Etat connexion_serveur_master();

/**
 * @fn afficherMenu(enum Etat e)
 * @param e l'etat du client
 * @return le choix de l'utilisateur
 */
int afficherMenu(enum Etat e);

/**
 * \fn enum Etat connexion_serveur_master(int sock)
 * \param sock
 * \return etat de connection
 */
enum Etat connexion_serveur_master(int sock) {
    enum Etat e;
    printf("Entrez votre pseudonyme (max 20 caractères):\n");
    char pseudo[20];
    fgets(pseudo, 20, stdin);
//    strcpy(pseudo, "Grégoire\n");
    sendReq(CONN_CLIENT_MASTER, sock, pseudo);
    read(sock, buffer, sizeof(buffer));
    if (strcmp(buffer, "OK") == 0) {
        puts("Connexion Réussie");
    } else {
        puts("Connexion Échouée");
    }
    e = CO;
    return e;
}

int sock;

/**
 * @fn void quit()
 * @brief Permet de quitter le serveur
 */
void quit();

/**
 * void commander_produit(int sock)
 * @param sock
 */
void commander_produit(int sock);

/**
 * \fn void handler()
 * \brief permet de ce deconnecter
 */
void handler() {
    puts("Au revoir !");
    sendReq(DMD_DECONNEXION, sock, "");
    close(sock);
    exit(0);
}

/**
 * \fn void afficher_catalogue(int sock)
 * \brief permet d'afficher le catalogue du serveur maitre
 * \param sock
 */
void afficher_catalogue(int sock) {
    char prot[5];
    protocol_as_char(DMD_CATALOGUE, prot);
    prot[4] = 32;
    write(sock, prot, 5);
    read(sock, buffer, sizeof(buffer));
    printf("Le catalogue est le suivant: %s", buffer);
}


int main() {
    signal(SIGINT, handler);

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
                        e = connexion_serveur_master(sock);
                        break;
                    }
                    case 2:
                        quit();
                        return 0;
                    default:
                        printf("Choix incorrect");
                        break;
                }
                break;
            case CO:
                switch (choix) {
                    case 1:
                        afficher_catalogue(sock);
                        break;
                    case 2:
                        commander_produit(sock);
                        break;
                    case 3:
                        quit();
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

/**
 * \fn void commander_produit(int sock)
 * \param sock
 * \brief permet de commander un produit au pres du serveur maitre
 */
void commander_produit(int sock) {
    int nproduit, quantite;

    puts("\n\nVeuillez saisir le numéro du produit ainsi que la quantité sous la forme n°-quantité");
    printf("\tVotre choix :");
    char input[10];
    fgets(input, 10, stdin);
    char *saisie[10];
    int nb = parser(saisie, input, "-", 2);
    if (nb == 2) {
        nproduit = atoi(saisie[0]);
        quantite = atoi(saisie[1]);
        printf("\tVous avez demandé %d en quantité %d\n", nproduit, quantite);
        char prot[20];
        protocol_as_char(DMD_PROD_SERV_MASTER, prot);
        sprintf(prot, "%s%s-%s", prot, saisie[0], saisie[1]);
        printf("le message total :%s\n", prot);
        write(sock, prot, strlen(prot) + 1);
        read(sock, buffer, sizeof(buffer));
        char *code = strtok(buffer, ":");
        if (strcmp(code, "203") == 0) {
            printf("\n\nCommande éffectuée\n");
        } else if (strcmp(code, "403") == 0) {
            printf("\n\nErreur produit non disponible dans la quantité demandée \n");
        }

    } else {
        fprintf(stderr, "\tErreur de saisie\n\n\n");
    }

}

/**
 * \fn int afficherMenu(enum Etat e)
 * \brief permet au client de choisir ce qu'il veut faire
 * \param etat connecter ou non connecter
 * \return le choix du client
 */
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
    int choix = 0;
    scanf("%d", &choix); //NOLINT
    getchar();
    return choix;
}

/**
 * \fn void quit()
 * \brief permet au client de quitter la connection avec le seveur maitre
 */
void quit() {
    puts("Au revoir !");
    sendReq(DMD_DECONNEXION, sock, "");
    close(sock);
}
//
// Created by pierre on 14/01/2020.
//

#include <signal.h>
#include   "../Include/streamInc.h"
#include "../Include/protocols.h"
#include "../Include/utils.h"

char requete[MAX_BUFF];
char cdProduit[MAX_BUFF];
char qte[MAX_BUFF];

void writeCatalogue();

typedef struct {
    char nom[20];
    int nProd;
    int qte;
} produit_t;

typedef struct {
    produit_t produits[30];
    int nprods;
} catalogue_t;

catalogue_t catalogue;


void dialogueSvc(int sd, struct sockaddr_in svc) {

    do {
        read(sd, buffer, sizeof(buffer));
        sscanf(buffer, "%s:%s:%s", requete, cdProduit, qte);
        char *req = strtok(buffer, ":");
        char *reste = strtok(NULL, ":");
        switch (atoi(requete)) {
            case DMD_DECONNEXION :
                printf("Au revoir\n");
                break;
            case DMD_PROD_ENTREPOT : {
                //demande produit entrepot
                char *split[4];
                parser(split, requete, ":", 4);
                for (int i = 0; i < 4; ++i) {
                    printf("%s\n", split[i]);
                }
                int prod = atoi(split[1]);
                int qte = atoi(split[2]);
                int flagFound = 0;
                for (int j = 0; j < catalogue.nprods; ++j) {
                    if (catalogue.produits[j].nProd == prod) {
                        flagFound = 1;
                        if (catalogue.produits[j].qte >= qte) {
                            char response[30];
                            char prot[4];
                            printf("QUANTITÉ OK !!!!\n");
                            protocol_as_char(PROD_DISPO_QTE, prot);
                            sprintf(response, "%s%s", prot, split[3]);
                            write(sd, response, strlen(response));
                            writeCatalogue();
                        } else {
                            char response[10];
                            printf("QUANTITÉ OK !!!!");
                            protocol_as_char(PROD_DISPO_NON_QTE, response);
                            write(sd, response, strlen(response));
                        }
                        break;
                    }
                }
                if (!flagFound) {

                }

//                printf("%s\n", requete);
//                printf("%s\n", qte);
//                parser()
                break;
            }
            case CMD_PROD_ENTREPOT : {
                char *split[3];
                parser(split, reste, ":", 2);
                int prod = atoi(split[0]);
                int qte = atoi(split[1]);
                for (int i = 0; i < catalogue.nprods; ++i) {
                    if (catalogue.produits[i].nProd == prod) {
                        catalogue.produits[i].qte -= qte;
                    }
                }
                break;
            }
            default:
//                write(sd, NOK, strlen(NOK) + 1);
//                printf("NOK : message recu %s\n", buffer);
                break;
        }
    } while (atoi(requete) != 102);
}


void getinfos(int no) {
    printf("Ici !!\n");
    FILE *file;
    char filename[100];
    sprintf(filename, "./infoEntrepot/info_e%d", no);
    file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    char adresse[100];
    fgets(adresse, 99, file);
    printf("%s\n", adresse);
    sprintf(filename, "./catalogues/catalogue_e%d.txt", no);
    fclose(file);
    file = fopen(filename, "r");
    if (file == NULL) {
        return;
    }
    char produitstr[100];
    char c;
    int n = 0;
    int i = 0;
    while ((c = getc(file)) != EOF) {
        produitstr[i++] = c;
        if (c == '\n') {
            produitstr[i + 1] = '\0';
            char *split[3];
            parser(split, produitstr, ":", 3);
            produit_t produit;
            produit.qte = atoi(split[2]);
            strcpy(produit.nom, split[1]);
            produit.nProd = atoi(split[0]);
            catalogue.produits[n++] = produit;
            catalogue.nprods++;
            i = 0;
        }
    }
    printf("%d\n", n);

    for (int j = 0; j < n; ++j) {
        produit_t x = catalogue.produits[j];
        printf("%d %s %d\n", x.nProd, x.nom, x.qte);
    }
    fclose(file);
}

int sock;
int eNumber;

void writeCatalogue() {
    char *filename = NULL;
    sprintf(filename, "./catalogues/catalogue_e%d.txt", eNumber);
        printf("écriture du ficher\n");
    FILE *f = fopen(filename, "w");
    if (f == NULL) {
        return;
    }
    for (int i = 0; i < catalogue.nprods; ++i) {
        produit_t x = catalogue.produits[i];
        fprintf(f, "%d:%s:%d", x.nProd, x.nom, x.qte);
    }
    fclose(f);
}


void quit() {
    write(sock, "102:", 5);
    close(sock);

    exit(0);
}

int main(int argc, char **argv) {

    if (argc <= 1) {
        fprintf(stderr, "Pas assez d'arguments");
        return 1;
    }




    signal(SIGINT, quit);
    struct sockaddr_in svc;
    struct sockaddr_in clt;
    catalogue.nprods = 0;

    // Création de la socket pour envoi de la requête
    CHECK(sock = socket(PF_INET, SOCK_STREAM, 0), "Can't create");
    // Préparation de l’adressage du service à contacter

    int no = atoi(argv[1]);
    eNumber = no;
    getinfos(no);

    svc.sin_family = PF_INET;
//    if (argc > 1) {
//        svc.sin_addr.s_addr = inet_addr(argv[1]);
//        svc.sin_port = htons(atoi(argv[2]));
//    } else {
    svc.sin_port = htons(PORT_SVC);
    svc.sin_addr.s_addr = inet_addr(INADDR_SVC);
//    }

    memset(&svc.sin_zero, 0, 8);
    CHECK(connect(sock, (struct sockaddr *) &svc, sizeof svc), "Can't connect")
    char message[10] = "101:";

//    CHECK(sendto(sock, message, strlen(message) + 1, 0, (struct sockaddr *) &svc, sizeof svc), "Can't send");
    CHECK(write(sock, message, strlen(message) + 1), "Can't write");

    int servMaitre = sizeof(svc); //  Réception d’un message
    read(sock, buffer, sizeof(buffer));
    sscanf(buffer, "%s:%s", requete, buffer);
    if (atoi(requete) == 200) {
        printf("Connecter au serveur maitre\n");
    } else {
        close(sock);
        return 0;
    }

    dialogueSvc(sock, svc);
    close(sock);
    return 0;
}


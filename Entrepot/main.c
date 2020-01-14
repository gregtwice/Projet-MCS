//
// Created by pierre on 14/01/2020.
//

#include   "../Include/streamInc.h"
char requete[MAX_BUFF];
char cdProduit[MAX_BUFF];
char qte[MAX_BUFF];
void dialogueSvc (int sd, struct sockaddr_in svc) {

    do {

        read(sd, buffer, sizeof(buffer));
        sscanf (buffer, "%s:%s:%s",requete, cdProduit, qte);
        switch (atoi(requete)) {
            case 102 :
                printf("Au revoir\n");
                break;
            case 120 :
                //demande produit entrepot
                break;
            case 121 :
                //commande produit entrepot
                break;
            default: write(sd, NOK, strlen(NOK)+1);
                printf("NOK : message recu %s\n", buffer);
                break;
        }
    }
    while ( atoi(requete) != 102);
}

void creationCatalogueLocal(){

    FILE *P_FICHIER;
    P_FICHIER = fopen("catalogue.txt", "w");  /* write */
    fprintf(P_FICHIER, "%s:%s:%s\n", "1", "Meuble", "2");
    fprintf(P_FICHIER, "%s:%s:%s\n", "2", "Livre", "10");
    fprintf(P_FICHIER, "%s:%s:%s\n", "3", "Table", "5");
    fprintf(P_FICHIER, "%s:%s:%s\n", "4", "TV", "0");
    fprintf(P_FICHIER, "%s:%s:%s\n", "5", "Chaise", "1");
    fprintf(P_FICHIER, "%s:%s:%s\n", "6", "Planche", "15");
    fclose(P_FICHIER);

}

int main(int argc, char** argv) {
    int sock;
    struct sockaddr_in svc;
    struct sockaddr_in clt;
    creationCatalogueLocal();
    // Création de la socket pour envoi de la requête
    CHECK(sock=socket(PF_INET, SOCK_DGRAM, 0), "Can't create");
    // Préparation de l’adressage du service à contacter
    creationCatalogueLocal();

    svc.sin_family = PF_INET;
    if (argc > 1) {
        svc.sin_addr.s_addr = inet_addr(argv[1]);
        svc.sin_port = htons(atoi(argv[2]));
    }
    else {
        svc.sin_port = htons(PORT_SVC);
        svc.sin_addr.s_addr = inet_addr(INADDR_SVC);
    }
    char message[10] = {'1','0','1',':','\0'};
    memset(&svc.sin_zero, 0, 8);
    CHECK(sendto(sock, message, strlen(message)+1, 0,  (struct sockaddr *)&svc, sizeof svc) , "Can't send");

    int servMaitre = sizeof(svc); //  Réception d’un message
    CHECK(recv(sock, buffer, sizeof(buffer), 0) , "Cant receive");
    sscanf (buffer, "%s:%s",requete, buffer);
    if(atoi(requete) == 200){
        printf("Connecter au serveur maitre\n");
    }else{
        close(sock);
        return 0;
    }

    dialogueSvc(sock,svc);


    close(sock);
    return 0;
}

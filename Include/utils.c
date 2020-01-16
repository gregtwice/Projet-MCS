//
// Created by gdefoy on 14/01/2020.
//

#include "utils.h"


char *address_to_char(struct in_addr sin_addr) {
    char *str = inet_ntoa(sin_addr);
    return str;
}

char *time_to_char(struct tm *localTime) {
    char *timeAsString = asctime(localTime);
    timeAsString[strlen(timeAsString) - 1] = '\0'; // asctime ajoute un \n à la fin , on le supprime
    return timeAsString;
}

struct tm *get_local_time() {
    time_t rawtime;
    // on récupère le temps
    time(&rawtime);
    // on passe en temps local
    struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    return timeinfo;
}

char *protocol_as_char(int protocol, char *prot) {
    sprintf(prot, "%d:", protocol);
    return prot;
}

int parser(char **strArr, char *toParse, char *sep, int nb) {

    int i = 0;
    char *reste = toParse;
    char *p = strtok_r(toParse, sep, &reste);
    while (p != NULL && i < nb) {
        strArr[i++] = p;
        p = strtok_r(reste, sep, &reste);
    }

    return i;
}
//
// Created by gdefoy on 14/01/2020.
//

#ifndef PROJET_UTILS_H
#define PROJET_UTILS_H

#include "streamInc.h"
#include <time.h>

/**
 * Convertit une adresse en chaine de caractères.
 * @param sin_addr l'adresse à convertir, l'adresse DOIT être au format ipv4
 * @return la chaine de caractère représentant l'adresse
 */
char *address_to_char(struct in_addr sin_addr);

char *time_to_char(struct tm *localTime);

struct tm *get_local_time();

#endif //PROJET_UTILS_H

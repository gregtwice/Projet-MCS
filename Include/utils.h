//
// Created by gdefoy on 14/01/2020.
//
/**
 * \file Include/utils.h
 */
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
/**
 * temps en caractere
 * @param localTime
 * @return
 */
char *time_to_char(struct tm *localTime);
/*
 * renvoie le temps actuel
 */
struct tm *get_local_time();
/**
 * renvoie le protocol de int en chaine de caractere
 * @param protocol
 * @return
 */
char * protocol_as_char(int protocol,char * );

int parser(char **strArr, char *toParse, char *sep, int nb);

#endif //PROJET_UTILS_H

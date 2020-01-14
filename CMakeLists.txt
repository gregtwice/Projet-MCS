cmake_minimum_required(VERSION 3.15)
project(Projet C)

set(CMAKE_C_STANDARD 11)

add_executable(
        Serveur
        Serveur/main.c
        Include/protocols.h
        Include/utils.c Include/utils.h Serveur/serveur.c Serveur/serveur.h)

add_executable(
        Client
        Client/main.c
        Include/protocols.h
        Client/client.c
        Client/client.h
        Include/utils.c Include/utils.h)

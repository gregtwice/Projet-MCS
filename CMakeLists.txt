cmake_minimum_required(VERSION 3.15)
project(Projet_MCS C)

set(CMAKE_C_STANDARD 11)

include_directories(Client)
include_directories(Include)
include_directories(Serveur)

add_executable(Projet_MCS
        Client/client.c
        Client/client.h
        Client/main.c
        Include/protocols.h
        Include/streamInc.h
        Entrepot/main.c)

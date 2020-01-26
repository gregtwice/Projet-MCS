cmake_minimum_required(VERSION 3.15)
project(Projet_MCS C)
find_package(Threads)
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
target_link_libraries(Serveur ${CMAKE_THREAD_LIBS_INIT})

add_executable(
        Entrepot
        Entrepot/main.c
        Include/protocols.h
        Include/streamInc.h
        Include/utils.h
        Include/utils.c
)
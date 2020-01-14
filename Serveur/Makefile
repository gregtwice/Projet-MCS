all : serveur.o utils.o
	gcc serveur.o utils.o main.c -o build/serveur.exe -lpthread

serveur.o : serveur.c serveur.h
	gcc -c serveur.c

utils.o: ../Include/utils.c ../Include/utils.h
	gcc -c ../Include/utils.c

clean :
	rm *.exe
	rm *.o

all: client

client: client.o main.o utils.o
	gcc main.c client.o ../Include/utils.o -o build/client.exe

client.o : client.c
	gcc -c client.c

main.o : main.c
	gcc -c main.c

utils.o: ../Include/utils.c
	cd ../Include; gcc -c utils.c

clean:
	rm *.exe
	rm *.o
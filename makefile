OBJETS = affichage.o deplacement.o interface.o main.o niveau.o terrains.o edit_main.o edit_affichage.o
HEADERS = affichage.h deplacement.h interface.h main.h niveau.h terrains.h edit_main.h edit_affichage.h
CC = gcc
CFLAGS = -Wall -g

all : SuperScalableLand clean

SuperScalableLand : $(OBJETS)
	$(CC) $(CFLAGS) $(OBJETS) -o SuperScalableLand -lcairo -lSDL -lSDL_ttf -lSDL_image -lSDL_mixer

main.o : main.c $(HEADERS)
	$(CC) $(CFLAGS) -c main.c 
	
affichage.o : affichage.c $(HEADERS)
	$(CC) $(CFLAGS) -c affichage.c

deplacement.o : deplacement.c $(HEADERS)
	$(CC) $(CFLAGS) -c deplacement.c
	
interface.o : interface.c $(HEADERS)
	$(CC) $(CFLAGS) -c interface.c
	
niveau.o : niveau.c $(HEADERS)
	$(CC) $(CFLAGS) -c niveau.c
	
terrains.o : terrains.c $(HEADERS)
	$(CC) $(CFLAGS) -c terrains.c
	
edit_main.o : edit_main.c $(HEADERS)
	$(CC) $(CFLAGS) -c edit_main.c
	
edit_affichage.o : edit_affichage.c $(HEADERS)
	$(CC) $(CFLAGS) -c edit_affichage.c
	

clean :
	rm -f *.o

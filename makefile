OBJETS = affichage.o deplacement.o interface.o main.o niveau.o
HEADERS = affichage.h deplacement.h interface.h main.h niveau.h
CC = gcc
CFLAGS = -Wall -g

all : SuperScalableLand clean

SuperScalableLand : $(OBJETS)
	$(CC) $(CFLAGS) $(OBJETS) -lSDL -lSDL_Image -lSDL_ttf -lcairo -o SuperScalableLand

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
	
clean :
	rm -f *.o
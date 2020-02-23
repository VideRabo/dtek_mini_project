CC = gcc

default: main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h settings.h settings.c
	$(CC) main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h settings.h settings.c -o build.o

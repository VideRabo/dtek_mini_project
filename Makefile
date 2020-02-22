CC = gcc

default: main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h display.c
	$(CC) main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h display.c -o build.o

CC = gcc

default: main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h
	$(CC) main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h -o build.o

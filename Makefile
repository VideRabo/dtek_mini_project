CC = gcc

default: main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h settings.h settings.c display.c input.c input.h
	$(CC) main.c config.h gamelogic.c gamelogic.h mapmanager.c mapmanager.h settings.h settings.c display.c input.c input.h -o build.o

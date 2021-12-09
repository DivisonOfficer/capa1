#Makefile

cache : main.c mycache.c cblock.h mycache_dependent.c
	gcc -g -o cache main.c mycache.c mycache_dependent.c
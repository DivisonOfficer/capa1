#Makefile

pa1.out : main.c mycache.c cblock.h mycache_dependent.c
	gcc -g -o pa1.out main.c mycache.c mycache_dependent.c
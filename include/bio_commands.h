#include "bio_struct.h"
#ifndef BIO_COMMANDS_H
#define BIO_COMMANDS_H

#define MAX_CMD 128  
#define MAX_ARG 64

typedef struct 
{
    char cmd[16];   // Comando principal (bio)
    char arg1[MAX_ARG];  // Subcomando (start, read, search)
    char arg2[MAX_ARG];  // Argumento adicional 
} Comando;


void mostrar_bienvenida();

void ejecutar_cli();

void leer_comando(Comando *c);

int ejecutar_comando(Comando *c, Trie** trie);

void bio_start(const char* profundidad_str, Trie** trie);
void bio_read(const char* filename, Trie* trie);
void bio_search(Trie* trie, const char* gen);


#endif
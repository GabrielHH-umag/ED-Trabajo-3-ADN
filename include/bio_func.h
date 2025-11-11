#ifndef BIO_FUNC_H
#define BIO_FUNC_H

#include "bio_struct.h"

/* Prototipos públicos */
Nodo* crear_nodo_recursivo(int nivel_actual, int profundidad_total);
void inicializar_trie(Trie* trie, int profundidad);
void liberar_nodo(Nodo* nodo);
void liberar_trie(Trie* trie);
int char_a_indice(char c);
void insertar_en_trie(Trie* trie, const char* secuencia, int posicion);

#endif // BIO_FUNC_H
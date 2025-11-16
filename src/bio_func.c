#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bio_struct.h"
#include "bio_func.h"

Nodo* crear_nodo_recursivo(int nivel_actual, int profundidad_total) 
{
    //Profundidad total representa la altura que elije el usuario
    Nodo *nodo;
    if ((nodo = malloc(sizeof *nodo)) == NULL) return NULL;

    nodo->esHoja = (nivel_actual == profundidad_total);
    nodo->posiciones = NULL;
    nodo->numPosiciones = 0;

    for (int i = 0; i < 4; i++) nodo->hijos[i] = NULL; 

    if (!nodo->esHoja) 
    {
        for (int i = 0; i < 4; i++)
        {
            nodo->hijos[i] = crear_nodo_recursivo(nivel_actual + 1, profundidad_total);
            if (nodo->hijos[i] == NULL)
            {
                for (int j = 0; j < i; j++)
                    liberar_nodo(nodo->hijos[j]);
                /* nodo->posiciones es NULL aquí (inicializado arriba), no es necesario free */
                free(nodo);
                return NULL;
            }
        }
    }
    return nodo;
}


void inicializar_trie(Trie* trie, int profundidad) {
    trie->profundidad = profundidad;

    // Crear arbol completo con la funcion recursiva 
    trie->raiz = crear_nodo_recursivo(0, profundidad);

    if (!trie->raiz) {
        fprintf(stderr, "Error: no se pudo crear el arbol.\n");
        exit(1);
    }
}


void liberar_nodo(Nodo* nodo) {
    if (!nodo) return;
    for (int i = 0; i < 4; i++) liberar_nodo(nodo->hijos[i]);
    free(nodo->posiciones);
    free(nodo);
}

void liberar_trie(Trie* trie) {
    if (!trie) return;
    liberar_nodo(trie->raiz);
    free(trie);
}

 int char_a_indice(char c) 
 {
    int u = toupper((unsigned char)c);
    switch (u) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default:  return -1; // Carácter inválido
    }
}

void insertar_en_trie(Trie* trie, const char* secuencia, int posicion) 
{
    if (!trie || !trie->raiz || !secuencia) 
        return;

    /* validar longitud para evitar indexación fuera de bounds */
    if (strlen(secuencia) < (size_t)trie->profundidad) {
        fprintf(stderr, "Secuencia demasiado corta (m=%d)\n", trie->profundidad);
        return;
    }

    Nodo* actual = trie->raiz;
    for (int i = 0; i < trie->profundidad; i++) 
    {
        int indice = char_a_indice(secuencia[i]);
        if (indice < 0) 
            return; //carácter inválido se detiene la inserción
        if (!actual->hijos[indice]) 
            return;

        actual = actual->hijos[indice];
    }
    actual->esHoja = 1;

    size_t new_count = (size_t)actual->numPosiciones + 1;
    int *tmp = realloc(actual->posiciones, new_count * sizeof *actual->posiciones);
    if (!tmp) 
        return; // si falla realloc, no escribimos
    actual->posiciones = tmp;
    actual->posiciones[actual->numPosiciones++] = posicion;
}
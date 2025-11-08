#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "bio_struct.h"

Nodo* crear_nodo_recursivo(int nivel_actual, int profundidad_total) 
{
    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    nodo->esHoja = (nivel_actual == profundidad_total);
    nodo->posiciones = NULL;
    nodo->numPosiciones = 0;

    for (int i = 0; i < 4; i++)
    {
        nodo->hijos[i] = NULL;
    }
    if (!nodo->esHoja) 
    {
        for (int i = 0; i < 4; i++)
        {
            nodo->hijos[i] = crear_nodo_recursivo(nivel_actual + 1, profundidad_total);
        }
    }
    return nodo;
}

void inicializar_trie(Trie* trie, int profundidad)
{
    trie->profundidad = profundidad;
    trie->raiz = crear_nodo_recursivo(1, profundidad);
}
void liberar_nodo(Nodo* nodo) 
{
    if (nodo == NULL) return;
    for (int i = 0; i < 4; i++) 
    {
        liberar_nodo(nodo->hijos[i]);
    }
    free(nodo->posiciones);
    free(nodo);
}
void liberar_trie(Trie* trie) 
{
    if (trie == NULL) return;
    liberar_nodo(trie->raiz);
    free(trie);
}

int char_a_indice(char c) 
{
    switch (c) 
    {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default: return -1; // Carácter inválido
    }
}
void insertar_en_trie(Trie* trie, const char* secuencia, int posicion) 
{
    Nodo* actual = trie->raiz;
    for (int i = 0; i < trie->profundidad; i++) 
    {
        int indice = char_a_indice(secuencia[i]);
        if (indice == -1) 
        {
            printf("Carácter inválido en la secuencia: %c\n", secuencia[i]);
            return;
        }
        if (actual->hijos[indice] == NULL) 
        {
            actual->hijos[indice] = (Nodo*)malloc(sizeof(Nodo));
            actual->hijos[indice]->esHoja = 0;
            actual->hijos[indice]->numPosiciones = 0;
            actual->hijos[indice]->posiciones = NULL;

            for (int j = 0; j < 4; j++)
                actual->hijos[indice]->hijos[j] = NULL;
        }
        actual = actual->hijos[indice];
    }
    // Llegamos al nodo hoja
    actual->esHoja = 1;
    actual->numPosiciones++;
    actual->posiciones = (int*)realloc(actual->posiciones,(actual->numPosiciones + 1) * sizeof(int));
    actual->posiciones[actual->numPosiciones - 1] = posicion;
}

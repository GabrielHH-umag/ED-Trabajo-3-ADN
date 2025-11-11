#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/stat.h>
#include <unistd.h>
#include <sys/types.h>
#include <dirent.h>
#include "bio_func.h"

Nodo* crear_nodo_recursivo(int nivel_actual, int profundidad_total) 
{
    Nodo *nodo;
    if ((nodo = malloc(sizeof *nodo)) == NULL) return NULL;

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
            if (nodo->hijos[i] == NULL)
            {
                for (int j = 0; j < i; j++)
                    liberar_nodo(nodo->hijos[j]);
                free(nodo->posiciones);
                free(nodo);
                return NULL;
            }
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
    if (trie == NULL || trie->raiz == NULL || secuencia == NULL) 
        return;

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
            Nodo *nuevo;
            if ((nuevo = malloc(sizeof *nuevo)) == NULL) return;
            
            nuevo->esHoja = 0;
            nuevo->numPosiciones = 0;
            nuevo->posiciones = NULL;
            for (int j = 0; j < 4; j++) nuevo->hijos[j] = NULL;
            actual->hijos[indice] = nuevo;
        }

        actual = actual->hijos[indice];
        if (actual == NULL) return; 
    }
    // Llegamos al nodo hoja
    actual->esHoja = 1;
    actual->numPosiciones++;
    actual->posiciones = (int*)realloc(actual->posiciones,(actual->numPosiciones + 1) * sizeof(int));
    actual->posiciones[actual->numPosiciones - 1] = posicion;
}

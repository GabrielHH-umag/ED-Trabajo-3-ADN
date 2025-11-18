/**
 * @file bio_func.c
 * @brief Implementación de las funciones principales para la creación, manejo
 *        e inserción de genes en un Trie 4-ario utilizado para el análisis de ADN.
 *
 * Este módulo implementa:
 * - La construcción recursiva del árbol Trie de altura m.
 * - La conversión de caracteres de ADN a índices (A,C,G,T).
 * - La inserción de apariciones de genes dentro de nodos hoja.
 * - La liberación completa y segura de toda la estructura.
 *
 * Cada nodo del Trie corresponde a una base nitrogenada y, al llegar al nivel m,
 * se obtiene una hoja que almacena dinámicamente todas las posiciones en las que
 * aparece ese gen dentro de la secuencia S.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "bio_struct.h"
#include "bio_func.h"

/* ------------------------------------------------------------------------- */
/* ----------------------- CREACIÓN DEL TRIE (RECURSIVA) -------------------- */
/* ------------------------------------------------------------------------- */

Nodo* crear_nodo_recursivo(int nivel_actual, int profundidad_total) 
{
    /**
     * @brief Crea un nodo del Trie y, si no es hoja, crea sus 4 hijos recursivamente.
     *
     * @param nivel_actual      Nivel en el que se está creando el nodo.
     * @param profundidad_total Nivel máximo (profundidad m) del Trie.
     *
     * La función genera un árbol completamente expandido desde la raíz
     * hasta todas las hojas, representando los 4^m posibles genes.
     */

    Nodo* nodo = (Nodo*)malloc(sizeof(Nodo));
    if (!nodo) return NULL;
    nodo->esHoja = (nivel_actual == profundidad_total);
    nodo->posiciones = NULL;
    nodo->numPosiciones = 0;

    /* Inicializar hijos */
    for (int i = 0; i < 4; i++) nodo->hijos[i] = NULL;

    /* Si no es hoja, crear subárboles */
    if (!nodo->esHoja) {
        for (int i = 0; i < 4; i++) {
            nodo->hijos[i] = crear_nodo_recursivo(nivel_actual + 1, profundidad_total);
        }
    }
    return nodo;
}


void inicializar_trie(Trie* trie, int profundidad) {
    /**
     * @brief Construye un Trie completamente expandido a partir de la profundidad m.
     *
     * @param trie        Puntero a la estructura Trie a inicializar.
     * @param profundidad Profundidad total del árbol (longitud del gen m).
     */

    trie->profundidad = profundidad;

    trie->raiz = crear_nodo_recursivo(0, profundidad);

    if (!trie->raiz) {
        fprintf(stderr, "Error: no se pudo crear el arbol.\n");
        exit(1);
    }
}

/* ------------------------------------------------------------------------- */
/* ----------------------------- LIBERACIÓN MEMORIA ------------------------ */
/* ------------------------------------------------------------------------- */

void liberar_nodo(Nodo* nodo) {
    /**
     * @brief Libera recursivamente un nodo, sus hijos y sus posiciones.
     *
     * @param nodo Puntero al nodo a liberar.
     */

    if (!nodo) return;
    for (int i = 0; i < 4; i++) liberar_nodo(nodo->hijos[i]);
    free(nodo->posiciones);
    free(nodo);
}

void liberar_trie(Trie* trie) {
    /**
     * @brief Libera por completo la estructura Trie, incluida su raíz.
     *
     * @param trie Puntero al Trie.
     */

    if (!trie) return;
    liberar_nodo(trie->raiz);
    free(trie);
}

/* ------------------------------------------------------------------------- */
/* ------------------------ UTILIDADES DE APOYO ----------------------------- */
/* ------------------------------------------------------------------------- */

int char_a_indice(char c) {
    /**
     * @brief Convierte un carácter de ADN en un índice entre 0 y 3.
     *
     * @param c Carácter ('A', 'C', 'G', 'T').
     * @return Índice correspondiente o -1 si el carácter es inválido.
     */

    switch (c) {
        case 'A': return 0;
        case 'C': return 1;
        case 'G': return 2;
        case 'T': return 3;
        default:  return -1; // invalido
    }
}

/* ------------------------------------------------------------------------- */
/* --------------------------- INSERCIÓN DE GENES --------------------------- */
/* ------------------------------------------------------------------------- */

void insertar_en_trie(Trie* trie, const char* secuencia, int posicion) 
{
    /**
     * @brief Inserta un gen (cadena de longitud m) dentro del Trie.
     *
     * La inserción consiste en:
     * 1. Navegar desde la raíz siguiendo cada base nitrogenada.
     * 2. Al llegar a la hoja, expandir su arreglo dinámico de posiciones.
     *
     * @param trie      Trie ya inicializado mediante `bio start`.
     * @param secuencia Cadena de caracteres A, C, G, T de longitud m.
     * @param posicion  Posición de inicio del gen dentro de la secuencia S.
     */

    if (!trie || !trie->raiz || !secuencia) 
        return;

    Nodo* actual = trie->raiz;

    /* Recorrer cada nivel del Trie */
    for (int i = 0; i < trie->profundidad; i++) 
    {
        int indice = char_a_indice(secuencia[i]);
        if (indice < 0) 
            return; /* Carácter inválido */
        if (!actual->hijos[indice]) 
            return; /* Camino inconsistente (no debería ocurrir en Trie expandido) */

        actual = actual->hijos[indice];
    }
    actual->esHoja = 1;

    /* Redimensionar arreglo dinámico de posiciones */
    int *tmp = (int*)realloc(actual->posiciones, (size_t)(actual->numPosiciones + 1) * sizeof(int));
    if (!tmp) 
        return; /* Si falla realloc, no insertamos */
    actual->posiciones = tmp;
    actual->posiciones[actual->numPosiciones++] = posicion;
}
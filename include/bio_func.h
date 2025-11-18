/**
 * @file bio_func.h
 * @brief Funciones principales del módulo de administración del Trie 4-ario
 *        utilizado para almacenar genes en la secuencia de ADN.
 *
 * Este archivo contiene las funciones encargadas de:
 * - Crear el Trie completo de profundidad m.
 * - Liberar toda la memoria asociada.
 * - Insertar genes detectados en la secuencia.
 * - Navegar mediante índices derivados de caracteres A, C, G y T.
 */

#ifndef BIO_FUNC_H
#define BIO_FUNC_H

#include "bio_struct.h"

/* ------------------------------------------------------------------------- */
/* -------------------------- CREACIÓN DEL TRIE ---------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * @brief Crea de forma recursiva los nodos del Trie 4-ario hasta alcanzar
 *        la profundidad especificada.
 *
 * Esta función construye el árbol completo antes de insertar cualquier gen.
 * Los nodos hoja se caracterizan por tener `nivel_actual == profundidad_total`.
 *
 * @param nivel_actual       Nivel actual dentro del Trie (comienza en 0).
 * @param profundidad_total  Altura máxima del árbol, también llamada m.
 *
 * @return Puntero al nodo raíz del subárbol creado, o NULL en caso de fallo.
 */
Nodo* crear_nodo_recursivo(int nivel_actual, int profundidad_total);

/**
 * @brief Inicializa completamente un Trie construyendo su árbol interno.
 *
 * @param trie        Puntero al Trie a inicializar.
 * @param profundidad Profundidad del árbol, equivalente al tamaño m del gen.
 *
 * Se crea un Trie completamente expandido, con todos los nodos necesarios
 * para representar todas las combinaciones posibles de genes de longitud m.
 */
void  inicializar_trie(Trie* trie, int profundidad);

/* ------------------------------------------------------------------------- */
/* --------------------------- LIBERACIÓN MEMORIA -------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * @brief Libera recursivamente un nodo y todos sus hijos.
 *
 * @param nodo Nodo que se desea liberar.
 *
 * Se encarga de liberar:
 * - todos los subárboles,
 * - arreglos dinámicos de posiciones,
 * - y el propio nodo.
 */
void  liberar_nodo(Nodo* nodo);

/**
 * @brief Libera completamente un Trie, incluyendo su raíz y todos los nodos.
 *
 * @param trie Puntero al Trie que debe ser liberado.
 *
 * Esta función se invoca al finalizar el programa o cuando el usuario ejecute
 * el comando `bio exit`.
 */
void  liberar_trie(Trie* trie);

/* ------------------------------------------------------------------------- */
/* -------------------------- UTILIDADES DE APOYO -------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * @brief Convierte un carácter representando una base nitrogenada
 *        en su índice correspondiente dentro de los hijos del Trie.
 *
 * Mapeo válido:
 * - A → 0  
 * - C → 1  
 * - G → 2  
 * - T → 3  
 *
 * @param c Carácter a convertir.
 * @return Índice entre 0 y 3, o -1 si el carácter es inválido.
 */
int   char_a_indice(char c);

/**
 * @brief Inserta una aparición del gen dentro de la hoja correspondiente del Trie.
 *
 * La cadena `secuencia` debe tener longitud igual a `trie->profundidad`.
 * Esta función:
 * - Navega el Trie desde la raíz,
 * - Localiza la hoja del gen,
 * - Amplía el arreglo de posiciones mediante realloc,
 * - Inserta la posición al final.
 *
 * @param trie      Trie ya inicializado.
 * @param secuencia Cadena de longitud m que representa el gen encontrado.
 * @param posicion  Posición dentro de la secuencia S en la cual inicia el gen.
 */
void  insertar_en_trie(Trie* trie, const char* secuencia, int posicion);

#endif // BIO_FUNC_H

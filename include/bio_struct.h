/**
 * @file bio_struct.h
 * @brief Definición de las estructuras principales utilizadas por el
 *        Analizador de ADN basado en un Trie 4-ario.
 *
 * Este módulo declara las estructuras fundamentales del proyecto:
 * - @ref Nodo : representa cada nodo del árbol 4-ario.
 * - @ref Trie : representa el árbol completo utilizado para indexar genes
 *               de longitud m dentro de la secuencia genética S.
 *
 * El árbol 4-ario se utiliza para almacenar todas las combinaciones posibles
 * de secuencias génicas de longitud fija, siguiendo el modelo de A, C, G y T.
 */

#ifndef BIO_STRUCT_H
#define BIO_STRUCT_H

/**
 * @struct Nodo
 * @brief Representa un nodo del Trie utilizado para almacenar genes.
 *
 * Cada nodo posee exactamente 4 hijos, uno por cada base nitrogenada:
 * - 〈0〉 → A  
 * - 〈1〉 → C  
 * - 〈2〉 → G  
 * - 〈3〉 → T  
 *
 * Los nodos hoja (nivel == profundidad m) almacenan dinámicamente las
 * posiciones dentro de la secuencia S donde aparece el gen correspondiente.
 */
typedef struct Nodo 
{
    int esHoja;             /**< Indica si el nodo es una hoja (nivel m). */
    struct Nodo* hijos[4]; /**< Punteros a los hijos correspondientes a A, C, G y T. */
    int* posiciones;       /**< Arreglo dinámico de posiciones donde aparece el gen. */
    int numPosiciones;     /**< Cantidad de posiciones almacenadas en el arreglo. */
} Nodo;

/**
 * @struct Trie
 * @brief Representa el árbol 4-ario completo para la indexación de genes.
 *
 * La profundidad del Trie corresponde al tamaño m de los genes que se desean
 * detectar dentro de la secuencia genética S. Cada camino desde la raíz hasta
 * un nodo hoja representa un posible gen de longitud m.
 */
typedef struct Trie 
{
    Nodo* raiz;         /**< Puntero al nodo raíz del Trie. */
    int profundidad;    /**< Profundidad total m (tamaño del gen). */
} Trie;

#endif // BIO_STRUCT_H
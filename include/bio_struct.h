#ifndef BIO_STRUCT_H
#define BIO_STRUCT_H


typedef struct Nodo {
    int esHoja;
    struct Nodo* hijos[4]; // A,C,G,T
    int* posiciones;       // Array de posiciones donde aparece
    int numPosiciones;     // Cantidad de posiciones almacenadas
} Nodo;

typedef struct Trie {
    Nodo* raiz;
    int profundidad;      // Valor m especificado en bio start
} Trie;
/* sin usar por ahora
typedef struct Resultado {
    char* gen;           // Secuencia del gen
    int frecuencia;      // Número de apariciones
    int* posiciones;     // Array de posiciones
    struct Resultado* siguiente;
} Resultado;

typedef struct Lista {
    Resultado* primero;
    int cantidad;
} Lista;
*/

#endif // BIO_STRUCT_H
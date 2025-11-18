/**
 * @file bio_commands.h
 * @brief Declaraciones del módulo encargado de la interfaz de comandos (CLI)
 *        para el Analizador de ADN basado en estructuras Trie.
 *
 * Este módulo implementa el manejo de entrada del usuario, el procesamiento
 * de comandos y la vinculación con las funciones lógicas internas del
 * analizador de ADN. Los comandos disponibles permiten iniciar el árbol,
 * cargar una secuencia genética desde archivo, buscar genes, listar todos
 * los existentes y calcular los de mayor o menor frecuencia.
 */

#ifndef BIO_COMMANDS_H
#define BIO_COMMANDS_H

#include "bio_struct.h"

/** 
 * @brief Tamaño máximo permitido para una línea completa ingresada por el usuario.
 * Se utiliza para buffer seguro en la lectura del comando.
 */
#define MAX_CMD 128

/**
 * @brief Tamaño máximo de cada argumento individual de un comando.
 * Este valor previene desbordamientos en cadenas largas.
 */
#define MAX_ARG 64

/**
 * @struct Comando
 * @brief Estructura que representa un comando ingresado por el usuario.
 *
 * Un comando válido tiene la forma:
 * @code
 * bio <accion> <argumento>
 * @endcode
 *
 * Ejemplos:
 * - bio start 4  
 * - bio read adn.txt  
 * - bio search ACTG  
 */
typedef struct {
    char cmd[16];       /**< Comando principal. Siempre debe ser "bio". */
    char arg1[MAX_ARG]; /**< Subcomando (start, read, search, all, max, min, exit). */
    char arg2[MAX_ARG]; /**< Argumento adicional. */
} Comando;

/**
 * @brief Muestra un mensaje inicial con información del programa.
 */
void mostrar_bienvenida(void);

/**
 * @brief Inicia el bucle principal del intérprete de comandos.
 *
 * Esta función mantiene activo el programa leyendo comandos del usuario
 * hasta que se invoque explícitamente la acción `bio exit`.
 */
void ejecutar_cli(void);

/**
 * @brief Lee una línea ingresada por el usuario y separa los argumentos.
 *
 * @param c Puntero a una estructura Comando donde se almacenará la información.
 *
 * La función extrae:
 * - el comando base,
 * - la acción,
 * - un argumento adicional (si existe).
 */
void leer_comando(Comando *c);

/**
 * @brief Ejecuta la acción solicitada por el usuario.
 *
 * @param c    Comando ya separado en cmd, arg1 y arg2.
 * @param trie Doble puntero al Trie, ya que puede inicializarse dentro del comando `start`.
 *
 * @return 0 para finalizar el programa, 1 para continuar ejecutando la CLI.
 */
int  ejecutar_comando(Comando *c, Trie** trie);

/* ------------------------------------------------------------------------- */
/* ------------------------ PROTOTIPOS DE ACCIONES ------------------------- */
/* ------------------------------------------------------------------------- */

/**
 * @brief Inicializa la estructura Trie con una profundidad dada.
 *
 * @param profundidad_str Cadena que representa el valor entero de m.
 * @param trie            Doble puntero al Trie. Puede crear uno nuevo.
 */
void bio_start(const char* profundidad_str, Trie** trie);

/**
 * @brief Lee un archivo de texto con la secuencia genética S e inserta todos los genes posibles.
 *
 * @param filename Nombre del archivo a leer.
 * @param trie     Trie ya inicializado mediante `bio start`.
 */
void bio_read(const char* filename, Trie* trie);

/**
 * @brief Busca un gen específico dentro del Trie y muestra todas sus posiciones.
 *
 * @param trie Trie previamente cargado.
 * @param gen  Cadena de longitud m a buscar.
 */
void bio_search(Trie* trie, const char* gen);

/**
 * @brief Lista todos los genes presentes en el Trie junto con sus posiciones.
 *
 * @param trie Trie previamente cargado.
 */
void bio_all(Trie* trie);

/**
 * @brief Muestra los genes con la mayor frecuencia en el Trie.
 *
 * @param trie Trie previamente cargado.
 */
void bio_max(Trie* trie);

/**
 * @brief Muestra los genes con la menor frecuencia en el Trie.
 *
 * @param trie Trie previamente cargado.
 */
void bio_min(Trie* trie);

#endif // BIO_COMMANDS_H

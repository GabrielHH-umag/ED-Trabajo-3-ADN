/**
 * @file main.c
 * @brief Punto de entrada del Analizador de ADN basado en estructuras Trie.
 *
 * Este archivo inicializa la interfaz de comandos (CLI) del programa,
 * la cual permite ejecutar:
 *  - bio start m
 *  - bio read archivo.txt
 *  - bio search GEN
 *  - bio all
 *  - bio max
 *  - bio min
 *  - bio exit
 *
 * La lógica principal y las funciones internas se encuentran en los módulos
 * bio_commands.c, bio_func.c y bio_struct.h. Este archivo únicamente inicia
 * el flujo de ejecución.
 */

#include "bio_commands.h"

/**
 * @brief Función principal del programa.
 *
 * Inicia la interfaz de comandos interactiva, donde el usuario puede realizar
 * todas las operaciones del analizador de ADN. El control se mantiene dentro
 * de `ejecutar_cli()` hasta que el usuario ingresa `bio exit`.
 *
 * @return 0 al finalizar correctamente.
 */
int main() {
    ejecutar_cli();
    return 0;
}

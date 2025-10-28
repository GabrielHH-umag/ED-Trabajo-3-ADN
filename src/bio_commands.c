#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bio_commands.h"

void mostrar_bienvenida() {
    printf("=========================================\n");
    printf("            Analizador de ADN            \n");
    printf("      Escriba 'bio exit' para salir.     \n");
    printf("=========================================\n");
}

void leer_comando(Comando *c) {
    char buffer[MAX_CMD];
    printf("> ");
    fgets(buffer, sizeof(buffer), stdin);

    buffer[strcspn(buffer, "\n")] = '\0';

    strcpy(c->cmd, "");
    strcpy(c->arg1, "");
    strcpy(c->arg2, "");

    sscanf(buffer, "%15s %63s %63s", c->cmd, c->arg1, c->arg2);
}

int ejecutar_comando(Comando *c) {
    if (strcmp(c->cmd, "bio") != 0) {
        printf("Comando no reconocido. Use 'bio <accion>'.\n");
        return 1;
    }

    if (strcmp(c->arg1, "start") == 0) {
        printf("-> Creando árbol con profundidad %s (pendiente)...\n", c->arg2);
    } 
    else if (strcmp(c->arg1, "read") == 0) {
        printf("-> Leyendo secuencia desde archivo '%s' (pendiente)...\n", c->arg2);
    } 
    else if (strcmp(c->arg1, "search") == 0) {
        printf("-> Buscando gen '%s' (pendiente)...\n", c->arg2);
    } 
    else if (strcmp(c->arg1, "max") == 0) {
        printf("-> Mostrando gen/es más repetidos (pendiente)...\n");
    } 
    else if (strcmp(c->arg1, "min") == 0) {
        printf("-> Mostrando gen/es menos repetidos (pendiente)...\n");
    } 
    else if (strcmp(c->arg1, "all") == 0) {
        printf("-> Mostrando todos los genes (pendiente)...\n");
    } 
    else if (strcmp(c->arg1, "exit") == 0) {
        printf("Saliendo del programa...\n");
        return 0;
    } 
    else {
        printf("Accion '%s' no reconocida.\n", c->arg1);
    }

    return 1;
}

void ejecutar_cli() {
    Comando c;
    mostrar_bienvenida();

    while (1) {
        leer_comando(&c);
        if (ejecutar_comando(&c) == 0) break;
    }
}
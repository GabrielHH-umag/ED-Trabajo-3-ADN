#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "bio_commands.h"
#include "bio_func.h"

void mostrar_bienvenida() 
{
    printf("=========================================\n");
    printf("            Analizador de ADN            \n");
    printf("      Escriba 'bio exit' para salir.     \n");
    printf("=========================================\n");
}

void leer_comando(Comando *c) 
{
    char buffer[MAX_CMD];
    printf("> ");
    fgets(buffer, sizeof(buffer), stdin);
    buffer[strcspn(buffer, "\n")] = '\0';
    strcpy(c->cmd, "");
    strcpy(c->arg1, "");
    strcpy(c->arg2, "");
    sscanf(buffer, "%15s %63s %63s", c->cmd, c->arg1, c->arg2);
}

int ejecutar_comando(Comando *c, Trie** trie) 
{
    // Validar comando principal
    if (strcmp(c->cmd, "bio") != 0) 
    {
        printf("Comando no reconocido. Use 'bio <accion>'.\n");
        return 1;
    }
    //Ejecutar los comandos
    if (strcmp(c->arg1, "start") == 0) 
    {
        printf("-> Creando árbol con profundidad %s\n", c->arg2);
        bio_start(c->arg2, trie);
    } 
    else if (strcmp(c->arg1, "read") == 0) 
    {
        printf("-> Leyendo secuencia desde archivo '%s'\n", c->arg2);
        bio_read(c->arg2, *trie);
    } 
    else if (strcmp(c->arg1, "search") == 0) 
    {
        printf("-> Buscando gen '%s' (pendiente)...\n", c->arg2);
        bio_search(*trie, c->arg2);
    } 
    else if (strcmp(c->arg1, "max") == 0) 
    {
        printf("-> Mostrando gen/es más repetidos (pendiente)...\n");
    } 
    else if (strcmp(c->arg1, "min") == 0) 
    {
        printf("-> Mostrando gen/es menos repetidos (pendiente)...\n");
    } 
    else if (strcmp(c->arg1, "all") == 0) 
    {
        printf("-> Mostrando todos los genes (pendiente)...\n");
    } 
    else if (strcmp(c->arg1, "exit") == 0) 
    {
        printf("Saliendo del programa...\n");
        return 0;
    } 
    else 
    {
        printf("Accion '%s' no reconocida.\n", c->arg1);
    }

    return 1;
}

void ejecutar_cli() 
{
    Comando c;
    Trie* trie = NULL;
    mostrar_bienvenida();

    while (1) 
    {
        leer_comando(&c);
        if (ejecutar_comando(&c, &trie) == 0) break;
    }
    liberar_trie(trie);
}

void bio_start(const char* profundidad_str, Trie** trie) 
{
    if (*trie != NULL) 
    {
        printf("El trie ya ha sido inicializado. Reinicie para cambiar la profundidad.\n");
        return;
    }
    if (atoi(profundidad_str) <= 0) 
    {
        printf("Profundidad inválida. Debe ser un número entero positivo.\n");
        return;
    }
    *trie = malloc(sizeof(Trie));
    if (*trie == NULL) 
    {
        printf("Error al asignar memoria para el trie.\n");
        return;
    }
    inicializar_trie(*trie, atoi(profundidad_str));
    //printf("Trie inicializado con profundidad %s.\n", profundidad_str);
}
void bio_read(const char* filename, Trie* trie) 
{
    if (trie == NULL|| trie->raiz == NULL) 
    {
        printf("El trie no ha sido inicializado. Use 'bio start <profundidad>' primero.\n");
        return;
    }
    FILE* file = fopen(filename, "r");
    if (file == NULL) 
    {
        printf("No se pudo abrir el archivo: %s\n", filename);
        return;
    }
    char secuencia[1000];
    secuencia[0] = '\0';
    char linea[256];
    while (fgets(linea, sizeof(linea), file)) 
    {
        linea[strcspn(linea, "\n")] = '\0';
        strcat(secuencia, linea);
    }
    fclose(file);
    int longitud = strlen(secuencia);
    
    if (longitud < trie->profundidad) 
    {
        printf("La secuencia es más corta que la profundidad del trie.\n");
        return;
    }
    for (int i = 0; i <= longitud - trie->profundidad; i++) 
    {
        char gen[32];
        strncpy(gen, secuencia + i, trie->profundidad);
        gen[trie->profundidad] = '\0';
        insertar_en_trie(trie, gen, i);
    }
    //printf("Secuencia leída e insertada en el trie.\n");
}
void bio_search(Trie* trie, const char* secuencia) 
{
    if (trie == NULL || trie->raiz == NULL) 
    {
        printf("El trie no ha sido inicializado.\n");
        return;
    }
    Nodo* actual = trie->raiz;
    for (int i = 0; i < trie->profundidad; i++)
    {
        int indice = char_a_indice(secuencia[i]);
        if (indice == -1 || actual->hijos[indice] == NULL) 
        {
            printf("El gen '%s' no se encontró en la secuencia.\n", secuencia);
            // Secuencia no encontrada
            return;
        }
        actual = actual->hijos[indice];
    }
    if (actual->numPosiciones == 0) 
    {
        printf("El gen '%s' no se encontró en la secuencia.\n", secuencia);
        // Secuencia no encontrada
        return;
    }
    // Secuencia encontrada
    printf("Secuencia '%s' encontrada en posiciones: ", secuencia);
    for (int i = 0; i < actual->numPosiciones; i++) 
    {
        printf("%d ", actual->posiciones[i]);
    }
    printf("\n");
}
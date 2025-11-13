#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <limits.h>

#include "bio_commands.h"
#include "bio_func.h"

/* ----------------- Forward declarations (por si el .h aún no los tiene) ----------------- */
static int  validar_gen(const char* s, int m);
static Nodo* navegar(Trie* trie, const char* s);
static void imprimir_posiciones(const Nodo* n);

void bio_all(Trie* trie);
void bio_max(Trie* trie);
void bio_min(Trie* trie);

/* ------------------------------------ CLI ------------------------------------ */

void mostrar_bienvenida() {
    printf("=========================================\n");
    printf("            Analizador de ADN            \n");
    printf("      Escriba 'bio exit' para salir.     \n");
    printf("=========================================\n");
}

void leer_comando(Comando *c) {
    char buffer[MAX_CMD];
    printf("> ");
    if (!fgets(buffer, sizeof(buffer), stdin)) {
        c->cmd[0] = c->arg1[0] = c->arg2[0] = '\0';
        return;
    }
    buffer[strcspn(buffer, "\n")] = '\0';
    c->cmd[0] = c->arg1[0] = c->arg2[0] = '\0';
    sscanf(buffer, "%15s %63s %63s", c->cmd, c->arg1, c->arg2);
}

int ejecutar_comando(Comando *c, Trie** trie) {
    if (strcmp(c->cmd, "bio") != 0) {
        printf("Comando no reconocido. Use 'bio <accion>'.\n");
        return 1;
    }

    if (strcmp(c->arg1, "start") == 0) {
        bio_start(c->arg2, trie);
    } else if (strcmp(c->arg1, "read") == 0) {
        bio_read(c->arg2, *trie);
    } else if (strcmp(c->arg1, "search") == 0) {
        bio_search(*trie, c->arg2);    // imprime posiciones o -1
    } else if (strcmp(c->arg1, "max") == 0) {
        bio_max(*trie);                // imprime empates con mayor frecuencia
    } else if (strcmp(c->arg1, "min") == 0) {
        bio_min(*trie);                // imprime empates con menor frecuencia (>0)
    } else if (strcmp(c->arg1, "all") == 0) {
        bio_all(*trie);                // imprime todos los genes presentes con posiciones
    } else if (strcmp(c->arg1, "exit") == 0) {
        printf("Clearing cache and exiting…\n"); 
        return 0;
    } else {
        printf("Accion '%s' no reconocida.\n", c->arg1);
    }

    return 1;
}

void ejecutar_cli() {
    Comando c;
    Trie* trie = NULL;
    mostrar_bienvenida();

    while (1) {
        leer_comando(&c);
        if (ejecutar_comando(&c, &trie) == 0) break;
    }
    liberar_trie(trie);
}

/* -------------------------- START / READ (IO + carga) -------------------------- */

void bio_start(const char* profundidad_str, Trie** trie) {
    if (*trie != NULL) {
        printf("El trie ya ha sido inicializado. Reinicie para cambiar la profundidad.\n");
        return;
    }
    if (!profundidad_str || atoi(profundidad_str) <= 0) {
        printf("Profundidad invalida. Debe ser un numero entero positivo.\n");
        return;
    }
    *trie = (Trie*)malloc(sizeof(Trie));
    if (*trie == NULL) {
        printf("Error al asignar memoria para el trie.\n");
        return;
    }
    inicializar_trie(*trie, atoi(profundidad_str));
    printf("Tree created with height %d\n", (*trie)->profundidad);
}

void bio_read(const char* filename, Trie* trie) {
    if (!trie || !trie->raiz) { printf("El trie no ha sido inicializado...\n"); return; }
    FILE* file = fopen(filename, "r");
    if (!file) { printf("No se pudo abrir: %s\n", filename); return; }

    int m = trie->profundidad;
    static const size_t MAX = 100000;
    char *secuencia = malloc(MAX + 1);
    if (!secuencia) { fclose(file); return; }
    size_t len = 0;

    int ch;
    while ((ch = fgetc(file)) != EOF) {
        if (ch == '\n' || ch == '\r') continue;
        if (len < MAX) secuencia[len++] = (char)toupper((unsigned char)ch);
        else break;
    }
    fclose(file);
    if (len < (size_t)m) { printf("La secuencia es mas corta que m.\n"); free(secuencia); return; }
    secuencia[len] = '\0';

    for (size_t i = 0; i + m <= len; i++) {
        // crear ventana m segura
        char *gen = malloc((size_t)m + 1);
        if (!gen) break;
        memcpy(gen, &secuencia[i], (size_t)m);
        gen[m] = '\0';
        insertar_en_trie(trie, gen, (int)i);
        free(gen);
    }
    free(secuencia);
    printf("Sequence S read from file\n");
}

/* -------------------------- Helpers de busqueda/impresion -------------------------- */

static int validar_gen(const char* s, int m) {
    if (!s) return 0;
    for (int i = 0; i < m; i++) {
        char c = s[i];
        if (c == '\0') return 0;
        if (!(c == 'A' || c == 'C' || c == 'G' || c == 'T')) return 0;
    }
    return s[m] == '\0'; // exactamente largo m
}

static Nodo* navegar(Trie* trie, const char* s) {
    Nodo* act = trie->raiz;
    for (int i = 0; i < trie->profundidad; i++) {
        int idx = char_a_indice(s[i]);
        if (idx < 0 || !act->hijos[idx]) return NULL;
        act = act->hijos[idx];
    }
    return act;
}

static void imprimir_posiciones(const Nodo* n) {
    for (int i = 0; i < n->numPosiciones; i++) {
        printf("%d", n->posiciones[i]);
        if (i + 1 < n->numPosiciones) printf(" ");
    }
    printf("\n");
}

/* -------------------------- SEARCH (salida: posiciones o -1) -------------------------- */

void bio_search(Trie* trie, const char* secuencia) {
    if (!trie || !trie->raiz || !secuencia) { printf("-1\n"); return; }
    int m = trie->profundidad;
    char *buf = malloc((size_t)m + 1);
    if (!buf) { printf("-1\n"); return; }
    for (int i = 0; i < m && secuencia[i]; i++)
        buf[i] = (char)toupper((unsigned char)secuencia[i]);

    if ((int)strlen(secuencia) != m) {
        free(buf);
        printf("-1\n");
        return;
    }
    buf[m] = '\0';
    if (!validar_gen(buf, m)) {
        free(buf);
        printf("-1\n");
        return;
    }
    Nodo* hoja = navegar(trie, buf);
    if (!hoja || hoja->numPosiciones == 0) {
        free(buf);
        printf("-1\n");
        return;
    }
    imprimir_posiciones(hoja);
    free(buf);
}


/* -------------------------- ALL -------------------------- */

static void dfs_all(Nodo* nodo, char* pref, int depth, int m) {
    if (!nodo) return;
    if (depth == m) {
        if (nodo->numPosiciones > 0) {
            pref[m] = '\0';
            printf("%s ", pref);
            imprimir_posiciones(nodo);
        }
        return;
    }
    static const char L[4] = {'A','C','G','T'};
    for (int i = 0; i < 4; i++) {
        pref[depth] = L[i];
        dfs_all(nodo->hijos[i], pref, depth + 1, m);
    }
}

void bio_all(Trie* trie) {
    if (!trie || !trie->raiz) return;
    int m = trie->profundidad;
    char *pref = malloc((size_t)m + 1);
    if (!pref) return;
    dfs_all(trie->raiz, pref, 0, m);
    free(pref);
}

/* -------------------------- MAX / MIN -------------------------- */

static void dfs_freq(Nodo* nodo, int depth, int m, int* maxf, int* minf) {
    if (!nodo) return;
    if (depth == m) {
        if (nodo->numPosiciones > 0) {
            if (nodo->numPosiciones > *maxf) *maxf = nodo->numPosiciones;
            if (nodo->numPosiciones < *minf) *minf = nodo->numPosiciones;
        }
        return;
    }
    for (int i = 0; i < 4; i++) dfs_freq(nodo->hijos[i], depth + 1, m, maxf, minf);
}

static void dfs_print_by_freq(Nodo* nodo, char* pref, int depth, int m, int target) {
    if (!nodo) return;
    if (depth == m) {
        if (nodo->numPosiciones == target && target > 0) {
            pref[m] = '\0';
            printf("%s ", pref);
            imprimir_posiciones(nodo);
        }
        return;
    }
    static const char L[4] = {'A','C','G','T'};
    for (int i = 0; i < 4; i++) {
        pref[depth] = L[i];
        dfs_print_by_freq(nodo->hijos[i], pref, depth + 1, m, target);
    }
}

void bio_max(Trie* trie) {
    if (!trie || !trie->raiz) { printf("-1\n"); return; }
    int maxf = 0, minf = INT_MAX;
    dfs_freq(trie->raiz, 0, trie->profundidad, &maxf, &minf);
    if (maxf <= 0) { printf("-1\n"); return; }
    char *pref = malloc((size_t)trie->profundidad + 1);
    if (!pref) return;
    dfs_print_by_freq(trie->raiz, pref, 0, trie->profundidad, maxf);
    free(pref);
}

void bio_min(Trie* trie) {
    if (!trie || !trie->raiz) { printf("-1\n"); return; }
    int maxf = 0, minf = INT_MAX;
    dfs_freq(trie->raiz, 0, trie->profundidad, &maxf, &minf);
    if (minf == INT_MAX) { printf("-1\n"); return; }
    char *pref = malloc((size_t)trie->profundidad + 1);
    if (!pref) return;
    dfs_print_by_freq(trie->raiz, pref, 0, trie->profundidad, minf);
    free(pref);
}

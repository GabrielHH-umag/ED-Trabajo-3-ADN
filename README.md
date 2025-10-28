> Universidad de Magallanes — Ingeniería Civil en Computación e Informática  
> Estructura de Datos — Grupo 3 — Tarea 3: Analizador de de secuencias de ADN

# 🧬 Analizador ADN

**Analizar ADN** es un sistema de análisis de secuencias de ADN usando árboles 4-arios (Tries). Este trabajo se desarrolló en C para la asignatura de **Estructura de Datos** siendo la **Tarea 3**.

## 👥 Integrantes
- [Diego Herrera](https://github.com/diherrerab) - correo@umag.cl
- [Catalina Viñas](https://github.com/2wedcfvb) - correo@umag.cl
- [Oscar Cifuentes](https://github.com/iBluZiiZ) - ocifuent@umag.cl 
- [Gabriel Hernández](https://github.com/GabrielHH-umag) - correo@umag.cl
- [Sebastián Vera](https://github.com/SeVAguilar) - correo@umag.cl

## 🎯 Objetivos
- Implementar y manipular estructuras de datos abstractas como árboles y listas enlazadas para almacenar y gestionar los datos manipulados.
- Desarrollar habilidades en programación en lenguaje C, centrándose en el manejo de memoria, punteros y eficiencia algorítmica.
- Implementar un sistema de búsqueda de patrones utilizando árboles tries.

## ⚙️ Compilación
El proyecto utiliza un **Makefile multiplataforma (Linux/Windows)** que automatiza todas las tareas de compilación, limpieza, prueba y empaquetado del proyecto.

### 🔧 Comandos del Makefile
| Comando        | Descripción                                                                     |
| -------------- | ------------------------------------------------------------------------------- |
| `make`         | Compila el proyecto y genera el ejecutable principal en la carpeta `build/bin/`.|
| `make clean`   | Elimina los archivos `.o` y los ejecutables generados (`adn`).                  |
| `make send`    | Crea el archivo comprimido `G3-3.tgz` con los archivos requeridos para entrega. |
| `make docs`    | Genera documentación con Doxygen.                                               |
| `make rebuild` | Limpia y recompila completamente el proyecto desde cero.                        |
| `make folders` | Crea las carpetas necesarias para el funcionamiento del proyecto (si no existen)|

### 🧩 Compilación del programa
```bash
make
```
El ejecutable queda en la carpeta `build/bin/` como:
- adn.exe  (Windows)
- adn      (Linux)

### 🧹 Limpiar archivos binarios:
```bash
make clean
```

## ▶️ Uso del programa
El programa se ejecuta desde la consola e interpreta comandos **prefijados** con `adn`.

### 📋 Funciones disponibles
| Comando               | Descripción                                                                  |
| --------------------- | ---------------------------------------------------------------------------- |
| `bio start m`         | Crea el árbol con profundidad `m`.                                           |
| `bio read adn.txt`    | Lee el archivo con la secuencia S.                                           |
| `bio search G`        | Busca el gen `G` y muestra posiciones.                                       |
| `bio max`             | Muestra los genes más repetidos.                                             |
| `bio min`             | Muestra los genes menos repetidos.                                           |
| `bio all`             | Muestra todos los genes y posiciones.                                        |
| `bio exit`            | Libera la memoria y cierra el programa.                                      |

## 🧪 Pruebas automáticas
Próximamente se agregarán casos de prueba en el directorio `test/`. Si es que se hace.

## 🧭 ¿Cómo dejar el programa funcional?
1. Al clonar el repositorio en su ordenador debe dirigirse a la carpeta en donde se encuentra el proyecto.
2. Debe crear las carpetas faltantes que son necesarias para el funcionamiento del programa.
```bash
make folders
```
3. Puede compilar y ejecutar el programa con las pruebas automáticas:
```bash
make test
```
4. O, si desea compilarlo sin ejecutar las pruebas:
```bash
make
```
5. Luego, ejecuta el programa:
```bash
./build/bin/adn     # Linux
build\bin\adn.exe   # Windows
```
6. Puede probar el [ejemplo de uso](#-ejemplo-de-uso), o por usted mismo.


## 📌 Ejemplo de uso
```bash
Al final.
```

## 📂 Estructura del proyecto
```java
ED-Trabajo-3-ADN/
│
├── src/           → Código fuente (.c)
├── include/       → Archivos de cabecera (.h)
├── test/          → Pruebas automáticas
├── docs/          → Documentación e informe
├── build/
│   ├── obj/       → Archivos objeto (.o)
│   └── bin/       → Ejecutables compilados
├── README.md
├── Doxyfile
└── Makefile
```
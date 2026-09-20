# Informatica_II_Reto1
# Sweet Crush - Bitwise Game Engine (Reto 1)

Un juego tipo *Match-3* desarrollado en **C++ puro**, optimizado a bajo nivel mediante empaquetamiento de bits en memoria dinámica y desarrollado bajo restricciones académicas estricta (sin bibliotecas externas, ni STL, ni programación orientada a objetos).

---

## Características Principales

* **Empaquetamiento de Bits (Bit-Packing):** Representación del tablero utilizando buffers unidimensionales `unsigned char*`, almacenando y leyendo fichas mediante desplazamientos (`<<`, `>>`) y máscaras binarias (`&`, `|`).
* **Sistema de Cascadas y Reacciones en Cadena:** Evaluación recursiva/iterativa de coincidencias horizontales y verticales de 3 o más fichas.
* **Detección de Cruces $T$ / $L$:** Cálculo de intersecciones especiales combinando máscaras mediante el operador `AND` a nivel de bits.
* **Ajuste Dinámico de Tablero:** Capacidad de insertar o eliminar filas y columnas modificando la memoria asignada.
* **Interfaz de Consola UTF-8:** Visualización de símbolos griegos ($\beta, \omega, \sigma, \gamma, \theta, \mu$) e inspección del tablero en formato binario.

---

## Restricciones Técnicas Cumplidas

Para este proyecto se implementó la arquitectura cumpliendo los siguientes parámetros:
- **Sin STL:** Uso exclusivo de punteros nativos (`unsigned char*`, `int*`) y memoria dinámica (`new[]` / `delete[]`).
- **Sin Programación Orientada a Objetos:** Implementado sin `struct`, `class` ni `template`.
- **Gestión Manual de Memoria:** Control estricto para evitar fugas (*memory leaks*).

---

## Estructura del Proyecto

```text
.
├── main.cpp                # Ciclo principal del juego, menús y HUD
├── logicaJuego.cpp/.h      # Reglas del juego, cascadas, gravedad y combos
├── motorDeBits.cpp/.h      # Lectura, escritura y cálculo de desplazamientos de bits
├── manejoMemoria.cpp/.h    # Asignación, redimensión y liberación de memoria dinámica
├── interfazUsuario.cpp/.h  # Impresión en consola, renderizado UTF-8 y validaciones
└── README.md               # Documentación del repositorio
```

## Arquitectura del Estado de Juego

El estado global se administra a través de un único arreglo contiguo en memoria (`int estadoJuego[9]`):

| Índice | Variable | Descripción |
| :---: | :--- | :--- |
| `0` | `FILAS` | Cantidad de filas actuales |
| `1` | `COLUMNAS` | Cantidad de columnas actuales |
| `2` | `BYTES_RESERVADOS` | Memoria total asignada para el tablero |
| `3` | `MOVIMIENTOS` | Acciones realizadas por el jugador |
| `4` | `FICHAS_ELIMINADAS` | Total acumulado de fichas destruidas |
| `5` | `COMBOS` | Total de combinaciones detectadas |
| `6` | `CASCADAS` | Fases de caídas consecutivas |
| `7` | `PUNTOS` | Puntuación total acumulada |
| `8` | `TOTAL_ELIMINACIONES` | Veces que el usuario usó la opción de eliminar |

---

## Autores

* **EMILIANO DE JESUS LINCE DIAZ - ESTEBAN GARCES ATEHORTUA** - *Desarrollo e implementación*
* **Universidad de Antioquia - Ingeniería Electrónica**
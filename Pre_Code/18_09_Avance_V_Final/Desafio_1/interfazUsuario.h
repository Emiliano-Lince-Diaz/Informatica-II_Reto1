#ifndef INTERFAZUSUARIO_H
#define INTERFAZUSUARIO_H

// Códigos de las opciones del menú en partida (se exponen para que
// el módulo de lógica del juego pueda comparar el valor devuelto
// por mostrarMenuPartida() sin usar números "mágicos").
const int OPCION_ELIMINAR_FICHA   = 1;
const int OPCION_AGREGAR_FILA     = 2;
const int OPCION_ELIMINAR_FILA    = 3;
const int OPCION_AGREGAR_COLUMNA  = 4;
const int OPCION_ELIMINAR_COLUMNA = 5;
const int OPCION_AYUDA            = 6;
const int OPCION_SALIR            = 7;

// Función auxiliar para pausar el juego hasta que el usuario presione ENTER
void esperarTecla();

// --- Validación de entrada ---
// Pide un entero mayor o igual a "minimo", repitiendo hasta que la
// entrada sea válida (maneja tanto texto no numérico como valores
// fuera de rango).
int leerEnteroValidado(const char* mensaje, int minimo);

// --- Menús ---
// Menú de inicio: pide filas y columnas del tablero (ya validados,
// siempre >= 1) y los devuelve por referencia.
void mostrarMenuInicio(int &filas, int &columnas);

// Menú en partida: muestra las opciones disponibles, pide una opción
// válida (1-7) y la retorna. No ejecuta ninguna acción de juego.
int mostrarMenuPartida();

// Muestra la lista de comandos válidos y qué hace cada uno.
void mostrarAyuda();

// --- Impresión del tablero ---
// Imprime el tablero con la representación gráfica de cada ficha,
// con los índices de fila/columna para que el usuario pueda ubicar
// una ficha al elegir coordenadas.
void imprimirTableroFichas(unsigned char* tablero, int filas, int columnas);

// Imprime el tablero en formato binario (3 bits por ficha), pensado
// para verificación del profesor.
void imprimirTableroBinario(unsigned char* tablero, int filas, int columnas, int* estadoJuego);

// Imprime en pantalla las métricas y puntuación actual del juego
void imprimirEstadisticas(int* estadoJuego);

#endif

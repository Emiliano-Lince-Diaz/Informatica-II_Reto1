#ifndef MANEJOMEMORIA_H
#define MANEJOMEMORIA_H

// Crea dinámicamente el arreglo de bytes para el tablero inicial y calcula los bytes reservados
unsigned char* inicializarTablero(int* estadoJuego);

// Reserva nueva memoria, inserta una fila con fichas aleatorias en la posición indicada y reubica las demás
unsigned char* agregarFila(unsigned char* tableroViejo, int* estadoJuego);

// Elimina una fila desplazando las inferiores hacia arriba y reduce la memoria física si el uso cae por debajo del 65%
unsigned char* eliminarFila(unsigned char* tablero, int* estadoJuego);

// Construye un nuevo tablero, inserta una columna y recalcula el alineamiento de los bits desplazados
unsigned char* agregarColumna(unsigned char* tableroViejo, int* estadoJuego);

// Junta los bits hacia la izquierda para borrar una columna y reduce la memoria si la ocupación es menor al 65%
unsigned char* eliminarColumna(unsigned char* tablero, int* estadoJuego);

#endif // MANEJOMEMORIA_H

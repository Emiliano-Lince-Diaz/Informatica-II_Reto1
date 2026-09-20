#ifndef MOTORDEBITS_H
#define MOTORDEBITS_H

// Calcula la ubicación física (byte y desplazamiento de bits) de una ficha en el arreglo 1D a partir de sus coordenadas lógicas 2D
void calcularBtye_desplazamiento(int fila, int columna, int totalColumnas, int &indiceByte, int &offset);

// Extrae y retorna el valor de los 3 bits de una ficha ubicada en una posición física especifica
int leerFicha(unsigned char* tablero, int indiceByte, int offset);

// Inyecta un valor de 3 bits en la memoria física usando máscaras, protegiendo los bits de las fichas adyacentes
void escribirFicha(unsigned char* tablero, int indiceByte, int offset, int valorFicha);

#endif // MOTORDEBITS_H

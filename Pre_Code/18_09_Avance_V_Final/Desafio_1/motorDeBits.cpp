#include "motorDeBits.h"

void calcularBtye_desplazamiento(int fila, int columna, int totalColumnas, int &indiceByte, int &offset){
    // Convierte la coordenada 2D a un índice lineal (como si el tablero fuera de 1 sola dimensión)
    int indiceLineal = (fila * totalColumnas) + columna;

    // Cada ficha ocupa exactamente 3 bits
    int bitInicial = indiceLineal * 3;

    // Determina en qué byte físico cae la ficha y a partir de qué bit (offset) empieza
    indiceByte = bitInicial / 8;
    offset = bitInicial % 8;
}

int leerFicha(unsigned char* tablero, int indiceByte, int offset){
    int valorFicha = 0;

    // CASO 1: Los 3 bits de la ficha están completamente dentro del mismo byte
    if(offset<=5){
        valorFicha = (tablero[indiceByte] >> offset) & 7;
    }
    // CASO 2: La ficha está dividida (2 bits en el primer byte, 1 bit en el siguiente)
    else if(offset == 6){
        int byte1 = (tablero[indiceByte] >> offset) & 3;

        int byte2 = (tablero[indiceByte + 1] & 1) << 2;

        valorFicha = byte1 | byte2;
    }
    // CASO 3: La ficha está dividida (1 bit en el primer byte, 2 bits en el siguiente)
    else if(offset == 7){
        int byte1 = (tablero[indiceByte] >> offset) & 1;

        int byte2 = (tablero[indiceByte + 1] & 3) << 1;

        valorFicha = byte1 | byte2;
    }

    return valorFicha;
}

void escribirFicha(unsigned char* tablero, int indiceByte, int offset, int valorFicha){

    // CASO 1: La ficha cabe en un solo byte. Limpiamos los 3 bits con máscara AND NOT (~), luego insertamos con OR (|)
    if(offset <= 5){
        tablero[indiceByte] = tablero[indiceByte] & ~(7 << offset);

        tablero[indiceByte] = tablero[indiceByte] | (valorFicha << offset);
    }
    // CASO 2: Escribir dividiendo la ficha (2 bits en el byte actual, 1 bit en el siguiente)
    else if(offset == 6){
        int bitsByte1 = valorFicha & 3;

        tablero[indiceByte] = tablero[indiceByte] & ~(3 << offset);

        tablero[indiceByte] = tablero[indiceByte] | (bitsByte1 << offset);

        int bitsByte2 = (valorFicha >> 2) & 1;

        tablero[indiceByte + 1] = tablero[indiceByte + 1] & ~(1);

        tablero[indiceByte + 1] = tablero[indiceByte + 1] | bitsByte2;
    }
    // CASO 3: Escribir dividiendo la ficha (1 bit en el byte actual, 2 bits en el siguiente)
    else if(offset == 7){
        int bitsByte1 = valorFicha & 1;

        tablero[indiceByte] = tablero[indiceByte] & ~(1 << offset);

        tablero[indiceByte] = tablero[indiceByte] | (bitsByte1 << offset);

        int bitsByte2 = (valorFicha >> 1) & 3;

        tablero[indiceByte + 1] = tablero[indiceByte + 1] & ~(3);

        tablero[indiceByte + 1] = tablero[indiceByte + 1] | bitsByte2;
    }
}
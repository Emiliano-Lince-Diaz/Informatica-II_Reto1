#include <iostream>
#include "motorDeBits.h"
#include "manejoMemoria.h"
#include "interfazUsuario.h"
#include "logicaJuego.h"
#include <windows.h>

using namespace std;

const int FILAS = 0;
const int COLUMNAS = 1;
const int BYTESRESERVADOS = 2;
const int ELIM_USUARIO = 3;
const int FICHAS_ELIMINADAS = 4;
const int COMBOS = 5;
const int CASCADAS = 6;
const int PUNTOS = 7;

int main(){
    SetConsoleOutputCP(65001);

    int estadoJuego[8] = {0};

    mostrarMenuInicio(estadoJuego[FILAS], estadoJuego[COLUMNAS]);

    unsigned char* tablero = inicializarTablero(estadoJuego);

    cout << "Bytes reservados: " << estadoJuego[BYTESRESERVADOS] << endl;

    inicializarSemillaAleatoria(); // Arrancamos el motor de azar
    generarFichasAleatorias(tablero, estadoJuego[FILAS], estadoJuego[COLUMNAS]);

    bool jugando = true;

    while(jugando){
        imprimirTableroFichas(tablero, estadoJuego[FILAS], estadoJuego[COLUMNAS]);

        imprimirTableroBinario(tablero, estadoJuego[FILAS], estadoJuego[COLUMNAS], estadoJuego);

        imprimirEstadisticas(estadoJuego);

        esperarTecla();

        int opcion = mostrarMenuPartida();

        switch(opcion){
        case OPCION_ELIMINAR_FICHA:
            eliminarFicha(tablero, estadoJuego);
            gravedad(tablero,estadoJuego);
            rellenar(tablero, estadoJuego);
            estadoJuego[CASCADAS] = 0;
            cout << "\nEvaluando tablero..." << endl;
            funcion_cascadas(tablero, estadoJuego);
            break;

        case OPCION_AGREGAR_FILA:
            tablero=agregarFila(tablero,estadoJuego);
            estadoJuego[CASCADAS] = 0;
            funcion_cascadas(tablero, estadoJuego);
            break;

        case OPCION_ELIMINAR_FILA:
            tablero=eliminarFila(tablero,estadoJuego);
            estadoJuego[CASCADAS] = 0;
            funcion_cascadas(tablero, estadoJuego);
            break;

        case OPCION_AGREGAR_COLUMNA:
            tablero = agregarColumna(tablero, estadoJuego);
            estadoJuego[CASCADAS] = 0;
            funcion_cascadas(tablero, estadoJuego);
            break;

        case OPCION_ELIMINAR_COLUMNA:
            tablero = eliminarColumna(tablero, estadoJuego);
            estadoJuego[CASCADAS] = 0;
            funcion_cascadas(tablero, estadoJuego);
            break;

        case OPCION_AYUDA:
            mostrarAyuda();
            break;

        case OPCION_SALIR:
            jugando = false;
            cout << "Gracias por jugar Sweet Crush." << endl;
            break;
        }
    }

    delete[] tablero;
    cout << "Memoria liberada" << endl;

    return 0;
}
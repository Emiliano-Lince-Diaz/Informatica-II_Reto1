#include <iostream>
#include <iomanip>
#include "interfazUsuario.h"
#include "motorDeBits.h"

using namespace std;


// Función auxiliar para pausar el juego hasta que el usuario presione ENTER
void esperarTecla() {
    cout << "\nPresione ENTER para abrir el menu de opciones...";
    cin.ignore(10000, '\n'); // Limpia el saldo del '\n' pendiente en la entrada de texto
    cin.get();               // Espera la tecla ENTER del usuario
}
// ============================================================
// Validación de entrada
// ============================================================
int leerEnteroValidado(const char* mensaje, int minimo){
    int valor;
    bool valido = false;

    do{
        cout << mensaje;
        cin >> valor;

        if(cin.fail()){
            // El usuario ingresó algo que no es un número.
            cin.clear();
            cin.ignore(10000, '\n');
            cout << "Entrada invalida. Debe ingresar un numero." << endl;
        }
        else if(valor < minimo){
            cout << "El valor debe ser mayor o igual a " << minimo << "." << endl;
        }
        else{
            valido = true;
        }

    }while(!valido);

    return valor;
}

// ============================================================
// Menú de inicio
// ============================================================
void mostrarMenuInicio(int &filas, int &columnas){
    cout << "=================================================" << endl;
    cout << "               BIENVENIDO A SWEET CRUSH           " << endl;
    cout << "=================================================" << endl;

    filas    = leerEnteroValidado("Ingrese la cantidad de filas del tablero: ", 1);
    columnas = leerEnteroValidado("Ingrese la cantidad de columnas del tablero: ", 1);

    cout << endl;
}

// ============================================================
// Menú en partida
// ============================================================
int mostrarMenuPartida(){
    cout << endl << "----------------- MENU EN PARTIDA -----------------" << endl;
    cout << OPCION_ELIMINAR_FICHA   << ". Eliminar ficha"   << endl;
    cout << OPCION_AGREGAR_FILA     << ". Agregar fila"     << endl;
    cout << OPCION_ELIMINAR_FILA    << ". Eliminar fila"    << endl;
    cout << OPCION_AGREGAR_COLUMNA  << ". Agregar columna"  << endl;
    cout << OPCION_ELIMINAR_COLUMNA << ". Eliminar columna" << endl;
    cout << OPCION_AYUDA            << ". Ayuda"            << endl;
    cout << OPCION_SALIR            << ". Salir"            << endl;
    cout << "----------------------------------------------------" << endl;

    int opcion = leerEnteroValidado("Seleccione una opcion: ", OPCION_ELIMINAR_FICHA);

    while(opcion > OPCION_SALIR){
        cout << "Opcion fuera de rango. Intente de nuevo." << endl;
        opcion = leerEnteroValidado("Seleccione una opcion: ", OPCION_ELIMINAR_FICHA);
    }

    return opcion;
}

// ============================================================
// Ayuda
// ============================================================
void mostrarAyuda(){
    cout << endl << "===================== AYUDA =====================" << endl;
    cout << OPCION_ELIMINAR_FICHA   << ": Elimina la ficha en la posicion (fila, columna) indicada." << endl;
    cout << OPCION_AGREGAR_FILA     << ": Agrega una fila nueva en la posicion indicada."            << endl;
    cout << OPCION_ELIMINAR_FILA    << ": Elimina la fila indicada."                                 << endl;
    cout << OPCION_AGREGAR_COLUMNA  << ": Agrega una columna nueva en la posicion indicada."          << endl;
    cout << OPCION_ELIMINAR_COLUMNA << ": Elimina la columna indicada."                               << endl;
    cout << OPCION_AYUDA            << ": Muestra este mensaje de ayuda."                             << endl;
    cout << OPCION_SALIR            << ": Sale del juego."                                            << endl;
    cout << "==================================================" << endl << endl;
}

// ============================================================
// Representación gráfica de una ficha
// ============================================================
// Tabla I del informe:
// 0 (000) vacio | 1..6 (001..110) fichas normales | 7 (111) eliminacion
const char* obtenerSimboloFicha(int valorFicha){
    switch(valorFicha){
    case 0: return " . ";  // Estado vacío
    case 1: return " β ";  // Beta
    case 2: return " Ω ";  // Omega
    case 3: return " Σ ";  // Sigma
    case 4: return " Γ ";  // Gamma
    case 5: return " θ ";  // Theta
    case 6: return " μ ";  // Mu
    case 7: return " X ";  // Estado Eliminación
    default: return " ? ";
    }
}


// ============================================================
// Impresión del tablero con fichas
// ============================================================
void imprimirTableroFichas(unsigned char* tablero, int filas, int columnas){
    int indiceByte, offset, valorFicha;

    cout << endl << "TABLERO DE JUEGO" << endl;

    // Encabezado con el indice de cada columna
    cout << "    ";
    for(int columna = 0; columna < columnas; columna++){
        cout << setw(3) << columna;
    }
    cout << endl;

    for(int fila = 0; fila < filas; fila++){
        cout << setw(3) << fila << " |";

        for(int columna = 0; columna < columnas; columna++){
            calcularBtye_desplazamiento(fila, columna, columnas, indiceByte, offset);
            valorFicha = leerFicha(tablero, indiceByte, offset);

            cout << setw(3) << obtenerSimboloFicha(valorFicha);
        }

        cout << endl;
    }

    cout << endl;
}

// ============================================================
// Impresión del tablero en binario (vista profesor - memoria real)
// ============================================================
void imprimirTableroBinario(unsigned char* tablero, int filas, int columnas, int* estadoJuego){
    cout << endl << "TABLERO EN BINARIO (vista profesor)" << endl;

    // EL CAMBIO CLAVE: Leemos la capacidad física total
    int bytesTotales = estadoJuego[2];

    for(int i = 0; i < bytesTotales; i++){
        // Alineación visual sencilla
        if (i < 10) cout << "Byte  " << i << ": ";
        else cout << "Byte " << i << ": ";

        for(int bit = 7; bit >= 0; bit--){
            cout << ((tablero[i] >> bit) & 1);
        }
        cout << endl;
    }
    cout << endl;
}

void imprimirEstadisticas(int* estadoJuego) {
    cout << "\n=================================================" << endl;
    cout << "           ESTADISTICAS DE LA PARTIDA            " << endl;
    cout << "=================================================" << endl;

    // 1. Dimensiones actuales del tablero
    cout << "Dimensiones del tablero: " << estadoJuego[0] << " filas x " << estadoJuego[1] << " columnas" << endl;

    // 2. Cantidad de eliminaciones realizadas por el usuario
    cout << "Jugadas del usuario:     " << estadoJuego[3] << endl;

    // 3. Cantidad de fichas eliminadas en total
    cout << "Fichas destruidas:       " << estadoJuego[4] << endl;

    // 4. Cantidad de combinaciones detectadas
    cout << "Combinaciones formadas:  " << estadoJuego[5] << endl;

    // 5. Cantidad de cascadas producidas
    cout << "Cascadas generadas:      " << estadoJuego[6] << endl;

    // 6. Puntuación obtenida
    cout << "PUNTUACION TOTAL:        " << estadoJuego[7] << endl;
    cout << "=================================================\n" << endl;
}
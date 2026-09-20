#include "logicaJuego.h"
#include "motorDeBits.h"
#include "interfazUsuario.h"
#include <cstdlib> // Para rand() y srand()
#include <ctime>   // Para time()
#include <iostream>

using namespace std;

void inicializarSemillaAleatoria() {
    // Usamos la hora actual del sistema operativo como semilla inicial
    srand(time(0));
}

void generarFichasAleatorias(unsigned char* tablero, int filas, int columnas) {
    int indiceByte, offset;

    for (int fila = 0; fila < filas; fila++) {
        for (int columna = 0; columna < columnas; columna++) {

            // Calculamos donde estamos parados
            calcularBtye_desplazamiento(fila, columna, columnas, indiceByte, offset);

            // Solo generamos si la posicion actual esta vacia (Estado Libre = 0)
            if (leerFicha(tablero, indiceByte, offset) == 0) {

                int fichaAleatoria;
                bool fichaValida;

                // Bucle de prevención: repetimos hasta encontrar una ficha que no forme linea de 3
                do {
                    fichaValida = true;
                    fichaAleatoria = (rand() % 6) + 1; // Genera un número del 1 al 6

                    // --- PREVENCIÓN HORIZONTAL (Mirar a la izquierda) ---
                    if (columna >= 2) {
                        int byteIzq1, offsetIzq1, byteIzq2, offsetIzq2;

                        calcularBtye_desplazamiento(fila, columna - 1, columnas, byteIzq1, offsetIzq1);
                        int fichaIzq1 = leerFicha(tablero, byteIzq1, offsetIzq1);

                        calcularBtye_desplazamiento(fila, columna - 2, columnas, byteIzq2, offsetIzq2);
                        int fichaIzq2 = leerFicha(tablero, byteIzq2, offsetIzq2);

                        // Si la que generamos es igual a las dos de la izquierda, es invalida
                        if (fichaAleatoria == fichaIzq1 && fichaAleatoria == fichaIzq2) {
                            fichaValida = false;
                        }
                    }

                    // --- PREVENCIÓN VERTICAL (Mirar hacia arriba) ---
                    // Solo revisamos si sigue siendo valida despues de la prueba horizontal
                    if (fichaValida && fila >= 2) {
                        int byteArr1, offsetArr1, byteArr2, offsetArr2;

                        calcularBtye_desplazamiento(fila - 1, columna, columnas, byteArr1, offsetArr1);
                        int fichaArr1 = leerFicha(tablero, byteArr1, offsetArr1);

                        calcularBtye_desplazamiento(fila - 2, columna, columnas, byteArr2, offsetArr2);
                        int fichaArr2 = leerFicha(tablero, byteArr2, offsetArr2);

                        // Si la que generamos es igual a las dos de arriba, es invalida
                        if (fichaAleatoria == fichaArr1 && fichaAleatoria == fichaArr2) {
                            fichaValida = false;
                        }
                    }

                } while (!fichaValida);

                // Si salio del bucle do-while, significa que la ficha es 100% segura. La escribimos.
                escribirFicha(tablero, indiceByte, offset, fichaAleatoria);
            }
        }
    }
}

void eliminarFicha(unsigned char* tablero, int* estadoJuego){
    int filas = estadoJuego[0];
    int columnas = estadoJuego[1];

    int filaEscogida = -1;
    int columnaEscogida = -1;
    bool coordenadaValida = false;

    // Validación estricta para evitar accesos fuera de los límites de la memoria
    while(!coordenadaValida){
        filaEscogida = leerEnteroValidado("Ingrese la fila: ",0);
        columnaEscogida = leerEnteroValidado("Ingrese la columna: ",0);

        if(filaEscogida < filas && columnaEscogida < columnas){
            coordenadaValida = true;
        }
        else{
            cout<<"Coordenada invalida, ingresa el valor de una fila maximo: "<<(filas - 1)<<" y una columna maximo: "<<(columnas - 1)<<endl;
        }
    }

    int indiceByte = 0;
    int offset = 0;
    //Calcula la posicion de la ficha
    calcularBtye_desplazamiento(filaEscogida,columnaEscogida,columnas,indiceByte,offset);

    // Sobrescribe la ficha con 0 y actualiza el contador de eliminaciones del usuario
    escribirFicha(tablero, indiceByte, offset, 0);
    estadoJuego[3] = estadoJuego[3] + 1;
    estadoJuego[4] = estadoJuego[4] + 1;
    cout << "Ficha ubicada en la posicion (" << filaEscogida << ", " << columnaEscogida << ") ha sido eliminada." << endl;
}

void gravedad(unsigned char *tablero, int* estadoJuego){
    int filas = estadoJuego[0];
    int columnas = estadoJuego[1];

    // Procesamos columna por columna
    for(int columna=0;columna<columnas;columna++){
        int filaEscribir=filas-1;   // Apunta al hueco más profundo disponible

        for(int filaLeer=filas-1; filaLeer >=0; filaLeer--){
            int byteLeer,offsetLeer;
            calcularBtye_desplazamiento(filaLeer,columna,columnas,byteLeer,offsetLeer);
            int fichaActual = leerFicha(tablero, byteLeer,offsetLeer);

            // Si encontramos una ficha válida (distinta de 0), la dejamos caer
            if(fichaActual!=0){
                // Solo movemos si la ficha no está ya en el fondo
                if(filaLeer!=filaEscribir){
                    int byteEscribir,offsetEscribir;
                    calcularBtye_desplazamiento(filaEscribir,columna,columnas,byteEscribir,offsetEscribir);

                    // Copiamos la ficha a su nueva posición y vaciamos la antigua
                    escribirFicha(tablero,byteEscribir,offsetEscribir,fichaActual);
                    escribirFicha(tablero,byteLeer,offsetLeer,0);
                }
                // El fondo útil sube un nivel
                filaEscribir--;
            }
        }
    }
}

void rellenar(unsigned char* tablero, int* estadoJuego) {
    int filas = estadoJuego[0];
    int columnas = estadoJuego[1];
    int indiceByte, offset;

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {

            calcularBtye_desplazamiento(f, c, columnas, indiceByte, offset);

            // Si encontramos un hueco (0)
            if (leerFicha(tablero, indiceByte, offset) == 0) {

                // Esto es lo que permite que caigan fichas haciendo cascadas
                int fichaAleatoria = (rand() % 6) + 1;
                escribirFicha(tablero, indiceByte, offset, fichaAleatoria);
            }
        }
    }
}

// 1. Función para buscar combinaciones (usa operadores de bits para marcar)
bool funcion_buscar_combinaciones(unsigned char* tablero, int* estadoJuego, unsigned char* mascaraFinal, unsigned char* mascaraH, unsigned char* mascaraV) {
    int filas = estadoJuego[0];
    int columnas = estadoJuego[1];
    bool huboMatch = false;

    // Búsqueda Horizontal
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas - 2; c++) { // Hasta columnas-2 para leer de a 3
            int b1, o1, b2, o2, b3, o3;

            // Leemos 3 fichas contiguas
            calcularBtye_desplazamiento(f, c, columnas, b1, o1);
            int ficha1 = leerFicha(tablero, b1, o1);
            if (ficha1 == 0) continue; // Ignorar espacios vacíos

            calcularBtye_desplazamiento(f, c+1, columnas, b2, o2);
            int ficha2 = leerFicha(tablero, b2, o2);

            calcularBtye_desplazamiento(f, c+2, columnas, b3, o3);
            int ficha3 = leerFicha(tablero, b3, o3);
            // Si hay 3 iguales
            if (ficha1 == ficha2 && ficha1 == ficha3) {
                huboMatch = true;

                // Calculamos índice lineal de cada una y activamos el bit con OR (|) y desplazamiento (<<)
                int idx1 = f * columnas + c;
                int idx2 = f * columnas + c + 1;
                int idx3 = f * columnas + c + 2;

                mascaraH[idx1 / 8] |= (1 << (idx1 % 8));
                mascaraH[idx2 / 8] |= (1 << (idx2 % 8));
                mascaraH[idx3 / 8] |= (1 << (idx3 % 8));
            }
        }
    }

    // Búsqueda Vertical
    for (int c = 0; c < columnas; c++) {
        for (int f = 0; f < filas - 2; f++) {
            int b1, o1, b2, o2, b3, o3;

            calcularBtye_desplazamiento(f, c, columnas, b1, o1);
            int ficha1 = leerFicha(tablero, b1, o1);
            if (ficha1 == 0) continue;

            calcularBtye_desplazamiento(f+1, c, columnas, b2, o2);
            int ficha2 = leerFicha(tablero, b2, o2);

            calcularBtye_desplazamiento(f+2, c, columnas, b3, o3);
            int ficha3 = leerFicha(tablero, b3, o3);

            if (ficha1 == ficha2 && ficha1 == ficha3) {
                huboMatch = true;

                int idx1 = f * columnas + c;
                int idx2 = (f+1) * columnas + c;
                int idx3 = (f+2) * columnas + c;

                mascaraV[idx1 / 8] |= (1 << (idx1 % 8));
                mascaraV[idx2 / 8] |= (1 << (idx2 % 8));
                mascaraV[idx3 / 8] |= (1 << (idx3 % 8));
            }
        }
    }

    // Combinar las máscaras (Horizontal OR Vertical)
    int bytesMascara = ((filas * columnas) + 7) / 8;
    for (int i = 0; i < bytesMascara; i++) {
        mascaraFinal[i] = mascaraH[i] | mascaraV[i];
    }

    return huboMatch;
}

// 2. Función para detectar cruces usando AND a nivel de bits
int funcion_detectar_cruces_simultaneos(unsigned char* mascaraH, unsigned char* mascaraV, int bytesMascara) {
    int crucesDetectados = 0;
    for (int i = 0; i < bytesMascara; i++) {
        // Un cruce ocurre si la misma ficha está en una línea Horizontal Y Vertical (&)
        unsigned char interseccion = mascaraH[i] & mascaraV[i];

        // Contamos cuántos bits encendidos hay en esta intersección
        for (int bit = 0; bit < 8; bit++) {
            if ((interseccion >> bit) & 1) {
                crucesDetectados++;
            }
        }
    }
    return crucesDetectados;
}

// 3. Función de Eliminación y regeneración
void funcion_eliminacion_por_combinacion(unsigned char* tablero, unsigned char* mascaraFinal, int* estadoJuego) {
    int filas = estadoJuego[0];
    int columnas = estadoJuego[1];

    // 1. Eliminar fichas validadas
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnas; c++) {
            int idx = f * columnas + c;
            int byteM = idx / 8;
            int offsetM = idx % 8;

            // Si el bit está encendido, reemplazamos por 0 (vacío)
            if ((mascaraFinal[byteM] >> offsetM) & 1) {
                int bT, oT;
                calcularBtye_desplazamiento(f, c, columnas, bT, oT);
                escribirFicha(tablero, bT, oT, 0);
                estadoJuego[4]++; // Aumentamos FICHAS_ELIMINADAS
                estadoJuego[7] += 10;
            }
        }
    }

    // 2. Aplicar gravedad
    gravedad(tablero, estadoJuego); //

    // 3. Generar nuevas fichas
    rellenar(tablero, estadoJuego);
}

// 4. Función directora de cascadas
void funcion_cascadas(unsigned char* tablero, int* estadoJuego) {
    int filas = estadoJuego[0];
    int columnas = estadoJuego[1];

    // Calcular tamaño de la memoria dinámica necesaria
    int bytesMascara = ((filas * columnas) + 7) / 8;

    // Asignación de MEMORIA DINÁMICA
    unsigned char* mascaraH = new unsigned char[bytesMascara]();
    unsigned char* mascaraV = new unsigned char[bytesMascara]();
    unsigned char* mascaraFinal = new unsigned char[bytesMascara]();

    bool hayCombinaciones = false;
    int iteracionCascada = 0;

    do {
        // Limpiar la memoria para la iteración actual
        for (int i = 0; i < bytesMascara; i++) {
            mascaraH[i] = 0; mascaraV[i] = 0; mascaraFinal[i] = 0;
        }

        hayCombinaciones = funcion_buscar_combinaciones(tablero, estadoJuego, mascaraFinal, mascaraH, mascaraV);

        if (hayCombinaciones) {
            iteracionCascada++;
            estadoJuego[5]++; // Aumentar COMBOS
            estadoJuego[7] += (iteracionCascada * 50);

            // Detectar cruces T o L simultáneos
            int cruces = funcion_detectar_cruces_simultaneos(mascaraH, mascaraV, bytesMascara);
            if (cruces > 0) {
                cout << "\n>>> ¡CRUCE T/L DETECTADO! (" << cruces << " intersecciones) <<<" << endl;
                estadoJuego[7] += (cruces * 100); // Puntos extra por cruces
            }

            funcion_eliminacion_por_combinacion(tablero, mascaraFinal, estadoJuego);
            estadoJuego[6]++; // Aumentar CASCADAS totales
        }
    } while (hayCombinaciones);

    // Liberar la MEMORIA DINÁMICA
    delete[] mascaraH;
    delete[] mascaraV;
    delete[] mascaraFinal;
}


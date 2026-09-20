#include "manejoMemoria.h"
#include<iostream>
#include "interfazUsuario.h"
#include "motorDeBits.h"

using namespace std;

const int FILAS = 0;
const int COLUMNAS = 1;
const int BYTESRESERVADOS = 2;

unsigned char* inicializarTablero(int *estadoJuego){
    int filas = estadoJuego[FILAS];
    int columnas = estadoJuego[COLUMNAS];

    // Calcula la cantidad exacta de bytes necesarios para empaquetar 3 bits por ficha
    int bitsTotales = 3 * filas * columnas;
    int bytesNecesitados = (bitsTotales + 7) / 8;

    // Reserva memoria dinámica inicializada en ceros
    unsigned char* tablero = new unsigned char[bytesNecesitados]();

    estadoJuego[BYTESRESERVADOS] = bytesNecesitados;

    return tablero;
}

unsigned char* agregarFila(unsigned char* tableroViejo, int* estadoJuego){
    int filasAnteriores = estadoJuego[0];
    int columnas = estadoJuego[1];

    int posicion = -1;
    bool coordenadaValida = false;

    // Validación de límites
    while(!coordenadaValida){
        posicion = leerEnteroValidado("Ingresa la posicion donde quieres agregar la fila: ",0);
        if(posicion<=filasAnteriores){
            coordenadaValida= true;
        }
        else{
            cout<<"Coordenada no valida, la posicion maxima es: "<<filasAnteriores<<endl;
        }
    }

    int filasNuevas = filasAnteriores+1;
    int bytesNuevos = ((3*filasNuevas*columnas)+7)/8;

    // Creación del nuevo bloque de memoria ampliado
    unsigned char* tableroNuevo = new unsigned char[bytesNuevos]();

    // Mudanza: recorremos la nueva memoria celda por celda
    for(int f=0;f<filasNuevas;f++){
        for(int c=0;c<columnas;c++){
            int byteNuevo, offsetNuevo;
            calcularBtye_desplazamiento(f,c,columnas,byteNuevo,offsetNuevo);

            if(f==posicion){
                // Fila nueva: se inyectan fichas al azar
                int fichaAleatoria = (rand()%6)+1;
                escribirFicha(tableroNuevo,byteNuevo,offsetNuevo,fichaAleatoria);
            }
            else if(f<posicion){
                // Filas superiores: se copian exactamente igual
                int byteViejo, offsetViejo;
                calcularBtye_desplazamiento(f,c,columnas,byteViejo,offsetViejo);

                int fichaVieja= leerFicha(tableroViejo,byteViejo,offsetViejo);
                escribirFicha(tableroNuevo,byteNuevo,offsetNuevo,fichaVieja);
            }
            else{
                // Filas inferiores: leen la información desplazada un índice hacia arriba (f-1)
                int byteViejo, offsetViejo;
                calcularBtye_desplazamiento(f-1,c,columnas,byteViejo,offsetViejo);

                int fichaVieja= leerFicha(tableroViejo,byteViejo,offsetViejo);
                escribirFicha(tableroNuevo,byteNuevo,offsetNuevo,fichaVieja);
            }
        }
    }

    // Liberamos la memoria antigua para evitar fugas
    delete[] tableroViejo;

    estadoJuego[0] = filasNuevas;
    estadoJuego[2] = bytesNuevos;
    estadoJuego[3] = estadoJuego[3] + 1;

    cout<<"La fila se agrego con exito"<<endl;
    cout<<"Nuevos bytes reservados: "<<estadoJuego[2];
    return tableroNuevo;
}

unsigned char* eliminarFila(unsigned char* tablero, int* estadoJuego){
    int filasAnteriores=estadoJuego[0];
    int columnas = estadoJuego[1];
    int bytesResActuales=estadoJuego[2];

    // Evita la destrucción total del tablero
    if(filasAnteriores<=1){
        cout<<"Es la ultima fila, no se puede eliminar";
        return tablero;
    }

    int posicion = -1;
    bool coordenadaValida = false;

    // Validación de límites
    while(!coordenadaValida){
        posicion = leerEnteroValidado("Ingresa la posicion donde quieres eliminar la fila: ",0);
        if(posicion<=filasAnteriores){
            coordenadaValida= true;
        }
        else{
            cout<<"Coordenada no valida, la posicion maxima es: "<<filasAnteriores<<endl;
        }
    }

    // Aplasta la fila eliminada copiando las filas de abajo hacia arriba
    for(int f=posicion;f<filasAnteriores-1;f++){
        for(int c=0;c<columnas;c++){
            int byteDest,offsetDest,byteOrigen,offsetOrigen;

            calcularBtye_desplazamiento(f,c,columnas,byteDest,offsetDest);
            calcularBtye_desplazamiento(f+1,c,columnas,byteOrigen,offsetOrigen);

            int ficha=leerFicha(tablero,byteOrigen,offsetOrigen);
            escribirFicha(tablero,byteDest,offsetDest,ficha);
        }
    }

    int filasNuevas=filasAnteriores-1;
    int bytesNecesitados=((3*filasNuevas*columnas)+7)/8;
    int usoMemoria=(bytesNecesitados*100)/bytesResActuales;

    unsigned char* tableroFinal=tablero;

    cout<<"Memoria requerida: "<<bytesNecesitados<<endl;
    cout<<"Porcentaje: "<<usoMemoria<<"%"<<endl;

    // Regla de optimización: Solo reasigna memoria física si cae bajo el 65%
    if(usoMemoria<65){
        cout<<"El uso es menor al 65%, por esto se reduce la memoria"<<endl;

        unsigned char* tableroNuevo=new unsigned char[bytesNecesitados]();

        // Extrae solo la información útil hacia la nueva memoria compacta
        for(int f=0;f<filasNuevas;f++){
            for(int c=0;c<columnas;c++){
                int byteNuevo,offsetNuevo;
                calcularBtye_desplazamiento(f,c,columnas,byteNuevo,offsetNuevo);

                int ficha=leerFicha(tablero,byteNuevo, offsetNuevo);
                escribirFicha(tableroNuevo,byteNuevo,offsetNuevo,ficha);
            }
        }
        // Liberamos la memoria antigua para evitar fugas
        delete[] tablero;

        tableroFinal=tableroNuevo;
        estadoJuego[2]=bytesNecesitados;
    }
    else{
        cout<<"Se conserva la memoria";
    }

    estadoJuego[0]=filasNuevas;
    estadoJuego[3] = estadoJuego[3] + 1;
    cout<<"La Fila se elimino con exito"<<endl;
    return tableroFinal;
}

unsigned char* agregarColumna(unsigned char* tableroViejo, int* estadoJuego){
    int filas = estadoJuego[0];
    int columnasViejas = estadoJuego[1];

    int posicion = -1;
    bool coordenadaValida = false;
    // Validación de límites
    while (!coordenadaValida) {
        posicion = leerEnteroValidado("Ingresa la posicion donde quieres agregar la nueva columna: ", 0);
        if (posicion <= columnasViejas) {
            coordenadaValida = true;
        } else {
            cout<<"Coordenada no valida: La posicion maxima es "<<columnasViejas<< endl;
        }
    }

    int columnasNuevas = columnasViejas + 1;
    int nuevosBytes = ((3 * filas * columnasNuevas) + 7) / 8;

    // Mudanza: agregar una columna altera el alineamiento de todos los bits
    unsigned char* tableroNuevo = new unsigned char[nuevosBytes]();

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < columnasNuevas; c++) {

            int byteNuevo, offsetNuevo;
            calcularBtye_desplazamiento(f, c, columnasNuevas, byteNuevo, offsetNuevo);

            if (c == posicion) {
                // Columna nueva: genera valores aleatorios
                int fichaAleatoria = (rand() % 6) + 1;
                escribirFicha(tableroNuevo, byteNuevo, offsetNuevo, fichaAleatoria);

            } else if (c < posicion) {
                // Lado izquierdo: la lectura usa la base de columnas viejas intacta
                int byteViejo, offsetViejo;
                calcularBtye_desplazamiento(f, c, columnasViejas, byteViejo, offsetViejo);

                int fichaVieja = leerFicha(tableroViejo, byteViejo, offsetViejo);
                escribirFicha(tableroNuevo, byteNuevo, offsetNuevo, fichaVieja);

            } else {
                // Lado derecho: lee la información retrasada una columna (c - 1)
                int byteViejo, offsetViejo;
                calcularBtye_desplazamiento(f, c - 1, columnasViejas, byteViejo, offsetViejo);

                int fichaVieja = leerFicha(tableroViejo, byteViejo, offsetViejo);
                escribirFicha(tableroNuevo, byteNuevo, offsetNuevo, fichaVieja);
            }
        }
    }
    // Liberamos la memoria antigua para evitar fugas
    delete[] tableroViejo;

    estadoJuego[1] = columnasNuevas;
    estadoJuego[2] = nuevosBytes;
    estadoJuego[3] = estadoJuego[3] + 1;

    cout<< "Columna agregada exitosamente en la posicion "<< posicion<< endl;

    return tableroNuevo;
}

unsigned char* eliminarColumna(unsigned char* tablero, int* estadoJuego){
    int filas = estadoJuego[0];
    int columnasViejas = estadoJuego[1];
    int bytesResActuales = estadoJuego[2];

    // Evita la destrucción total del tablero
    if (columnasViejas <= 1) {
        cout << "No puedes eliminar la ultima columna." << endl;
        return tablero;
    }

    int posicion = -1;
    bool coordenadaValida = false;
    // Validación de límites
    while (!coordenadaValida) {
        posicion = leerEnteroValidado("Ingrese la posicion donde quieres eliminar la columna: ", 0);
        if (posicion < columnasViejas) {
            coordenadaValida = true;
        } else {
            cout << "Coordenada no valida: La posicion maxima es " << (columnasViejas - 1) << "." << endl;
        }
    }

    int nuevasColumnas = columnasViejas - 1;

    // Compactación en sitio: sobreescribe la memoria desplazando los bits a la izquierda
    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < nuevasColumnas; c++) {

            // Se salta la columna eliminada al leer del origen
            int c_origen = (c >= posicion) ? (c + 1) : c;

            int byteOrigen, offsetOrigen;
            calcularBtye_desplazamiento(f, c_origen, columnasViejas, byteOrigen, offsetOrigen);
            int ficha = leerFicha(tablero, byteOrigen, offsetOrigen);

            int byteDestino, offsetDestino;
            calcularBtye_desplazamiento(f, c, nuevasColumnas, byteDestino, offsetDestino);
            escribirFicha(tablero, byteDestino, offsetDestino, ficha);
        }
    }

    int bytesUtilesRequeridos = ((3 * filas * nuevasColumnas) + 7) / 8;
    int porcentajeUso = (bytesUtilesRequeridos * 100) / bytesResActuales;

    unsigned char* tableroFinal = tablero;

    cout << "Memoria util requerida: " << bytesUtilesRequeridos << endl;
    cout << "Porcentaje de uso de memoria: " << porcentajeUso << "%" << endl;

    // Regla del 65% para reducción horizontal
    if (porcentajeUso < 65) {
        cout << "El uso es menor al 65%, por esto se reduce la memoria" << endl;

        unsigned char* tableroNuevo = new unsigned char[bytesUtilesRequeridos]();

        for (int f = 0; f < filas; f++) {
            for (int c = 0; c < nuevasColumnas; c++) {
                int b, o;
                calcularBtye_desplazamiento(f, c, nuevasColumnas, b, o);
                int ficha = leerFicha(tablero, b, o);
                escribirFicha(tableroNuevo, b, o, ficha);
            }
        }
        // Liberamos la memoria antigua para evitar fugas
        delete[] tablero;
        tableroFinal = tableroNuevo;
        estadoJuego[2] = bytesUtilesRequeridos;

    } else {
        cout << "Se conserva la memoria" << endl;
    }

    estadoJuego[1] = nuevasColumnas;
    estadoJuego[3] = estadoJuego[3] + 1;
    cout << "Columna " << posicion << " eliminada con exito." << endl;

    return tableroFinal;
}
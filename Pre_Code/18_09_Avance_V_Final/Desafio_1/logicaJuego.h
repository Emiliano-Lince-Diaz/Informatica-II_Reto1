#ifndef LOGICAJUEGO_H
#define LOGICAJUEGO_H

// Inicializa el reloj interno para que los números sean realmente aleatorios cada partida
void inicializarSemillaAleatoria();

// Recorre el tablero llenando los vacíos sin crear combinaciones
void generarFichasAleatorias(unsigned char* tablero, int filas, int columnas);

// Pide coordenadas al usuario, las valida y elimina la ficha seleccionada
void eliminarFicha(unsigned char* tablero, int* estadoJuego);

// Desplaza las fichas hacia abajo para ocupar los espacios vacíos tras una eliminación
void gravedad(unsigned char* tablero, int* estadoJuego);

// Rellena los huecos superiores con fichas al azar, permitiendo generar cascadas
void rellenar(unsigned char* tablero, int* estadoJuego);

// Recorre el tablero y marca en las máscaras de bits las fichas que forman líneas de 3 o más.
bool funcion_buscar_combinaciones(unsigned char* tablero, int* estadoJuego, unsigned char* mascaraFinal, unsigned char* mascaraH, unsigned char* mascaraV);

// Compara las máscaras horizontales y verticales (mediante AND) para encontrar cruces (Formas L o T).
int funcion_detectar_cruces_simultaneos(unsigned char* mascaraH, unsigned char* mascaraV, int bytesMascara);

// Usa la máscara final para borrar (poner en 0) las fichas que hicieron match y llama a la gravedad.
void funcion_eliminacion_por_combinacion(unsigned char* tablero, unsigned char* mascaraFinal, int* estadoJuego);

// Función directora (bucle): busca combinaciones, las elimina, aplica gravedad y repite si hay reacciones en cadena.
void funcion_cascadas(unsigned char* tablero, int* estadoJuego);

#endif // LOGICAJUEGO_H
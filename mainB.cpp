/******************************************************************************\
 * Programación 2. Trabajo obligatorio curso 2021-22
 * Autores: Álvaro de Francisco (838819)
 * Ultima revisión: 26-05-2022
 * Resumen: Fichero principal donde se encuentra la función main para realizar
 *  backtracking sobre el juego de tetris.
\******************************************************************************/

#include <iostream>
#include <iomanip>
#include <cstdlib>

#include "./betris.hpp"

using namespace std;

// Pre: Argumentos al llamar la función:
//          nfils ncols objetivo retardo N1 N2 N3 N4 N5 N6 N7 N8 ... -1
//          nfils ncols objetivo retardo -N1
// Post: Utilizando el esquema algorítmico Backtracking resolvemos "objetivo" filas de Tetris
//       utilizando las piezas dadas al llamar la función, o si N1 es negativo generadas aleatoriamente
//       en un tablero con dimensiones nfils y ncols, mostrando en pantalla el proceso y el resultado final.
int main(int argc, char *argv[])
{
    if (argc > ARGS_ANTES_DE_N1)
    {
        // inicializacion tablero
        tpTablero tablero = {
            atoi(argv[1]),
            atoi(argv[2]),
        };
        inicializarTablero(tablero);

        // numero de filas que se deben completar
        int objetivo = atoi(argv[3]);
        
        if (objetivo > tablero.nfils)
        {
            cerr << "ERROR. El objetivo no puede ser mayor que el número de filas" << endl;
            return 1;
        }

        // numero de milisegundos que se debe esperar tras mostrar el estado del tablero,
        // antes de mostrarlo en su estado siguiente.
        int retardo = atoi(argv[4]);

        int vEntrada[MAXENTRADA];
        int vSalida[MAXENTRADA];

        int i = 0;
        int pieza = atoi(argv[i + ARGS_ANTES_DE_N1]);

        // Si la primera pieza es un número negativo, generamos piezas aleatorias
        if (pieza <= -1)
        { 
            generarPiezas(vEntrada, vSalida, -pieza);
        }
        else
        {
            while (pieza != -1)
            {
                vEntrada[i] = pieza;
                vSalida[i] = -1;
                i++;
                pieza = atoi(argv[i + ARGS_ANTES_DE_N1]);
            }
            // insertamos un -1 al final para denotar que no hay más piezas en vEntrada
            vEntrada[i] = -1;
        }

        inicializarTablero(tablero);

        // Comenzamos el backtracking
        int resul = buscaSolucion(tablero, vEntrada, vSalida, objetivo, 0, retardo);

        mostrarResultados(tablero, vEntrada, vSalida, resul);

        return 0;
    }
    else
    {
        cerr << "ERROR: Faltan variables por asignar." << endl;
        return 1;
    }
}

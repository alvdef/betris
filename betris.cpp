/******************************************************************************\
 * Programación 2. Trabajo obligatorio curso 2021-22
 * Autores: Álvaro de Francisco (838819)
 * Ultima revisión: 26-05-2022
 * Resumen: Fichero de implementación «betris.cpp».
\******************************************************************************/

#include <iomanip>
#include <string>
#include <unistd.h>
#include <ctime>

#include "betris.hpp"

using namespace std;

// Pre: true
// Post: Todas las componentes de tablero.matriz son -1 (casilla vacía)
void inicializarTablero(tpTablero &tablero)
{   
    for (int i = 0; i < tablero.nfils; i++)
    {
        for (int j = 0; j < tablero.ncols; j++)
        {
            tablero.matriz[i][j] = -1;
        }
    }
}


// Pre: 0 < nPiezas < MAXENTRADA-1
// Post: vEntrada contiene nPiezas generadas de forma aleatoria con valores entre 0 y PIEZASDEF
//       El último valor es -1
void generarPiezas(int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA], const int nPiezas)
{
    time_t semilla = time(nullptr);
    srand(semilla);
    for (int i = 0; i < nPiezas; i++)
    {
        vEntrada[i] = rand() % PIEZASDEF;
        vSalida[i] = -1;
    }
    vEntrada[nPiezas] = -1;
    vSalida[nPiezas] = -1;
}


// Pre: tablero, vEntrada, vSalida contienen los resultados después de hacer el backtracking
//      resul contiene el número de piezas colocadas, y si no hay solución -1.
// Post: Ha mostrado en pantalla los resultados.
void mostrarResultados(const tpTablero tablero, const int vEntrada[MAXENTRADA], const int vSalida[MAXENTRADA], const int resul)
{
    mostrarTablero(tablero, vEntrada);
    cout << "Entrada (números de piezas): ";
    int e = 0;
    while (vEntrada[e] != -1)
    {
        cout << vEntrada[e] << " ";
        e++;
    }
    cout << "\nNúmero de piezas colocadas: " << resul << endl;
    cout << "Salida (números de columnas): ";
    if (resul != -1)
    {
        for (int i = 0; i < resul; i++)
        { 
            cout << vSalida[i] << " ";
        }
    }
    cout << endl;
}


// Pre: vEntrada contiene los números de las piezas que entran (0 <= número < PIEZASDEF) y tablero contiene el estado actual
//      del tablero, en el que pueden estar colocados los índices de vEntrada de algunas de dichas piezas, y si está vacía la casilla -1.
// Post: Se ha mostrado el tablero por pantalla (cada pieza se ha dibujado con su color)
void mostrarTablero(const tpTablero &tablero, const int vEntrada[MAXENTRADA])
{
    system("clear");
    for (int i = 0; i < tablero.nfils; i++)
    {
        for (int j = 0; j < tablero.ncols; j++)
        {
            int indiceEntradaActual = tablero.matriz[i][j];
            if (indiceEntradaActual != -1)
            {
                int colorActual = vPiezas[vEntrada[indiceEntradaActual]].color;
                string cadenaAtributos = "\033[" + to_string(colorActual) + "m";
                cout << cadenaAtributos << " ";
            }
            else
            {
                cout << "\033[0m"
                     << " "; 
            }
        }
        cout << "\033[0m" << endl;
    }
}


// Pre: "pieza" contiene las coordenadas relativas a la esquina izquierda de la pieza a escribir.
//      0≤"n"<MAXENTRADA ∧ 0≤"fila"<tablero.nfils ∧ 0≤"cols"<"tablero.ncols".
// Post: Para cada componente de la pieza, se ha escrito el índice de esta en las casillas de
//       "tablero.matriz" relativas a "fil" y "col".
void escribirPieza(tpTablero &tablero, const tpPieza pieza, const int n, const int fila, const int col)
{
    for (int i = 0; i < TAMPIEZA; i++)
    {
        int filaComp_i = fila + pieza.forma[i][0];
        int colComp_i = col + pieza.forma[i][1];

        tablero.matriz[filaComp_i][colComp_i] = n;
    }
}


// Pre: "pieza" contiene las coordenadas relativas a la esquina izquierda de la pieza.
//      0≤"n"<MAXENTRADA ∧ 0≤"fila"<tablero.nfils ∧ 0≤"cols"<"tablero.ncols".
// Post: Para cada componente de la pieza, se ha sustituido el índice de esta en las casillas de
//       "tablero.matriz" relativas a "fil" y "col" por -1 (casilla vacía).
void quitarPieza(tpTablero &tablero, const tpPieza pieza, const int fila, const int col)
{
    for (int i = 0; i < TAMPIEZA; i++)
    {
        int filaComp_i = fila + pieza.forma[i][0];
        int colComp_i = col + pieza.forma[i][1];

        tablero.matriz[filaComp_i][colComp_i] = -1;
    }
}


// Pre: "tablero" contiene "nfils" y "ncols" del juego, así como su tablero.
//      "objetivo" es un entero que representa las filas completas que ha de
//      tener el tablero para ser solución.
// Post: "filasResueltas">="objetivo" -→ true
//       ∧ "filasResueltas"<"objetivo" -→ false
bool esSolucion(const tpTablero &tablero, const int objetivo)
{
    int filasResueltas = 0;
    // comenzamos a comprobar desde abajo
    int i = tablero.nfils - 1;

    // desde abajo ((0,0) está arriba) hasta la posicion 0
    while ((i >= 0) && (filasResueltas < objetivo))
    {
        int j = 0;
        bool resuelta = true;
        while (resuelta && (j < tablero.ncols))
        {
            if (tablero.matriz[i][j] == -1)
            { // la fila no esta resuelta
                resuelta = false;
            }
            else if (j == tablero.ncols - 1)
            { // hemos llegado al final de la fila
                filasResueltas++;
                resuelta = false; // salimos del bucle y aumentamos la cuenta de filas resueltas
            }
            else
            { 
                j++;
            }
        }
        i--;
    }
    return filasResueltas >= objetivo;
    // comprobamos si el motivo por el que ha salido del bucle ha sido la cond. 1 o la 2
}


// Pre: "tablero" contiene "nfils" y "ncols" del juego, así como su tablero.
//      "pieza" contiene su forma y las coordenadas relativas a la esquina izquierda de la pieza (fila, columna).
//      0≤"fila"<tablero.nfils ∧ 0≤"columna"<"tablero.ncols".
// Post: Devuelve "true" si ninguna componente de la pieza se sale de los límites del tablero
//       y todas las posiciones por encima suyo y en el lugar de caída están libres.
//       En caso contrario devuelve "false".
bool cabePieza(tpTablero &tablero, const tpPieza pieza, const int fila, const int columna)
{
    int i = 0;
    bool cabe = true;
    int colComp_i_anterior = -1; // Inicializamos el valor de la columna anterior para comprobar que no haya otra pieza por encima
    while ((i < TAMPIEZA) && cabe)
    {
        // toma como referencia "fila" y "columna" y accede a las componentes de la pieza
        int filaComp_i = fila + pieza.forma[i][0]; 
        int colComp_i = columna + pieza.forma[i][1];

        // comprobamos que no existe ninguna pieza colocada previamente que interfiera con la caida de la actual 
        if (colComp_i != colComp_i_anterior)
        { // Si la columna es distinta a la anterior (para no comprobar varias veces la misma columna)
            int f = 0;
            while (f < filaComp_i)
            { // Iteramos desde arriba hasta la fila a la que vamos a poner la pieza
                if (tablero.matriz[f][colComp_i] != -1)
                { // Comprobando que no haya ninguna pieza ya colocada por encima
                    cabe = false;
                }
                f++;
            }
        }
        colComp_i_anterior = colComp_i; // Actualizamos la columna anterior

        // Condicion de dominio: la componente a la que intenta acceder forma parte del tablero
        if ((filaComp_i > tablero.nfils - 1) || (colComp_i > tablero.ncols - 1))
        {
            cabe = false;
        }
        // La posición en la que se está intentando colocar, está ocupada por otra pieza
        else if (tablero.matriz[filaComp_i][colComp_i] != -1)
        {
            cabe = false;
        }
        else
        {
            i++;
        }
    }
    return cabe;
}


// Pre: "tablero" contiene "nfils" y "ncols" del juego, así como su tablero.
//      "pieza" contiene su forma y las coordenadas relativas a la esquina izquierda de la pieza.
//      0≤"col"<"tablero.ncols".
// Post: Ha devuelto la fila más baja en la que se puede colocar la esquina izquierda de "pieza" en la columna "col".
int menorFila(tpTablero &tablero, tpPieza pieza, int col)
{
    // comenzamos comprobando desde abajo
    int fil = tablero.nfils - 1;
    bool cabe = false;
    while ((fil >= 0) && !cabe)
    {
        if (cabePieza(tablero, pieza, fil, col))
        { // rompemos el bucle, ya que cabe
            cabe = true;
        }
        else
        { // seguimos comprobando con la fila siguiente
            fil--;
        }
    }
    return fil;
}


// Pre: "retardo" contiene en milisegundos el tiempo de espera deseado
//      "tablero" contiene "nfils" y "ncols" del juego, así como su tablero con sus respectivos índices de "vEntrada".
// Post: Ha mostrado el tablero en salida estándar y espera "retardo" milisegundos
void retarda(const int retardo, tpTablero &tablero, const int vEntrada[MAXENTRADA])
{
    // si el retardo es menor que 0, solo muestra el resultado final 
    if (retardo > 0)
    {
        mostrarTablero(tablero, vEntrada);
        // usleep hace un delay de microsegundos, por lo que multiplicamos el retardo (ms) por 1000
        usleep(retardo * 1000);
    }
}


// Pre:  En el tablero se han colocada las n primeras piezas de vEntrada, en la columnas indicadas
//       respectivamente en vSalida
// Post: Si las piezas colocadas completan al menos objetivo filas sin huecos, entonces devuelve
//       el número de piezas colocadas, en vSalida las columnas en las que se han colocado las
//       piezas y el tablero con las piezas colocadas; si no hay solución devuelve -1
int buscaSolucion(tpTablero &tablero, const int vEntrada[MAXENTRADA], int vSalida[MAXENTRADA],
                  const int objetivo, int n, const int retardo)
{
    // CASO BASE: el tablero creado recursivamente es solución al problema
    // propuesto en la llamada al programa
    if (esSolucion(tablero, objetivo))
    {
        // devuelve el número de piezas colocadas
        return n;
    }
    // CASO BASE: se han acabado las piezas a colocar
    else if (vEntrada[n] == -1)
    {
        return -1;
    }
    // CASO RECURSIVO
    else
    {
        int nColocadas = -1;
        int col = 0;
        bool esSol = false;
        while ((col < tablero.ncols) && !esSol)
        { // Mientras no sea una solución iteramos por las columnas hasta encontrarnos con algún caso base
            tpPieza pieza = vPiezas[vEntrada[n]];
            int fila = menorFila(tablero, pieza, col); // Buscamos la menor fila. Si devuelve -1, no se puede colocar, pasamos a la sig
            if (fila != -1)
            { // Si la posición de la pieza en (fila, col) es válida, la añadimos al tablero y la mostramos en pantalla
                vSalida[n] = col;
                escribirPieza(tablero, pieza, n, fila, col);

                retarda(retardo, tablero, vEntrada);

                // Llamada recursiva a la función para probar a colocar la siguiente pieza o comprobar si es un caso base
                nColocadas = buscaSolucion(tablero, vEntrada, vSalida, objetivo, n + 1, retardo);
                if (nColocadas == -1)
                { // Si se han acabado las piezas sin haber encontrado solución o no hay ninguna posición válida, quitamos la pieza
                    quitarPieza(tablero, pieza, fila, col);
                    vSalida[n] = -1;
                }
                else
                { // Si se ha encontrado una solución, cortocircuita el backtracking para devolver el número de piezas colocadas
                    esSol = true;
                }
            }
            col++;
        }
        return nColocadas;
    }
}
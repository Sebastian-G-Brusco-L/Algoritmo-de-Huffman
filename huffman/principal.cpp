#include <iostream>
#include "C:TADstrings\funciones\strings.hpp"
#include "comprimir.hpp"
#include "descomprimir.hpp"

using namespace std;

void comprimir(string fName);

void descomprimir(string fName);

int main(int argc, char **argv) // recibe por linea de comandos argumentos
{
    // recibo el nombre del archivo
    // string fName = argv[1];
    string fName = "fotov2.bmp.huf";

    // si no termina con .huf comprimo
    if (!endsWith(fName, ".huf"))
    {
        cout << "Comprimiendo.." << endl;
        comprimir(fName);
    }
    else // descomprimo
    {
        cout << "Descomprimiendo.." << endl;
        descomprimir(fName);
    }

    return 0;
}

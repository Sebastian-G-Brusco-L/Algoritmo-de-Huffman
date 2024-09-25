#include "BitReader.hpp"
#include "BitWriter.hpp"
#include "C:TADarray\Array.hpp"
#include "C:TADfile\funciones\files.hpp"
#include "HuffmanSetup.hpp"

bool esHoja(HuffmanTreeInfo *nodo) // si es hoja true
{
    return nodo->left == nullptr && nodo->right == nullptr;
}

void descompresionDelArchivo(HuffmanTreeInfo *raiz, unsigned int tamanioArch, FILE *f, string fName)
{
    string aux;
    int largo = fName.length();
    for (int i = 0; i < largo - 4; i++) // le saco el .huf
    {
        aux += fName[i];
    }

    FILE *fDesc = fopen((aux ).c_str(), "w+b");
    if (fDesc == nullptr)
    {
        cout << "Error al abrir el archivo" << endl;
        exit(0);
    }

    BitReader br = bitReader(f); // inicializo bitReader

    for (int i = 0; i < tamanioArch; i++) // leo hasta su tamanio por el bitwritterFlush
    {

        HuffmanTreeInfo *aux = raiz; // vuelvo a la raiz

        while (!esHoja(aux)) // mientras que no sea hoja sigo leyendo bits
        {
            int bit = bitReaderRead(br); // leo primer bit

            if (bit == 1) // me muevo derecha
            {
                if (esHoja(aux->right))
                {
                    aux = aux->right;                            // actualizo aux para que apunte a la hoja
                    unsigned char c = aux->c;                    // obtengo el char y lo escribo en un nuevo archivo
                    fwrite(&c, sizeof(unsigned char), 1, fDesc); // escribo en el nuevo archivo
                }
                else
                {
                    aux = aux->right; // actualizo aux para seguir bajando los nodos
                }
            }
            else if (bit == 0) // me muevo izquierda
            {
                if (esHoja(aux->left))
                {
                    aux = aux->left;
                    unsigned char c = aux->c;
                    fwrite(&c, sizeof(unsigned char), 1, fDesc);
                }
                else
                {
                    aux = aux->left;
                }
            }
        }
    }

    fclose(fDesc);
}

HuffmanTreeInfo *recomponerArbol(string fName)
{
    FILE *f = fopen((fName).c_str(), "r+b"); // ya viene con .huf

    if (f == nullptr)
    {
        cout << "Error al abrir el archivo" << endl;
        exit(0);
    }

    HuffmanTreeInfo *raiz = new HuffmanTreeInfo(); // reservo memoria para la raiz

    unsigned char c;
    fread(&c, sizeof(unsigned char), 1, f); // primer byte con la cantidad de registros del encabezado
    int cantRegistros = c;

    if (cantRegistros == 255) // en caso de tener todos los ascii activos, que recorra toda la tabla
    {
        cantRegistros = 256;
    }

    for (int i = 0; i < cantRegistros; i++) // recorro los registros
    {
        fread(&c, sizeof(unsigned char), 1, f);
        unsigned char hoja = c;

        fread(&c, sizeof(unsigned char), 1, f);
        int longitudCod = c;

        HuffmanTreeInfo *aux = raiz; // comienza desde la raiz cada vez

        for (int j = 0; j < longitudCod; j++) // recorro el cod
        {
            fread(&c, sizeof(unsigned char), 1, f); // leo 0 o 1
            char bit = c;

            if (bit == '1') // me muevo a la derecha
            {
                if (aux->right == nullptr) // si aux der es null creo nuevo nodo a su derecha
                {
                    aux->right = new HuffmanTreeInfo();
                }
                aux = aux->right; // actualizo aux para que apunte al nodo creado
            }
            else if (bit == '0') // me muevo a la izquierda
            {
                if (aux->left == nullptr) // si aux izq es null creo nuevo nodo a su izq
                {
                    aux->left = new HuffmanTreeInfo();
                }
                aux = aux->left; // actualizo aux al nodo creado
            }
        }

        //  si es hoja almaceno su caracter
        if (esHoja(aux))
        {
            aux->c = (unsigned int)hoja;
        }
    }

  /*   for (int i = 0; i < sizeof(unsigned int); i++) // para ver si leia bien el unsigned int
    {
        fread(&c, sizeof(unsigned char), 1, f);
        cout << static_cast<int>(c) << " ";
    } */

    unsigned int tamanioArch;
    fread(&tamanioArch, sizeof(unsigned int), 1, f); // leo 4 bytes del unsiged int que representa el tamanio del arch original
    cout << tamanioArch;

    descompresionDelArchivo(raiz, tamanioArch, f, fName); // descomprimo post encabezado

    fclose(f);

    return raiz;
}

void descomprimir(string fName)
{
    HuffmanTreeInfo *raiz = recomponerArbol(fName); // return raiz solo para mostrarlo
    mostrarArbol(raiz);
}

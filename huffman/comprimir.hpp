#include "BitReader.hpp"
#include "BitWriter.hpp"
#include "C:Tadarray\Array.hpp"
#include "C:TADfiles\funciones\files.hpp"
#include "HuffmanSetup.hpp"
#include "C:TADlist\List.hpp"

//para mostrar arbol y lista 
#include <locale>
#include <codecvt>

void tablaEnCero(HuffmanTable tabla[])
{
    // inicializo arr en cero
    for (int i = 0; i < 256; i++)
    {
        tabla[i].n = 0;
    }
}

void mostrarTabla(HuffmanTable tabla[])
{
    // prueba de ver al arr con sus elementos
    for (int i = 0; i < 256; i++)
    {

        unsigned char c = i;
        // cout << c << " " se imprime hasta el 127 en la consola por temas de representacion si muestro el caracter
        cout << i << " " << tabla[i].n << " " << tabla[i].cod << endl;
    }
}

void contarOcurrencias(string fName, HuffmanTable tabla[])
{
    FILE *f = fopen(fName.c_str(), "r+b"); // obtengo el puntero a la cadena de caracteres (a la cadena fName)

    if (f == nullptr)
    {
        cout << "Error al abrir el archivo" << endl;
        exit(0);
    }

    unsigned char c;
    fread(&c, sizeof(unsigned char), 1, f); // leo primer byte

    while (!feof(f))
    {
        int asci = c; // leo A y guardo su ascii 65
        // me ubico en la pos 65 del arr  e incremento su cont
        tabla[asci].n++;

        fread(&c, sizeof(unsigned char), 1, f);
    }

    fclose(f);
}

int cmpListAscendente(HuffmanTreeInfo a, HuffmanTreeInfo b)
{
    return (a.n < b.n) ? -1 : (a.n > b.n) ? 1
                                          : (a.c - b.c); // ordena asc por cantidad primero y, en caso de empate, por valor ASCII
}

void crearLista(HuffmanTable tabla[], List<HuffmanTreeInfo> &lista)
{
    // recorro el arr tabla y agrego a la lista si es >0
    HuffmanTreeInfo hf;
    for (int i = 0; i < 256; i++)
    {

        if (tabla[i].n > 0)
        {
            hf.left = nullptr; // punteros a null, para identificar las hojas en el tad huff
            hf.right = nullptr;
            hf.n = tabla[i].n;
            hf.c = i;
            listAdd<HuffmanTreeInfo>(lista, hf);
        }
    }

    // ordeno lista ascentdente por ocurrencias
    listSort<HuffmanTreeInfo>(lista, cmpListAscendente);
}

/*  daba problemas con caracteres con acentuacion por ser multibyte (utf-8)

void mostrarLista(List<HuffmanTreeInfo> lista)
{
    // recorro la lista

    while (listHasNext<HuffmanTreeInfo>(lista))
    {
        HuffmanTreeInfo *hf = listNext<HuffmanTreeInfo>(lista);
        cout << hf->c << ", " << (unsigned char)hf->c << ", " << hf->n << endl;
    }
} */
void mostrarLista(List<HuffmanTreeInfo> lista) // chat gpt
{
    // Configurar la salida para manejar caracteres multibyte
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wcout.imbue(utf8_locale);

    // Recorrer la lista
    while (listHasNext<HuffmanTreeInfo>(lista))
    {
        HuffmanTreeInfo *hf = listNext<HuffmanTreeInfo>(lista);

        // Convertir el código del caracter a wstring
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wideChar = converter.from_bytes(std::to_string(hf->c));

        // Imprimir información
        std::wcout << L"Ascii: " << wideChar << ", " << L", Ocurrencias: " << hf->n << std::endl;
    }
}

HuffmanTreeInfo *crearArbol(List<HuffmanTreeInfo> lista)
{
    HuffmanTreeInfo *nuevoNodo = nullptr; // se convierte en raiz despues del while

    // recorro la lista
    int i = 1;                         // para diferenciar si las ocurrencias de dos nodos padres coinciden
    while (lista.head->sig != nullptr) // verifico si hay al menos un nodo mas en la lista despues del nodo head
    {
        // remuevo primero dos nodos y les asigno memoria  ya que no son temporales y necesito almacenarlos fuera del bucle
        HuffmanTreeInfo *nodo1 = new HuffmanTreeInfo(listRemoveFirst<HuffmanTreeInfo>(lista));
        HuffmanTreeInfo *nodo2 = new HuffmanTreeInfo(listRemoveFirst<HuffmanTreeInfo>(lista));

        nuevoNodo = new HuffmanTreeInfo; // creo nuevo nodo dinamicamente

        // le asigno sus hijos
        nuevoNodo->right = nodo1;
        nuevoNodo->left = nodo2;

        // sumo las ocurrencias de ambos nodos
        nuevoNodo->n = nodo1->n + nodo2->n;

        // ultimo caracter para que siempre sea el mayor de su categoria
        nuevoNodo->c = 255 + i;

        // inserto ordenadamente en la lista
        listOrderedInsert<HuffmanTreeInfo>(lista, *nuevoNodo, cmpListAscendente);

        i++;
    }

    // retorno la raiz del arbol

    return nuevoNodo;
}

/* void mostrarArbol(HuffmanTreeInfo *raiz)
{
    // recorro el arbol usando TAD HuffmanTree
    HuffmanTree ht = huffmanTree(raiz);

    string cod;
    while (huffmanTreeHasNext(ht))
    {
        HuffmanTreeInfo *x = huffmanTreeNext(ht, cod);
        cout << x->c << ", " << (unsigned char)x->c << ", (" << x->n << "), "
             << "[" << cod << "]" << endl;
    }
}
 */

//opte esta funcion ya que la anterior presentaba problemas con los caracteres acentuados
void mostrarArbol(HuffmanTreeInfo *raiz)
{
    // configurar la salida para manejar caracteres multibyte
    std::locale utf8_locale(std::locale(), new std::codecvt_utf8<wchar_t>);
    std::wcout.imbue(utf8_locale);

    // recorrer el arbol usando TAD HuffmanTree
    HuffmanTree ht = huffmanTree(raiz);

    string cod;
    while (huffmanTreeHasNext(ht))
    {
        HuffmanTreeInfo *x = huffmanTreeNext(ht, cod);

        // convertir el código del caracter a wstring
        std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
        std::wstring wideChar = converter.from_bytes(std::to_string(x->c));

        // imprimir informacion
        std::wcout << L"Ascii: " << wideChar.c_str() << L", Ocurrencias: " << x->n << L", Cod: [" << cod.c_str() << L"]" << std::endl;
    }
}
void cargarCodigosEnTabla(HuffmanTreeInfo *raiz, HuffmanTable tabla[])
{
    // recorro el arbol usando TAD HuffmanTree
    HuffmanTree ht = huffmanTree(raiz);

    string cod;
    while (huffmanTreeHasNext(ht))
    {
        HuffmanTreeInfo *x = huffmanTreeNext(ht, cod);
        unsigned int ascii = x->c; // caracter a int
        // accedo al arr en la pos ascii
        tabla[ascii].cod = cod; // guardo el codigo en la tabla
    }
}

void grabarArchivoComprimido(string fName, HuffmanTable tabla[])
{
    // abro archivo orignial, lo recorro byte por byte
    // accedo a la tabla con ese numde byte
    // obtengo secuencia de bit para reemplazar
    // agarro sig byte y asi ....
    // antes generar y cargar el encabezado

    // ESCRITURA ENCABEZADO
    // primer byte para n cantidad de caracteres diferentes
    //   n cantidad de registros
    // unsigned int (4bytes) al final con longitud archivo original

    FILE *fHuf = fopen((fName + ".huf").c_str(), "w+b"); // archivo comprimido

    // recorro tabla para contar cuantos caracteres >0 hay
    int contCaracteres = 0;
    for (int i = 0; i < 256; i++)
    {
        if (tabla[i].n > 0)
        {
            contCaracteres++;
        }
    }

    if (contCaracteres > 255) // caso en el que se utilicen todos los caracteres ascii, que no haya overflow
    {

        contCaracteres = 255;
    }
    // escribo primer byte
    unsigned char cC = contCaracteres; // buffer
    fwrite(&cC, sizeof(unsigned char), 1, fHuf);

    // escribo n registros de longitud variable

    for (int j = 0; j < 256; j++) // recorro la tabla y selecciono los elementos >0
    {

        if (tabla[j].n > 0)
        {
            // 1 byte - escribo el ascii del caracter seleccionado ej: a=97
            unsigned char cAscii = j;
            fwrite(&cAscii, sizeof(unsigned char), 1, fHuf);

            // 1 byte - escribo con cuantos bits voy a codificar el caracter (longitud del cod) ej: 3 ="101" -> etx=3
            int largo = tabla[j].cod.length();
            unsigned char cLargo = largo;
            fwrite(&cLargo, sizeof(unsigned char), 1, fHuf);

            //  n bytes  segun la longitud del cod
            for (int k = 0; k < cLargo; k++)
            {
                // obtengo el primer caracter de la cadena cod
                string s = tabla[j].cod;
                unsigned char c = s[k];
                if (c == '1') // si es 1  escribo el byte 49
                {
                    fwrite(&c, sizeof(unsigned char), 1, fHuf);
                }
                else if (c == '0') // si es 0 escribo el byte 48
                {
                    fwrite(&c, sizeof(unsigned char), 1, fHuf);
                }
            }
        }
    }

    // abro archivo original
    FILE *f = fopen(fName.c_str(), "r+b");
    if (f == nullptr) // verificacion de correcta apertura
    {
        cout << "Error al abrir el archivo" << endl;
        exit(0);
    }

    // ULTIMO BYTE ENCABEZADO
    // calculo longitud del archivo orignial
    unsigned int actual = ftell(f);
    fseek(f, 0, SEEK_END); // me muevo al final
    unsigned int ultimo = ftell(f);
    fseek(f, actual, SEEK_SET); // regreso a donde estaba
    unsigned int tamanioArchiOr = ultimo;

    fwrite(&tamanioArchiOr, sizeof(unsigned int), 1, fHuf); // escribo 4 bytes en el archivo comprimido

    // POST ENCABEZADO
    //  recorro archivo original
    BitWriter bw = bitWriter(fHuf); // inicializo estructura de escritura bit a bit
    unsigned char c;
    fread(&c, sizeof(unsigned char), 1, f); // leo primer byte

    while (!feof(f))
    {
        unsigned int ascii = c;        // lo convierto a indice de la tabla
        string cod = tabla[ascii].cod; // obtengo el codigo de ese caracter
        int largoCod = cod.length();
        for (int i = 0; i < largoCod; i++) // recorro la cadena
        {
            int bit = charToInt(cod[i]);
            bitWriterWrite(bw, bit); // grabo bit a bit
        }

        fread(&c, sizeof(unsigned char), 1, f);
    }
    bitWriterFlush(bw);

    fclose(f);
    fclose(fHuf);
}

void comprimir(string fName)
{
    // huffmanTreeDemo();

    // Paso 1
    HuffmanTable tabla[256];
    tablaEnCero(tabla);
    contarOcurrencias(fName, tabla);

    cout << "TABLA" << endl;
    cout << endl;
    mostrarTabla(tabla);

    cout << endl;
    cout << "------------------------------" << endl;
    cout << endl;
    cout << "LISTA" << endl;
    cout << endl;

    // Paso 2
    List<HuffmanTreeInfo> lista = list<HuffmanTreeInfo>();
    crearLista(tabla, lista);
    mostrarLista(lista);

    cout << endl;
    cout << "------------------------------" << endl;
    cout << endl;
    cout << "ARBOL" << endl;
    cout << endl;

    // Paso 3
    HuffmanTreeInfo *raiz = crearArbol(lista);

    mostrarArbol(raiz); // muestro arbol usando el tad huffman

    // Paso 4
    cargarCodigosEnTabla(raiz, tabla); // recorro el arbol y cargo el cod en la tabla

    cout << endl;
    cout << "------------------------------" << endl;
    cout << endl;
    cout << "TABLA CON CODIGOS" << endl;
    cout << endl;

    mostrarTabla(tabla);

    // Paso 5
    grabarArchivoComprimido(fName, tabla); // grabo archivo comprimido
}

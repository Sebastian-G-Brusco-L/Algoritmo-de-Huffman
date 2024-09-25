

#ifndef _TARR_BITREADER_
#define _TARR_BITREADER_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "C:TADstrings\funciones\strings.hpp"
#include "C:TADfile\funciones\files.hpp"

using namespace std;

struct BitReader
{
   FILE *f;
   unsigned char c; // ocupa un byte de memoria, lo que significa que puede representar 8 bits individuales.

   int contador;
};
// Crea e inicializa una variable tipo BitReader.
BitReader bitReader(FILE *f)
{
   BitReader Br;
   Br.f = f;
   Br.c = read<unsigned char>(Br.f); // leo primer byte/registro para comenzar su lectura
   Br.contador = 0;
   return {Br};
}
// Retorna una cadena compuesta de unos y ceros que representa al byte c.
string _binToString(unsigned char c) // c= 'A' -> 0100 0001
{
   string binStr;
   for (int i = 7; i >= 0; --i)              // recorro el byte desde el mas significativo
   {                                         // para obtener el char bit, debo desplazarlo al extremo izq,(menos signif.) para asi hacer & con la mascara del bit 1 (0000 0001) y luego asignarle el char '1' o '0'
      char bit = ((c >> i) & 1) ? '1' : '0'; // guarda el char 1 o 0. hago & con la mascara del bit 1
      binStr += bit;                         // almaceno el char en la cadena
   }
   return binStr;
}

// Lee un bit desde el archivo.
int bitReaderRead(BitReader &br)
{

   if (br.contador == 8) // si ya conte todos los bits
   {

      br.contador = 0; // seteo el contador a 0 para contar el nuevo byte

      br.c = read<unsigned char>(br.f); // leo el nuevo byte
   }
   string s = _binToString(br.c); // 0100 0001 cadena que contiene al registro
   char c = s[br.contador];       // caracter bit
   int bit = charToInt(c);        // bit
   br.contador++;                 // aumento contador para revolver siguiente caraceter

   return bit;
}

#endif

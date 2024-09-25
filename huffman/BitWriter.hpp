#ifndef _TARR_BITWRITER_
#define _TARR_BITWRITER_

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include "C:TADstrings\funciones\strings.hpp"
#include "C:TADfile\funciones\files.hpp"

using namespace std;

struct BitWriter
{
   FILE *f;
   unsigned char c; // ocupa un byte de memoria, lo que significa que puede representar 8 bits individuales.
   string buffer;
};

// Crea e inicializa una variable tipo BitWriter
BitWriter bitWriter(FILE *f)
{
   BitWriter bw;
   bw.f = f;
   bw.c = ' ';
   bw.buffer = "";
   return {bw};
}
// Graba un bit en el archivo.
void bitWriterWrite(BitWriter &bw, int bit)
{
   bw.buffer += intToString(bit); // guardo el bit en el buffer, cuando tengo 8 bits completo el byte y se lo paso al unsigned char

   // "01000001"
   if (length(bw.buffer) == 8) //  buffer con 8 bits
   {
      bw.c = 0; // Inicializo el unsigned char para hacer la mascara con la cadena, internamente bw.c queda  00000000

      for (int i = 0; i < 8; i++) // operacion logica entre la cadena y el unsigned char
      {
         bw.c = bw.c << 1;                                            //   01000001  bw.c                  Desplazo los bits de bw.c hacia la izquierda y lo guardo en bw.c 00000000
        
         bw.c = bw.c |static_cast<unsigned char>(bw.buffer[i] - '0'); //   01000000  bw.c             Agrego el bit actual al unsigned char, obtengo el valor entero de cada char de la cadena buffer, ej: "1" -'0' = 1 entero, "0" -'0' = 0 entero,
      }                                                              //    00000001    <- 1 = bw.buffer[7] - '0' 
                                                                     //or ----------
                                                                     //    01000001
      bw.buffer = ""; // Reinicio el buffer

      // Ahora puedes grabar el unsigned char bw.c en el archivo
      write<unsigned char>(bw.f, bw.c);
   }
}
// Graba en el archivo los unos y ceros que contiene bits
void bitWriterWrite(BitWriter &bw, string sbit)
{
   if (length(sbit)==8) //solo escribo si la cadena sbit tiene 8 bits
   {
      bw.c = 0; //inicializo el byte 00000000
      for (int i = 0; i < length(sbit); i++) //recorro la cadena sbit,  desplazo, op log. con la mascara(sbit)
      {
      bw.c = bw.c << 1; //desplazo 1 a la izq los bits 00000000
      bw.c = bw.c | static_cast<unsigned char>(sbit[i]-'0'); // op. log. or entre bw.c y el valor entero resultante de la resta del primer caracter de la cadena - el caracter 0.
      }
      bw.buffer = ""; //reinicio buffer para un byte futuro
      write<unsigned char>(bw.f,bw.c); 
   }
      
   bw.buffer = sbit;
}

void bitWriterFlush(BitWriter &bw) //bw.c = 011 00000
{
/* por curiosidad
   fseek(bw.f, 0, SEEK_END);         // posiciono el puntero alfinal del archivo
   long int fileSize = ftell(bw.f);  // tamanio del arch en bytes
   long int bitCount = fileSize * 8; // obtengo bits */
   
   int largo = length(bw.buffer);   
   if (largo<8) // si el buffer no esta completo
   {
      for (int i = 0;  i<8-largo; i++) //i < 8 - largo; para completar el resto del byte   0<6
      {
         bw.buffer +="0";
      }
      
   }

   //"01000000"         
   bw.c=0;                                                                       // "01"
   for (int i=0; i<length(bw.buffer) ; i++ ) // recorro el buffer
   {
      bw.c= bw.c << 1; // agrego un 0 hasta que se completen 8 bits    0000 0010  bw.c
      bw.c = bw.c | static_cast<unsigned char>(bw.buffer[i]-'0');//    0000 0001  buffer[2]=1
                                                               //      ----------
   }                                                           //      0000 0001  
   bw.buffer = ""; //reinicio buffer para un byte futuro
   write<unsigned char>(bw.f,bw.c);
}

#endif


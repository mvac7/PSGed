/* =============================================================================
   vprint.c
  (copyleft) 2016 303bcn

  Description:
     print functions for screen1 and screen2, SDCC library
============================================================================= */
#ifndef __VPRINT_H__
#define __VPRINT_H__

#include "newTypes.h"


/* =============================================================================
 Provides the value of tile which is located in screen coordinates indicated.
 Proporciona el valor de tile que se encuentra en las coordenadas de pantalla
 indicadas.
   x (byte) 0 - 255
   y (byte) 0 - 191
============================================================================= */
byte GetTileByCoordinates(byte x, byte y);


/* =============================================================================
 Provides the value of tile, which is in position indicate from the screen.
 Proporciona el valor de tile que se encuentra en la posicion de pantalla
 indicada.
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
============================================================================= */
byte GetTileByPosition(byte column, byte line);


/* =============================================================================
 It provides the address of the video memory map tiles, from the screen position
 indicated.
 Proporciona la direccion de la memoria de video del mapa de tiles, a partir de
 la posicion de pantalla indicada.
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
============================================================================= */
uint GetVAddressByPosition(byte column, byte line);


/* =============================================================================
 Prints a string in the indicated position.
 Imprime una cadena de texto en la posición indicada.
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
   text (char*) string
============================================================================= */
void VPRINT(byte column, byte line, char* text);


/* =============================================================================
 Prints a string in the indicated position.
 Imprime una cadena de texto en la posición indicada.
 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
   text (char*) string
   length (uint) longitud de la cadena a imprimir.
============================================================================= */
void VPRINTN(byte column, byte line, char* text, uint length);



/* =============================================================================
 Dump the contents of an array of char in a position shown in the video memory.
 Vuelca el contenido de un array de char en una posición indicada de la memoria 
 de vídeo.
 Inputs:
   vaddress (uint) 
   text (char*) array
============================================================================= */
void VPokeString(uint vaddress, char* text);



void VPrintNumber(byte column, byte line, int value);



/* =============================================================================
 16-bit Integer to ASCII (decimal)
 original code by baze
 (update) Add functionality to replace leading zeros by spaces.  
 Input: HL = number to convert, DE = location of ASCII string
 Output: ASCII string at (DE)
============================================================================= */
void num2Dec16(int aNumber, char *address);


//void VPRINTO(byte column, byte line, char* text, char offset);
//void vpokeBlockOffset(uint vaddr, char* text, char offset);




#endif
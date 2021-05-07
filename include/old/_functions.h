#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "../include/newTypes.h"

/*
int x;
int y;
byte acel; 
byte speed;
*/


//void joyController(char controller);
//void mouseController();
//void joyController(char controller, int *nx, int *ny, byte *nacel, byte *nspeed);

//byte strLength(char * str);
void print(byte posx, byte posy, char* text);
void nprint(byte posx, byte posy, char* text, byte length); // vprintLength DEPRECATED
void printo(byte posx, byte posy, char* text, char offset);
void printLines(byte posx, byte posy, char* text); // vprintLines DEPRECATED

void vprint(uint vaddr, char* text, char offset);
void vnprint(uint vaddr, char* text, byte length);

void vpoke_block(uint address, char* block, uint size);

byte peek(uint address);
uint peek_word(uint address);

void poke(uint address, byte value);
void poke_word(uint address, uint value);
void poke_block(uint address, char* block);


/* ========================================================================
Returns the joystick status
lee el estado de las teclas de cursor o mando de juego
=========================================================================== */
char joystick(byte n);

/* ========================================================================
Returns current trigger status
lee el estado de un boton del cursor o mando de juego
ofrece 0 si no esta pulsado o -1 si esta pulsado
0 - space key
1 - button 1 joy A
2 - button 1 joy B
3 - button 2 joy A
4 - button 2 joy B
=========================================================================== */
char joytrig(byte n);

//One character input (waiting)
byte inkey();

//Reset
void CHKRAM();

//Clear keyboard buffer
void KILBUF();

//Returns the value of the specified line from the keyboard matrix
byte SNSMAT(byte line);

// ===========================================================================
// byte getTileByPos(byte xPos, byte yPos)
// Proporciona el valor de tile a partir de dos coordenadas de pantalla(256x192)
// Solo para Screen 2.
// by aorante
// OLD name: getNumTile, getNumTileByCoordinates
// ===========================================================================
byte getTileByPos(byte xPos, byte yPos);

// old name getVAddressByPosition
int getVAddrByPos(byte column, byte line);

byte strlen(char* text);
void strcpy(char * destination, const char * source);
void strncpy(char * destination, const char * source, byte length);
void strcat(char* destination, const char* source);


void WAIT(uint cicles);

//byte getNumTile(byte xPos, byte yPos);
//int getAddress(byte column, byte line);

#endif
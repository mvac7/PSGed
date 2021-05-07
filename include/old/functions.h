#ifndef __FUNCTIONS_H__
#define __FUNCTIONS_H__

#include "../include/newTypes.h"

/*
int x;
int y;
byte acel; 
byte speed;
*/
#define  SEED         0xE000



// ===========================================================================
// Proporciona el valor de tile a partir de dos coordenadas de pantalla (256x192)
// Solo para Screen 2.
// by aorante
// OLD name: getNumTile, getNumTileByCoordinates
// ===========================================================================
byte getTileByPos(byte xPos, byte yPos);

// old name getVAddressByPosition
uint getVAddrByPos(byte column, byte line);

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
void fillRAM(uint address, int size, byte value);



//One character input (waiting)
byte inkey();

//Clear keyboard buffer
void KILBUF();

//Returns the value of the specified line from the keyboard matrix
byte SNSMAT(byte line);




byte strlen(char* text);
void strcpy(char* destination, const char* source);
void strncpy(char* destination, const char* source, byte length);
void strcat(char* destination, const char* source);
void strncat(char* destination, const char* source, byte length);


// genera un valor aleatorio de 8 bits y le aplica una máscara
// necesita de un byte en la RAM (SEED)
byte Rnd(char value);



//Reset
void CHKRAM();

//pausa en ciclos de VBLANK. PAL 1seg=50, NTSC 1seg=60. 
void WAIT(uint cicles);

//byte getNumTile(byte xPos, byte yPos);
//int getAddress(byte column, byte line);

#endif
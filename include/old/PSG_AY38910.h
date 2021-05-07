/* ========================================================================== */
/*                                                                            */
/*   PSG_AY38910.h                                                            */
/*                                                                            */
/*   Description                                                              */
/*   Opensource library for acces to PSG AY-3-8910                            */
/* ========================================================================== */
#ifndef  __PSG_AY38910_H__
#define  __PSG_AY38910_H__

#include "newTypes.h"

//byte PSG_type;
extern char PSG_type;


//void setPSGtype(char type);
//char getPSGtype();

// ===========================================================================
// escribe en un registro del PSG
// write into a register of PSG
// ===========================================================================
void sound_set(byte reg, byte val);


// ===========================================================================
// escribe en un registro del PSG interno del MSX
// write into a register of internal PSG 
// ===========================================================================
void PSGset(byte reg, byte val);


// ===========================================================================
// escribe en un registro del PSG de la MegaFlashROM SCC+
// write into a register of PSG from MegaFlashROM SCC+ 
// ===========================================================================
void PSGMFRset(byte reg, byte val);


// ===========================================================================
// lee el valor de un registro del PSG
// by Andrear
// ===========================================================================
byte sound_get(byte reg);

byte PSGget(byte reg);

byte PSGMFRget(byte reg);

#endif
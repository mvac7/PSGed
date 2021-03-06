// MSX System Variables v0.8 20/3/2013
// definitions & texts by MSX Assembly Page
// http://map.grauw.nl/resources/msxsystemvars.php

#ifndef  __MSXSYSTEMVARS_H__
#define  __MSXSYSTEMVARS_H__

// Z80 Wait for interrupt or reset
//#define  HALT __asm halt __endasm

// MSX System Variables
#define VDP.DR  0x0006 // Base port address for VDP data read
#define VDP.DW  0x0007 // Base port address for VDP data write

//0x002B	//1	Basic ROM version
/*
7 6 5 4 3 2 1 0
| | | | +-+-+-+-- Character set
| | | |           0 = Japanese, 1 = International, 2=Korean
| +-+-+---------- Date format
|                 0 = Y-M-D, 1 = M-D-Y, 2 = D-M-Y
+---------------- Default interrupt frequency
                  0 = 60Hz, 1 = 50Hz
*/
                  
//0x002C	//1	Basic ROM version
/*
7 6 5 4 3 2 1 0
| | | | +-+-+-+-- Keyboard type
| | | |           0 = Japanese, 1 = International
| | | |           2 = French (AZERTY), 3 = UK, 4 = German (DIN)
+-+-+-+---------- Basic version
                  0 = Japanese, 1 = International
*/

//0x002D	//1	MSX version number
/*0 = MSX 1
1 = MSX 2
2 = MSX 2+
3 = MSX turbo R
*/

//0x002E	//1	Bit 0: if 1 then MSX-MIDI is present internally (MSX turbo R only)



//
#define RDPRIM	0xF380	//5	Routine that reads from a primary slot
#define WRPRIM	0xF385	//7	Routine that writes to a primary slot
#define CLPRIM	0xF38C	//14	Routine that calls a routine in a primary slot
#define USRTAB	0xF39A	//2	Address to call with Basic USR0
/*
#F39C	2	Address to call with Basic USR1
#F39E	2	Address to call with Basic USR2
#F3A0	2	Address to call with Basic USR3
#F3A2	2	Address to call with Basic USR4
#F3A4	2	Address to call with Basic USR5
#F3A6	2	Address to call with Basic USR6
#F3A8	2	Address to call with Basic USR7
#F3AA	2	Address to call with Basic USR8
#F3AC	2	Address to call with Basic USR9
*/

#define LINL40	0xF3AE	//1	Width for SCREEN 0 (default 37)
#define LINL32	0xF3AF	//1	Width for SCREEN 1 (default 29)
#define LINLEN	0xF3B0	//1	Width for the current text mode
#define CRTCNT	0xF3B1	//1	Number of lines on screen
#define CLMLST	0xF3B2	//1	Column space. It?s uncertain what this address actually stores
#define TXTNAM	0xF3B3	//2	BASE(0) - SCREEN 0 name table
#define TXTCOL	0xF3B5	//2	BASE(1) - SCREEN 0 color table
#define TXTCGP	0xF3B7	//2	BASE(2) - SCREEN 0 character pattern table
#define TXTATR	0xF3B9	//2	BASE(3) - SCREEN 0 Sprite Attribute Table
#define TXTPAT	0xF3BB	//2	BASE(4) - SCREEN 0 Sprite Pattern Table
#define T32NAM	0xF3B3	//2	BASE(5) - SCREEN 1 name table
#define T32COL	0xF3B5	//2	BASE(6) - SCREEN 1 color table
#define T32CGP	0xF3B7	//2	BASE(7) - SCREEN 1 character pattern table
#define T32ATR	0xF3B9	//2	BASE(8) - SCREEN 1 sprite attribute table
#define T32PAT	0xF3BB	//2	BASE(9) - SCREEN 1 sprite pattern table
#define GRPNAM	0xF3B3	//2	BASE(10) - SCREEN 2 name table
#define GRPCOL	0xF3B5	//2	BASE(11) - SCREEN 2 color table
#define GRPCGP	0xF3B7	//2	BASE(12) - SCREEN 2 character pattern table
#define GRPATR	0xF3B9	//2	BASE(13) - SCREEN 2 sprite attribute table
#define GRPPAT	0xF3BB	//2	BASE(14) - SCREEN 2 sprite pattern table
#define MLTNAM	0xF3B3	//2	BASE(15) - SCREEN 3 name table
#define MLTCOL	0xF3B5	//2	BASE(16) - SCREEN 3 color table
#define MLTCGP	0xF3B7	//2	BASE(17) - SCREEN 3 character pattern table
#define MLTATR	0xF3B9	//2	BASE(18) - SCREEN 3 sprite attribute table
#define MLTPAT	0xF3BB	//2	BASE(19) - SCREEN 3 sprite pattern table
#define CLIKSW	0xF3DB	//1	
                        // =0 when key press click disabled
                        // =1 when key press click enabled
                        // SCREEN ,,n will write to this address
#define CSRY	  0xF3DC	//1	Current row-position of the cursor
#define CSRX	  0xF3DD	//1	Current column-position of the cursor
#define CNSDFG	0xF3DE	//1	=0 when function keys are not displayed
                        //  =1 when function keys are displayed
#define RG0SAV	0xF3DF	//1	Content of VDP(0) register (R#0)
#define RG1SAV	0xF3E0	//1	Content of VDP(1) register (R#1)
#define RG2SAV	0xF3E1	//1	Content of VDP(2) register (R#2)
#define RG3SAV	0xF3E2	//1	Content of VDP(3) register (R#3)
#define RG4SAV	0xF3E3	//1	Content of VDP(4) register (R#4)
#define RG5SAV	0xF3E4	//1	Content of VDP(5) register (R#5)
#define RG6SAV	0xF3E5	//1	Content of VDP(6) register (R#6)
#define RG7SAV	0xF3E6	//1	Content of VDP(7) register (R#7)
#define STATFL	0xF3E7	//1	Content of VDP(8) status register (S#0)

#define TRGFLG	0xF3E8	//1	Information about trigger buttons and space bar state
/*
7 6 5 4 3 2 1 0
| | | |       +-- Space bar, trig(0) (0 = pressed)
| | | +---------- Stick 1, Trigger 1 (0 = pressed)
| | +------------ Stick 1, Trigger 2 (0 = pressed)
| +-------------- Stick 2, Trigger 1 (0 = pressed)
+---------------- Stick 2, Trigger 2 (0 = pressed)
*/

#define FORCLR	0xF3E9	//1	Foreground color
#define BAKCLR	0xF3EA	//1	Background color
#define BDRCLR	0xF3EB	//1	Border color
#define MAXUPD	0xF3EC	//3	Jump instruction used by Basic LINE command. The routines used are: RIGHTC, LEFTC, UPC and DOWNC
#define MINUPD	0xF3EF	//3	Jump instruction used by Basic LINE command. The routines used are: RIGHTC, LEFTC, UPC and DOWNC
#define ATRBYT	0xF3F2	//1	Attribute byte (for graphical routines it?s used to read the color)
#define QUEUES	0xF3F3	//2	Address of the queue table
#define FRCNEW	0xF3F5	//1	CLOAD flag
//=0 when CLOAD
//=255 when CLOAD?
#define SCNCNT	0xF3F6	//1	Key scan timing
//When it?s zero, the key scan routine will scan for pressed keys so characters can be written to the keyboard buffer
#define REPCNT	0xF3F7	//1	This is the key repeat delay counter
//When it reaches zero, the key will repeat
#define PUTPNT	0xF3F8	//2	Address in the keyboard buffer where a character will be written
#define GETPNT	0xF3FA	//2	Address in the keyboard buffer where the next character is read

#define CS120	  0xF3FC	//5	Cassette I/O parameters to use for 1200 baud
#define CS240	  0xF401	//5	Cassette I/O parameters to use for 2400 baud
#define LOW	    0xF406	//2	Signal delay when writing a 0 to tape
#define HIGH	  0xF408	//2	Signal delay when writing a 1 to tape
#define HEADER	0xF40A	//1	Delay of tape header (sync.) block

#define ASPCT1	0xF40B	//2	Horizontal / Vertical aspect for CIRCLE command
#define ASPCT2	0xF40D	//2	Horizontal / Vertical aspect for CIRCLE command
#define ENDPRG	0xF40F	//5	Pointer for the RESUME NEXT command
#define ERRFLG	0xF414	//1	Basic Error code
#define LPTPOS	0xF415	//1	Position of the printer head
//Is read by Basic function LPOS and used by LPRINT Basic command
#define PRTFLG	0xF416	//1	Printer output flag is read by OUTDO
//=0 to print to screen
//=1 to print to printer
#define NTMSXP	0xF417	//1	Printer type is read by OUTDO. SCREEN ,,,n writes to this address
//=0 for MSX printer
//=1 for non-MSX printer
#define RAWPRT	0xF418	//1	Raw printer output is read by OUTDO
//=0 to convert tabs and unknown characters to spaces and remove graphical headers
//=1 to send data just like it gets it
#define VLZADR	0xF419	//2	Address of data that is temporarilly replaced by ?O? when Basic function VAL("") is running
#define VLZDAT	0xF41B	//1	Original value that was in the address pointed to with VLZADR
#define CURLIN	0xF41C	//2	Line number the Basic interpreter is working on, in direct mode it will be filled with #FFFF
//#F91F	1	Character set SlotID
//#F920	2	Character set address
#define EXBRSA	0xFAF8	//1	Slot address of the SUBROM (EXtended Bios-Rom Slot Address)

#define DRVINF	0xFB21	//1	Nr. of drives connected to disk interface 1
//#FB22	1	Slot address of disk interface 1
//#FB23	1	Nr. of drives connected to disk interface 2
//#FB24	1	Slot address of disk interface 2
//#FB25	1	Nr. of drives connected to disk interface 3
//#FB26	1	Slot address of disk interface 3
//#FB27	1	Nr. of drives connected to disk interface 4
//#FB28	1	Slot address of disk interface 4

#define EXPTBL	0xFCC1	//1	Slot 0: #80 = expanded, 0 = not expanded. Also slot address of the main BIOS-ROM.
//#FCC2	1	Slot 1: #80 = expanded, 0 = not expanded.
//#FCC3	1	Slot 2: #80 = expanded, 0 = not expanded.
//#FCC4	1	Slot 3: #80 = expanded, 0 = not expanded.

#define SLTTBL	0xFCC5	//1	Mirror of slot 0 secondary slot selection register.
//#FCC6	1	Mirror of slot 1 secondary slot selection register.
//#FCC7	1	Mirror of slot 2 secondary slot selection register.
//#FCC8	1	Mirror of slot 3 secondary slot selection register.

/*
FFE7    RG08SAV   Content of VDP(09) register (R#08)
FFE8    RG09SAV   Content of VDP(10) register (R#09)
FFE9    RG10SAV   Content of VDP(11) register (R#10)
FFEA    RG11SAV   Content of VDP(12) register (R#11)
FFEB    RG12SAV   Content of VDP(13) register (R#12)
FFEC    RG13SAV   Content of VDP(14) register (R#13)
FFED    RG14SAV   Content of VDP(15) register (R#14)
FFEE    RG15SAV   Content of VDP(16) register (R#15)
FFEF    RG16SAV   Content of VDP(17) register (R#16)
FFF0    RG17SAV   Content of VDP(18) register (R#17)
FFF1    RG18SAV   Content of VDP(19) register (R#18)
FFF2    RG19SAV   Content of VDP(20) register (R#19)
FFF3    RG20SAV   Content of VDP(21) register (R#20)
FFF4    RG21SAV   Content of VDP(22) register (R#21)
FFF5    RG22SAV   Content of VDP(23) register (R#22)
FFF6    RG23SAV   Content of VDP(24) register (R#23)
FFF7    ROMSLT    Slotadress of Main-ROM
FFF8                     Reserved
FFFA    RG25SAV   Content of VDP(26) register (R#25) MSX2+
FFFB    RG26SAV   Content of VDP(27) register (R#26) MSX2+
FFFC    RG27SAV   Content of VDP(28) register (R#27) MSX2+
FFFD              Temporary stack pointer storage
*/

#define SLTSL   0xFFFF //(all slots)	1	Secondary slot select register. Reading returns the inverted previously written value.


#endif
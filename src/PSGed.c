/* =============================================================================
   PSGed
   version: v0.83b (library update and some improvements)
   
   Author: aorante (aka mvac7)                                                      
   Architecture: MSX 
   Format: ROM 32K 
   Programming language: C and Z80 Assembler
   
   Powered by fR3eL (https://github.com/mvac7/SDCC_MSX_fR3eL)
                                     
   Description:
     program for MSX computers with the functionality to play with the registers 
     from PSG (AY-3-8910).
============================================================================= */

#include "../include/newTypes.h"
#include "../include/msxSystemVars.h"
#include "../include/msxBIOS.h"

#include "../include/mouse.h"
#include "../include/joystick.h"
#include "../include/VDP_TMS9918A.h"
#include "../include/VDP_SPRITES.h"
#include "../include/AY38910.h"
#include "../include/keyboard.h"
#include "../include/memory.h"
#include "../include/string.h"
#include "../include/unRLEWBtoRAM.h"
#include "../include/unRLEWBtoVRAM.h"

#include "../include/PSGed_GFX.h"
#include "../include/PSGed_demoSONGs.h"

#define  HALT __asm halt __endasm


#define FILESIZE 0x035D

const char app_name[] = "PSGED";
const char app_version[] = "0.83B";

const char EnvelopeValue[8]={1,4,8,10,11,12,13,14}; 


typedef struct {
  char trackA[16];
  char trackB[16];
  char trackC[16];
} PATTERN;


typedef struct {
  char name[20];
  char author[20];
  
  uint freqA;
  uint freqB;
  uint freqC;
  
  char noiseFreq;
  
  char ampA;
  char ampB;
  char ampC;

  boolean toneA;
  boolean toneB;
  boolean toneC;
  
  boolean noiseA;
  boolean noiseB;
  boolean noiseC;
    
  boolean envelopeA;
  boolean envelopeB;
  boolean envelopeC;
    
  uint envelopeFreq;
  char envelopeType;
    
  char speed;
  char length;
  char sequence[32];
  
  PATTERN patterns[16];
  
} PSG_SONG;


// definicion funciones <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
void initScreen();
void mainWindow();
void checkMSX();
//char getVdp(void);

void SetPalette(char number);

//void SetGUIgfxData();

void ShowInitScreen();
void ShowMainScreen(); //ShowEditScreen
void ShowHelpScreen();
void ShowLoadScreen();


void ShowWindow(char posX, char posY, char width, char height, uint RAMaddr);

void ShowInfoWin();
void ShowCopyWin();
//void ShowPSGDumpWin();
//void ShowConfigWin();
//void ShowConfirmWin();


void initPSGData();
//void setPSGdata();
void ShowEditData();

boolean reverseCheckBox(boolean checked, int tiladdre);
void showCheckBox(char column, char line, boolean checked);
char ShowEnvelope(char value);
signed char ShowCombo(char posx, char posy);

char getEnvelopeIndex(char envelope);
//char getEnvelopeByIndex(char index);

void showPattern(char column, char line, char *patternData);
void showPatterns(char numPattern);

void setLastPos(char value, char *patternA, char *patternB, char *patternC);

//void setChannel(char NumChannel, boolean isTone, boolean isNoise);

void FastClsSc2(char color);

//void toHelpFromEdit();

void setPosition();

boolean showModalWin(char* aText);


void copyPatternTool();
void vInput(char posx, char posy, char* text, char length);

void VPrintNumber(char column, char line, unsigned int value, char length);
void VPrintNum(unsigned int vaddr, unsigned int value, char length);
void num2Dec16(int aNumber, char *address);
unsigned int GetVRAMaddressByPosition(char column, char line);
void VPRINT(char posx, char posy, char* text);
void VPRINTN(char column, char line, char* text, unsigned int length);

void initDemoData();
boolean load();
void LoadDemo(char index);

void info();
void config();
void PSGdump();

void pointerController();
void joyController();
void mouseController();
void showPointer();

// player functions
void playSong();
void playPattern();
void playStep();
void stopSong();
void toSilencePSG();
void playPSGregs();
void initPSGregs();
//

void setAmpA();
void setAmpB();
void setAmpC();

void setEnvelopeA();
void setEnvelopeB();
void setEnvelopeC();

void setFreqA();
void setFreqB();
void setFreqC();

void setEnvelopeFreq();

// help functions
void initHelpData();
void Help();
void showHelpText(char numlin);
void showHelpPage(char numlin);
void showScrollBar(char numlin, char maxNumLines);
// end




// definicion variables globales <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<

// config
char controller;
char trigController;
//char PSG_type;
char VDP_type;

// data
PSG_SONG PSGsong;
char songPos;
char playCounter;
char lastPlayCounter;
char currentPattern;
char *bpms;

int x;
int y;
char acel; 
char speed;

char demofiles[3];

boolean chanA; 
boolean chanB; 
boolean chanC;

char isPlay;       // control del play stop=0,song=1,pattern=2
char isPause;       // control de pausa nothing=0,song=1,pattern=2
char tempoCounter; // contador para ajustar el tempo en el play
boolean isStop; 
boolean setStop;
boolean isLoop;    // control del loop en el play song

//Help
char helpLines;
//uint HELP_ADDR;
//

char SCR_BUFFER[0x300];
//char LOAD_SCR_BUFFER[0x300];



    
void main(void) 
{
 
  isAYextern = false; //selecciona el PSG interno por defecto
  
  checkMSX();
    
  //initDemoData();
  initHelpData();
  
  SCREEN(2);
  COLOR(15,1,1);
  SetSpritesSize(1);
  //SetSpritesZoom(0);
  
  POKE(CLIKSW,0);
   
  if (VDP_type>0) SetPalette(0); 
  
  initScreen();
  
  if (VDP_type>0) SetPalette(1);
  
  mainWindow();
  
__asm
  call CHKRAM
__endasm;
}

/*
// funciones de TEST
void chekini()
{
__asm
   ld      bc,#0x0507                        ; Color azul...
   call    0x0047
__endasm;
}

void chekend()
{
__asm   
   ld      bc,#0x0107
   call    0x0047
__endasm;
}*/


void checkMSX(void) 
{
  //char a=0;
  
  // identifica modelo de MSX
  VDP_type = PEEK(0x002D);
  //POKE (MSXMODEL,a);
  
  
  //a=getVdp();
  //if(a==6) a=0;
  //else a=1;  
  //VDP_type = a;
  //POKE(VDPTYPE,a);
   
}


// identifica el tipo de VDP (6=TMS99XX. 0=V9938, 4=V9958)
/*char getVdp(void) // Thanks to Nyyrikki
{
__asm
  ; Read status register 1
	di
	ld a,#1
	out (0x99),a
	ld a,#0x8F
	out (0x99),a
	in a,(0x99)
	push af
	xor a
	out (0x99),a
	ld a,#0x8f
	out (0x99),a
	pop af
	ei

; Return VDP number
	and #0b00000110 ;110

	ld L,A	; VDP number in register A
	
__endasm;
}*/


//
/*
#include "functions.c"
#include "screens.c"
#include "menu.c"
#include "help.c"
#include "load.c"
#include "data.c"
*/
//


/* ===========================================================================
initScreen                                
Function : Presentacion y menu de inicio
Input    : 
=========================================================================== */
void initScreen()
{  
  char A=0, y=135, joyval=0, i=0; 
  //char fin = 0;
  char isOut = 0;
  boolean joybool=false;
  
  FastClsSc2(0x5F);
  
  ShowInitScreen();
   
  VPRINT(9, 14, "VERSION");
  VPRINT(17, 14, app_version);
  
  // detecta el mouse 
  for (i=0;i<60;i++)
  {
    HALT;
    A = CheckMouse();
    if(A>0) break;   
  } 
  
  VPRINT(12, 17, "CURSOR KEYS");
  VPRINT(12, 19, "JOYSTICK");
  if(A>0){
    VPRINT(12, 21, "MOUSE");
    y=167;
  }
  
  SetSpritePattern(3,3);
  SetSpriteColor(3,6);  
  
  while(isOut==0)
  {
    //put_sprite(3, 84, y, 6);
    SetSpritePosition(3,84,y);
    
    joyval=STICK(0);
    if(joyval==0) joyval=STICK(1);

    if (joyval>0)
    {
      if (joybool==false)
      {
        joybool = true;       
        
        if(joyval==1) // arriba
        { 
          if(y==151){ y=135;}//cursor 
          if(A>0 && y==167){ y=151;}//joystick 
        }
        if(joyval==5) // abajo
        {
          if(A>0 && y==151){ y=167;}// mouse
          if(y==135){ y=151;}//joystick           
        }
      }
    }else{
      joybool = false;
    }
    
    if (STRIG(0)<0) isOut=1;
    if (STRIG(1)<0) isOut=1;
    if (STRIG(2)<0) isOut=1;

    
    HALT;       
  }
  
  
  if(y==135) controller=0;
  if(y==151) controller=1;
  trigController = controller;
  if(y==167)
  {
    controller=A+2;
    trigController = A;
  }
   
  return;
}



// ===========================================================================
// gestion de la pantalla principal
// ===========================================================================
void mainWindow()
{
  // define/inicializa variables locales
  uint tiladdre;
  boolean isLoad=false; 
  boolean response=false;
  boolean trigbool=false, trig2bool=false, keybool=false;
  boolean chanAkeybool=false, chanBkeybool=false, chanCkeybool=false;
  boolean stopkeybool=false, playPSGkeybool=false;
  boolean playsngkeybool=false, playpatkeybool=false;
  
  char line=0, column=0; 
  char envelopIndex=0;
  char pauseTimer=0;
   
  //char acel=10, speed=0; 
  char keyAcel=4, keySpeed=0;
  char pressKey;
  char i;
  char songPattern=0;
  char bpmsdata[]={0,250,188,150,126,108,94,84,75,68,63};
  char tmpValue=0;
  signed char result=0;
  

  // inicializa variables globales
  x = 127;
  y = 81;
  acel=10; 
  speed=0;
  songPos=0;
  currentPattern=0;
  
  isPlay=0; 
  isPause=0;
  playCounter=0;
  lastPlayCounter=15;
  
  tempoCounter=0;
    
  bpms = bpmsdata;
  
  isStop=false; 
  setStop=false;
  isLoop=true;
  
  chanA=true; 
  chanB=true; 
  chanC=true;
  
  //SetSpritePattern(0,31);  
  //SetSpritePattern(1,31);
  //SetSpritePattern(2,31);
  //SetSpritePattern(3,31);
  
  FastClsSc2(0xEE);
  
  ShowMainScreen();
  
  //setTileSet();
  //setSprites();  
  //ShowEditScreen();
   
  
  initPSGData();
  
  // initialize screen controllers
  ShowEditData();
  
  envelopIndex = getEnvelopeIndex(PSGsong.envelopeType);
 
  initPSGregs();
  
  // inicializa sprites
  SetSpritePattern(0,0);
  SetSpriteColor(0,1);
    
  SetSpritePattern(1,1);
  SetSpriteColor(1,6);
  // end    
  
  
  while(1)
  {
    HALT;
    
    
    // #######################
    // test
    //VPrintNumber(22,0, peek(CLIKSW), 3);
    // end Test
    
    
    // #######################
    //   play
    // #######################
    if (isPlay>0)
    {
      if (isPlay==1)
      {
        playSong();    
      }else{
        playPattern();
      }    
      
      if(playCounter != lastPlayCounter)// muestra info en la pantalla
      { 
              
        if (setStop==true)
        {
          stopSong();
          /*setStop=false;
          toSilencePSG();          
          isPlay=0;
          isPause=0;
          playCounter=0;
          setLastPos(lastPlayCounter, PSGsong.patterns[currentPattern].trackA, PSGsong.patterns[currentPattern].trackB, PSGsong.patterns[currentPattern].trackC);
          lastPlayCounter=15;
          VPOKE(0x1AFA,245);
          VPOKE(0x1A1E,245);*/
          
          setPosition();
                    
        }else{        
          if (isPlay==1 && playCounter==0) setPosition();         
         
          //channelA
          tiladdre = GetVRAMaddressByPosition(15+playCounter,19);
          tmpValue = PSGsong.patterns[currentPattern].trackA[playCounter];
          if (tmpValue==0)
          {              
            VPOKE(tiladdre,184);
          }else{
            VPOKE(tiladdre,185);     
          }
                  
          //channelB
          tiladdre = GetVRAMaddressByPosition(15+playCounter,20);
          tmpValue = PSGsong.patterns[currentPattern].trackB[playCounter];
          if (tmpValue==0)
          {
            VPOKE(tiladdre,184);
          }else{
            VPOKE(tiladdre,185);                 
          }
                  
          //channelC
          tiladdre = GetVRAMaddressByPosition(15+playCounter,21);
          tmpValue = PSGsong.patterns[currentPattern].trackC[playCounter];
          if (tmpValue==0)
          {
            VPOKE(tiladdre,184);
          }else{
            VPOKE(tiladdre,185);     
          }
                              
          // muestra la linea anterior
          setLastPos(lastPlayCounter, PSGsong.patterns[currentPattern].trackA, PSGsong.patterns[currentPattern].trackB, PSGsong.patterns[currentPattern].trackC);
          
          lastPlayCounter = playCounter;
                 
        }// end setStop             
      }// end set screen
    }//end PLAY
    // #######################
    
    
    if (isPause>0) // parpadeo icono play en modo pause
    {
      pauseTimer++;
      if(pauseTimer>10){
        pauseTimer=0;
        
        switch (isPause) 
        {
          case 1:
            isPause=2;
            VPOKE(0x1AFA,244);//play icon  
            break;
          case 2:
            isPause=1;
            VPOKE(0x1AFA,245);//play icon  
            break;
          case 3:
            isPause=4;
            VPOKE(0x1A1E,244);//play icon pattern  
            break;
          case 4:
            isPause=3;
            VPOKE(0x1A1E,245);//play icon pattern  
            break;
        }
      }    
    }// end pause
      
                               
    pointerController();
    //showPointer();
           
    
    // control de pulsacion del boton1 del joystick/barra espaciadora
    if (STRIG(trigController)<0)
    {
      if (trigbool==false)
      {
        trigbool = true;
        line = y/8;
        column = x/8;
        
        if (line==23)
        {
          if (column<6)//HELP
          {
             Help();
          }
          if (column>6 && column<12)//CONFIG
          {
            config(); 
          }
          if (column>12 && column<18)// LOAD 
          {
              isLoad = true;
          }                           
          if (column>18 && column<24) //SAVE
          {
             PSGdump();
          }
          /*if (column>24) //(F5)
          {
            
          } */
        }
        
        if (line==17)
        {
          // muestra ventana info
          if (column>0 && column<4)
          {
            info();                    
          }
          
          // NEW 
          if (column>3 && column<9)
          {
            response=showModalWin("ARE YOU SURE\nYOU WANT TO\nDELETE SONG?");
    
            if (response==true)
            {
              songPos=0;
              currentPattern=0;
              initPSGData();
              ShowEditData();
              initPSGregs();
              envelopIndex = getEnvelopeIndex(PSGsong.envelopeType);                
            }                    
          }
          
          // CLR - borra la informacion del pattern en curso
          if (column>12 && column<18) 
          {
            response=showModalWin("ARE YOU SURE\nYOU WANT TO\nDELETE PATTERN?");
    
            if (response==true)
            {
              for (i=0;i<16;i++)
              { 
                PSGsong.patterns[currentPattern].trackA[i]=0;
                PSGsong.patterns[currentPattern].trackB[i]=0;
                PSGsong.patterns[currentPattern].trackC[i]=0;
              }
              showPatterns(currentPattern);                
            }
          }
          
          // CPY - Copy
          if (column>17 && column<24) 
          {
            copyPatternTool();                        
          }
        }
        
        // buttons for mute channels
        if (column>12 && column<15)
        {
          if (line==19) //mute channel A on/off
          {
            if (chanA==true){
              VPOKE(0x1A6E,188);
              chanA=false;
              SetVolume(AY_Channel_A,0);
              //sound_set(8,0);
            }else{
              VPOKE(0x1A6E,187);
              chanA=true;              
            }      
          }
          
          if (line==20) //mute channel B on/off
          {
            if (chanB==true){
              VPOKE(0x1A8E,188);//14,20
              chanB=false;
              SetVolume(AY_Channel_B,0);
              //sound_set(9,0);
            }else{
              VPOKE(0x1A8E,187);
              chanB=true;              
            }
          }
          
          if (line==21) //mute channel C on/off
          {
            if (chanC==true){
              VPOKE(0x1AAE,188);//14,21
              chanC=false;
              SetVolume(AY_Channel_C,0);
              //sound_set(10,0);
            }else{
              VPOKE(0x1AAE,187);
              chanC=true;              
            }
          }
        }
        
        
        
        /* ##################### Channel A */
        if (column>7 && column<10)
        {
          //  0   Channel A tone enable       (0=Enable,1=Disable)
          if (line==5)
          {
            tiladdre = GetVRAMaddressByPosition(7,5);
            PSGsong.toneA = reverseCheckBox(PSGsong.toneA, tiladdre);
            SetChannel(AY_Channel_A,PSGsong.toneA,PSGsong.noiseA);        
          }
          
          // 3   Channel A noise enable      (0=Enable,1=Disable)
          if (line==8)
          {
            tiladdre = GetVRAMaddressByPosition(7,8);
            PSGsong.noiseA = reverseCheckBox(PSGsong.noiseA, tiladdre);
            SetChannel(AY_Channel_A,PSGsong.toneA,PSGsong.noiseA);    
          }
          
          // Envelope channel A
          if (line==9)
          {
            tiladdre = GetVRAMaddressByPosition(7,9);
            PSGsong.envelopeA = reverseCheckBox(PSGsong.envelopeA, tiladdre);
            setEnvelopeA();        
          }
        }
        
        
        /* ##################### Channel B */
        if (column>18 && column<21)
        {
          //  0   Channel B tone enable       (0=Enable,1=Disable)
          if (line==5)
          {
            tiladdre = GetVRAMaddressByPosition(18,5);
            PSGsong.toneB = reverseCheckBox(PSGsong.toneB, tiladdre);
            SetChannel(AY_Channel_B,PSGsong.toneB,PSGsong.noiseB);        
          }
          
          // 3   Channel B noise enable      (0=Enable,1=Disable)
          if (line==8)
          {
            tiladdre = GetVRAMaddressByPosition(18,8);
            PSGsong.noiseB = reverseCheckBox(PSGsong.noiseB, tiladdre);
            SetChannel(AY_Channel_B,PSGsong.toneB,PSGsong.noiseB);    
          }
          
          // Envelope channel B
          if (line==9)
          {
            tiladdre = GetVRAMaddressByPosition(18,9);
            PSGsong.envelopeB = reverseCheckBox(PSGsong.envelopeB, tiladdre);
            setEnvelopeB();        
          }
        }
        
        
        
        /* ##################### Channel C */
        if (column>28 && column<31)
        {
          //  0   Channel C tone enable       (0=Enable,1=Disable)
          if (line==5)
          {
            tiladdre = GetVRAMaddressByPosition(28,5);
            PSGsong.toneC = reverseCheckBox(PSGsong.toneC, tiladdre); 
            SetChannel(AY_Channel_C,PSGsong.toneC,PSGsong.noiseC);       
          }
          
          // 3   Channel C noise enable      (0=Enable,1=Disable)
          if (line==8)
          {
            tiladdre = GetVRAMaddressByPosition(28,8);
            PSGsong.noiseC = reverseCheckBox(PSGsong.noiseC, tiladdre);
            SetChannel(AY_Channel_C,PSGsong.toneC,PSGsong.noiseC);    
          }
          
          // Envelope channel C
          if (line==9)
          {
            tiladdre = GetVRAMaddressByPosition(28,9);
            PSGsong.envelopeC = reverseCheckBox(PSGsong.envelopeC, tiladdre);
            setEnvelopeC();        
          }
        }
       
        
        // Amplitud
        if (line==10)
        {
          // Amplitud del canal A. (0-15)
          if (column==8)// (+)
          {
             if (PSGsong.ampA<15)
             {
                 PSGsong.ampA++;
                 setAmpA();
             }
          }          
          if (column==9)// (-)
          {
             if (PSGsong.ampA>0)
             {
                 PSGsong.ampA--;
                 setAmpA();
             }
          }
          
        
          // Amplitud del canal B. (0-15)
          if (column==19)// (+)
          {
             if (PSGsong.ampB<15)
             {
                 PSGsong.ampB++;
                 setAmpB();
             }
          }
          if (column==20)// (-)
          {
             if (PSGsong.ampB>0)
             {
                 PSGsong.ampB--;
                 setAmpB();
             }
          }    
                
        
          // Amplitud del canal C. (0-15)
          if (column==29)// (+)
          {
             if (PSGsong.ampC<15)
             {
                 PSGsong.ampC++;
                 setAmpC();
             }
          }
          if (column==30)// (-)
          {
             if (PSGsong.ampC>0)
             {
                 PSGsong.ampC--;
                 setAmpC();
             }
          }        
        }

        
        //  noise frequency (0-31)
        if (line==13)
        {
          if (column==11)
          {
             if (PSGsong.noiseFreq>0)
             {
                 PSGsong.noiseFreq--;
                 VPrintNumber(8,13, PSGsong.noiseFreq, 2);
                 //06 = Noise period (0-31)
                 SetNoisePeriod(PSGsong.noiseFreq);
                //sound_set(6,PSGsong.noiseFreq);
             }
          }
          if (column==10)
          {
             if (PSGsong.noiseFreq<31)
             {
                 PSGsong.noiseFreq++;
                 VPrintNumber(8,13, PSGsong.noiseFreq, 2);
                 //06 = Noise period (0-31)
                 SetNoisePeriod(PSGsong.noiseFreq);
                //sound_set(6,PSGsong.noiseFreq);
             }
          }
        }
        
        
        // envelope
        if (column==25 && line==13)//combobox
        {
          result = ShowCombo(20,14);
          if (result>=0)
          {
            PSGsong.envelopeType = ShowEnvelope(result);
            PlayEnvelope(PSGsong.envelopeType);
            //sound_set(13,PSGsong.envelopeType);            
          } 
        }
        
        
/*        if (line==13)
        { 
          if (column==30)//(-)
          {
             if (envelopIndex>0)
             {
                envelopIndex--;
                PSGsong.envelopeType = ShowEnvelope(envelopIndex);
             }
          }
          if (column==29)//(+)
          {
             if (envelopIndex<7)
             {
                 envelopIndex++;
                 PSGsong.envelopeType = ShowEnvelope(envelopIndex);
             }
          }
        } */
        
        
        if (column>14 && column<31)
        {
          //  pattern channel A
          if  (line==19)
          {
            tiladdre = GetVRAMaddressByPosition(column,19);
            tmpValue = PSGsong.patterns[currentPattern].trackA[column-15];
            if (tmpValue<3) tmpValue+=181;
            else tmpValue=180;
            VPOKE(tiladdre,tmpValue);
            PSGsong.patterns[currentPattern].trackA[column-15]=tmpValue-180;
          }
                
          // pattern channel B
          if  (line==20)
          {
            tiladdre = GetVRAMaddressByPosition(column,20);          
            tmpValue = PSGsong.patterns[currentPattern].trackB[column-15];
            if (tmpValue<3) tmpValue+=181;
            else tmpValue=180;
            VPOKE(tiladdre,tmpValue);
            PSGsong.patterns[currentPattern].trackB[column-15]=tmpValue-180;
            
          }
        
          //  pattern channel C
          if (line==21)
          {
            tiladdre = GetVRAMaddressByPosition(column,21);          
            tmpValue = PSGsong.patterns[currentPattern].trackC[column-15];
            if (tmpValue<3) tmpValue+=181;
            else tmpValue=180;
            VPOKE(tiladdre,tmpValue);
            PSGsong.patterns[currentPattern].trackC[column-15]=tmpValue-180;
          }
        }
        
        
        // button play/pause song
        if (column==26 && line==23)
        {
          if (isPlay>0){
            //activa pause
            isPlay=0;
            isPause=1;
            pauseTimer=0;
            //isPlayPattern=false;
            VPOKE(0x1A1E,245);//play icon pattern
            VPOKE(0x1AFA,245);//play icon                       
            toSilencePSG();                        
          }else{
            // reanuda play
            setPosition();
            isPlay=1;
            isPause=0;
            //isPlayPattern=true;            
            VPOKE(0x1AFA,247);//pause icon
            VPOKE(0x1A1E,245);
          }                    
        }
        
        
        // button play/pause Pattern
        if (column==30 && line==16)
        {
          if (isPlay>0){
            //isPlayPattern=false;
            isPlay=0;
            isPause=3;
            pauseTimer=0;
            VPOKE(0x1A1E,245);//play icon pattern
            VPOKE(0x1AFA,245);//play icon            
            toSilencePSG();            
          }else{
            //isPlayPattern=true;
            isPlay=2;
            isPause=0;            
            VPOKE(0x1A1E,247);//pause icon
            VPOKE(0x1AFA,245);
          }                    
        }
        
        
        // button check loop
        if (column==28 && line==23)
        {
          if(isLoop==true){
            VPOKE(0x1AFC,254);
            isLoop = false;          
          }else{
            VPOKE(0x1AFC,253);
            VPOKE(0x1AFA,245);
            isLoop = true;
          }               
        }               
        
        
        // button stop
        if (column==30 && line==23)
        {
          if (isPlay>0)
          {
            isStop=true;
          }else{
            if(isPause>0) // si se pulsa stop en modo pausa
            {
              stopSong();
            }else{ // si esta parado, coloca la secuencia en la posicion inicial.
              toSilencePSG();
              songPos=0;
              setPosition(); 
            }          
          }                                       
        }
        
        
        //song Position POS
        if (line==18)
        {
          if (column==10)//(+)
          {
            if (songPos<31)
            {
              songPos++;
              VPrintNumber(8,18, songPos+1, 2);
              currentPattern=PSGsong.sequence[songPos];
              VPrintNumber(8,19, currentPattern+1, 2);
              VPrintNumber(27,17, currentPattern+1, 2);
              showPatterns(currentPattern);
            }
          }
          if (column==11)//(-)
          {
            if (songPos>0)
            {
              songPos--;
              VPrintNumber(8,18, songPos+1, 2);
              currentPattern=PSGsong.sequence[songPos];
              VPrintNumber(8,19, currentPattern+1, 2);
              VPrintNumber(27,17, currentPattern+1, 2);              
              showPatterns(currentPattern);
            }
          }
          
          // button SET. Actualiza el numero de patron de la posicion 
          // con el valor de patron en curso
          if (column==13)
          {
            PSGsong.sequence[songPos] = currentPattern;
            VPrintNumber(8,19, currentPattern+1, 2);                            
          } 
          
          
          // button Home (H). Coloca la secuencia en la posicion inicial.  
          /*if (column>5 && column<8)
          {
            songPos=0;
            VPrintNumber(8,18, songPos+1, 2);
            VPrintNumber(27,17, PSGsong.sequence[songPos]+1, 2);
            showPatterns(currentPattern);                     
          }*/
          
          if (column==6) // insert posicion
          {
            for(i=32;i>songPos;i--) PSGsong.sequence[i] = PSGsong.sequence[i-1];
            PSGsong.sequence[songPos] = currentPattern;
            VPrintNumber(8,19, PSGsong.sequence[songPos]+1, 2);
            if (PSGsong.length<32)
            {
              PSGsong.length++;
              VPrintNumber(8,20, PSGsong.length, 2);
            }             
          }
          
          if (column==7) // delete posicion
          {
            for(i=songPos;i<32;i++) PSGsong.sequence[i] = PSGsong.sequence[i+1];
            VPrintNumber(8,19, PSGsong.sequence[songPos]+1, 2);
            if (PSGsong.length>1)
            {
              PSGsong.length--;
              VPrintNumber(8,20, PSGsong.length, 2);
            }                       
          }
                    
        }
        
        
        //pattern seq
        if (line==19)
        {
          songPattern = PSGsong.sequence[songPos];
          if (column==10)//(+)
          {
            if (songPattern<15)
            {
              songPattern++;
              PSGsong.sequence[songPos] = songPattern;
              VPrintNumber(8,19, songPattern+1, 2);

            }
          }
          if (column==11)//(-)
          {
            if (songPattern>0)
            {
              songPattern--;
              PSGsong.sequence[songPos] = songPattern;
              VPrintNumber(8,19, songPattern+1, 2);

            }
          }          
        }
        
        //pattern
        if (line==17)
        {
          if (column==29)//(+)
          {
            if (currentPattern<15)
            {
              currentPattern++;
              VPrintNumber(27,17, currentPattern+1, 2);
              showPatterns(currentPattern);
            }
          }
          if (column==30)//(-)
          {
            if (currentPattern>0)
            {
              currentPattern--;
              VPrintNumber(27,17, currentPattern+1, 2);
              showPatterns(currentPattern);
            }
          }          
        }
        
        
        //longitud del song
        if (line==20)
        {
          if (column==11)//(-)
          {
            if (PSGsong.length>1)
            {
              PSGsong.length--;
              VPrintNumber(8,20, PSGsong.length, 2);
            }
          }
          if (column==10)//(+)
          {
            if (PSGsong.length<32)
            {
              PSGsong.length++;
              VPrintNumber(8,20, PSGsong.length, 2);
            }
          }
        }
        
        
        //  tempo (0-15)
        if (line==21)
        {
          if (column==10)//(+)
          {
             if (PSGsong.speed>1)
             {
                 PSGsong.speed--;
                 VPrintNumber(7,21, bpms[PSGsong.speed], 3);
             }
          }
          if (column==11)//(-)
          {
             if (PSGsong.speed<10)
             {
                 PSGsong.speed++;
                 VPrintNumber(7,21, bpms[PSGsong.speed], 3);
             }
          }
        }
        
      }// end - control de pulsacion
      
      
      // aceleracion
      if (keyAcel<60) keyAcel++;
      keySpeed = keyAcel/10;
      
      
      if (line==7)
      {
        // Frequency channel A 4095
        if (column==9)//(-)
        {
          if (PSGsong.freqA>keySpeed) PSGsong.freqA-=keySpeed;            
          else PSGsong.freqA=0;
          setFreqA(); 
        }
        if (column==8)//(+)
        {
          if (PSGsong.freqA<(4095-keySpeed)) PSGsong.freqA+=keySpeed;
          else PSGsong.freqA=4095;
          setFreqA();         
        }
        
        // Frequency channel B (4095)
        if (column==20)//(-)
        {
          if (PSGsong.freqB>keySpeed) PSGsong.freqB-=keySpeed;
          else PSGsong.freqB=0;
          setFreqB();
        }
        if (column==19)//(+)
        {
          if (PSGsong.freqB<(4095-keySpeed)) PSGsong.freqB+=keySpeed;            
          else PSGsong.freqB=4095;
          setFreqB();
        }
        
        // Frequency channel C (4095)
        if (column==30)//(-)
        {
          if (PSGsong.freqC>keySpeed) PSGsong.freqC-=keySpeed;
          else PSGsong.freqC=0;
          setFreqC();
        }
        if (column==29)//(+)
        {
          if (PSGsong.freqC<(4095-keySpeed)) PSGsong.freqC+=keySpeed;
          else PSGsong.freqC=4095;
          setFreqC();
        }
      }
      
      
        
      // Period. Envelope Frequency (65535)
      if (line==14)
      {
        if (column==26)//(-)
        {
           if (PSGsong.envelopeFreq>keySpeed) PSGsong.envelopeFreq-=keySpeed;
           else PSGsong.envelopeFreq = 0;
           VPrintNumber(20,14, PSGsong.envelopeFreq, 5);
           setEnvelopeFreq();
        }
        if (column==25)//(+)
        {
           if (PSGsong.envelopeFreq<(4095-keySpeed)) PSGsong.envelopeFreq+=keySpeed;
           else PSGsong.envelopeFreq=4095;
           VPrintNumber(20,14, PSGsong.envelopeFreq, 5);
           setEnvelopeFreq();
        }
      }  
        
        
    }else{
      trigbool = false;
      keyAcel=4;
    }
    // end boton1
    
    
    
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    //   keyboard control
    pressKey = GetKeyMatrix(6);
    
    
    // <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
    // boton2 de joystick/raton o tecla Graph
    if ((trigController>0 && STRIG(trigController+2)<0) || (pressKey|0xFB)==0xFB)
    {
      if (trig2bool==false)
      {
        trig2bool = true;
        line = y/8;
        column = x/8;
        
        if (column>14 && column<31)
        {
          //  pattern channel A
          if  (line==19)
          {
            tiladdre = GetVRAMaddressByPosition(column,19);
            tmpValue = PSGsong.patterns[currentPattern].trackA[column-15];
            if (tmpValue>0) tmpValue=180;
            else tmpValue=186;
            VPOKE(tiladdre,tmpValue);
            PSGsong.patterns[currentPattern].trackA[column-15]=tmpValue-180;
          }      
     
          // pattern channel B
          if  (line==20)
          {
            tiladdre = GetVRAMaddressByPosition(column,20);          
            tmpValue = PSGsong.patterns[currentPattern].trackB[column-15];
            
            if (tmpValue>0) tmpValue=180;
            else tmpValue=186;
            
            VPOKE(tiladdre,tmpValue);
            PSGsong.patterns[currentPattern].trackB[column-15]=tmpValue-180;
            
          }
        
          //  pattern channel C
          if (line==21)
          {
            tiladdre = GetVRAMaddressByPosition(column,21);          
            tmpValue = PSGsong.patterns[currentPattern].trackC[column-15];
            if (tmpValue>0) tmpValue=180;
            else tmpValue=186;
            VPOKE(tiladdre,tmpValue);
            PSGsong.patterns[currentPattern].trackC[column-15]=tmpValue-180;
          }
        }
      }    
    }else{
      trig2bool = false;
    }
    // end boton2
    
    
  
    
    if ((pressKey|0xDF)==0xDF) //F1
    { 
      Help();
    }
        
    if ((pressKey|0xBF)==0xBF) //F2
    {
      config(); 
    }
    
    if ((pressKey|0x7F)==0x7F) //F3
    {
      isLoad = true; 
    } 
    

    pressKey = GetKeyMatrix(7);
    if ((pressKey|0x7F)==0x7F) //RETURN Play song
    {
      if (playsngkeybool==false)
      {
        playsngkeybool = true;
        // play/pause song
        if (isPlay>0){
          isPlay=0;
          isPause=1;
          pauseTimer=0;
          VPOKE(0x1A1E,245);//play icon pattern
          VPOKE(0x1AFA,245);//play icon             
          toSilencePSG();            
        }else{
          setPosition();
          isPlay=1;
          isPause=0;
          //isPlayPattern=true;            
          VPOKE(0x1AFA,247);//icono pause
          VPOKE(0x1A1E,245);
        }
      }       
    }else
    {
      playsngkeybool = false;
    }
    
    if ((pressKey|0xBF)==0xBF) //SELECT  (PLAY Pattern)
    {
      if (playpatkeybool==false)
      {
        playpatkeybool = true;   
        // play/pause Pattern
        if (isPlay>0){
          //isPlayPattern=false;
          isPlay=0;
          isPause=3;
          pauseTimer=0;
          VPOKE(0x1A1E,245);//play icon pattern
          VPOKE(0x1AFA,245);//play icon            
          toSilencePSG();            
        }else{
          //isPlayPattern=true;
          isPlay=2;
          isPause=0;
          pauseTimer=0;            
          VPOKE(0x1A1E,247);//pause icon
          VPOKE(0x1AFA,245);
        }
      }       
    }else
    {
      playpatkeybool = false;
    }
       
    if ((pressKey|0xEF)==0xEF) //STOP
    {
      if (stopkeybool==false)
      {
        stopkeybool = true;
        if (isPlay>0)
        {
          isStop=true;
        }else{
          if(isPause>0) // si se pulsa stop en modo pausa
            {
              stopSong();
              /*isPlay=0;
              isPause=0;
              setStop=false;
              playCounter=0;
              tempoCounter=0;
              setLastPos(lastPlayCounter, PSGsong.patterns[currentPattern].trackA, PSGsong.patterns[currentPattern].trackB, PSGsong.patterns[currentPattern].trackC);
              lastPlayCounter=15;
              VPOKE(0x1AFA,245);
              VPOKE(0x1A1E,245);*/
            }else{ // si esta parado, coloca la secuencia en la posicion inicial.
              toSilencePSG();
              songPos=0;
              setPosition(); 
            }          
        }
      }
    }else
    {
      stopkeybool = false;
    }
    
    /*if ((pressKey|0xFE)==0xFE) //F4
    {              
    } */
    
    if ((pressKey|0xFD)==0xFD) //F5
    {
      PSGdump();
    }
    

    pressKey = GetKeyMatrix(0);
       
    if ((pressKey|0xFD)==0xFD) //[1] mute channel A on/off
    {
      if (chanAkeybool==false)
      {
        chanAkeybool = true;
        if (chanA==true){
          VPOKE(0x1A6E,188);//14,19
          chanA=false;
          SetVolume(AY_Channel_A,0);
          //sound_set(8,0);
        }else{
          VPOKE(0x1A6E,187);
          chanA=true;              
        }
       }     
    }else{
      chanAkeybool = false;    
    }
    
    if ((pressKey|0xFB)==0xFB) //[2] mute channel B on/off
    {
      if (chanBkeybool==false)
      {
        chanBkeybool = true;
        if (chanB==true){
          VPOKE(0x1A8E,188);//14,20
          chanB=false;
          SetVolume(AY_Channel_B,0);
          //sound_set(9,0);
        }else{
          VPOKE(0x1A8E,187);
          chanB=true;              
        }
      }     
    }else{
      chanBkeybool = false;    
    }
    
    if ((pressKey|0xF7)==0xF7) //[3] mute channel C on/off
    {
      if (chanCkeybool==false)
      {
        chanCkeybool = true;
        if (chanC==true){
          VPOKE(0x1AAE,188);//14,21
          chanC=false;
          SetVolume(AY_Channel_C,0);
          //sound_set(10,0);
        }else{
          VPOKE(0x1AAE,187);
          chanC=true;              
        }
      } 
    }else{
      chanCkeybool = false;    
    }
    
    pressKey = GetKeyMatrix(8);
    if ((pressKey|0xFB)==0xFB) //INS
    {
      if (playPSGkeybool==false)
      {
        playPSGkeybool=true;
        if (isPlay==0) playPSGregs();
      } 
    }else{
      playPSGkeybool=false;
    }
    // end keycontrol
    // #######################
    
    
    
    // lanza la funcionalidad del load
    if (isLoad==true)
    {
      isLoad=false;
      
      toSilencePSG();          
      
      if(isPlay==2) // si esta en play, lo para
      {
        isPlay=0;
        //isPlayPattern=false;
        playCounter=0;
        tempoCounter=0;
        setLastPos(lastPlayCounter, PSGsong.patterns[currentPattern].trackA, PSGsong.patterns[currentPattern].trackB, PSGsong.patterns[currentPattern].trackC);
        lastPlayCounter=15;
        VPOKE(0x1AFA,245);
        VPOKE(0x1A1E,245);
      }             
      
      //guada la venta principal
      CopyFromVRAM(BASE10,(uint) SCR_BUFFER,0x300);
      response=load();
      //recupera la ventana principal
      CopyToVRAM((uint) SCR_BUFFER,BASE10,0x300);
      
      if (response==true)
      {
        songPos=0;
        currentPattern=0;
        //setPSGdata();
        ShowEditData();
        initPSGregs();
        envelopIndex = getEnvelopeIndex(PSGsong.envelopeType);                
      }
    
    }
    
    //chekend();
    //wait_retrace();   
    
  }
  //return; 
}// end mainWindow


// ===========================================================================
//  muestra la informacion de secuencia y pattern a partir de la posicion
// ===========================================================================
void setPosition()
{
  currentPattern = PSGsong.sequence[songPos];
  VPrintNumber(8,18, songPos+1, 2);
  VPrintNumber(27,17, currentPattern+1, 2);
  VPrintNumber(8,19, currentPattern+1, 2);              
  showPatterns(currentPattern);
} // end setPosition



/* ===========================================================================
playSong                                
Function : Gestiona la reproduccion de una cancion.
Input    : -
Output   : -
=========================================================================== */
void playSong()
{

  if (tempoCounter>PSGsong.speed)
  {
    tempoCounter=0;
    
    playStep();
    
    //lastPlayCounter = playCounter;
    playCounter++;
    
    // control de final del pattern
    if (playCounter==16)
    {
      playCounter=0;
      if (isStop==true)
      {
        isStop=false;
        setStop=true;
      }else{
        //if(isPlay==true)
        //{
          songPos++;
          if (songPos>=PSGsong.length)
          {
            if(isLoop==false) setStop=true;
            songPos=0;
          }                
        //}
      }
    }        
  
  
  }else
  {
     tempoCounter++;
  } 
} // end playSong


void playPattern()
{

  if (tempoCounter>PSGsong.speed)
  {
    tempoCounter=0;
    
    playStep();
    
    playCounter++;
    
    // control de final del pattern
    if (playCounter==16)
    {
      playCounter=0;
      if (isStop==true)
      {
        isStop=false;
        setStop=true;
      }
    }                    
  
  }else
  {
     tempoCounter++;
  }   
} // end playPattern


void playStep()
{
  boolean playEnvelopNote=false;
  char tmpValue=0;
  int offsetFreq=0;
    

    //channelA
  
    tmpValue = PSGsong.patterns[currentPattern].trackA[playCounter];
    if (tmpValue>0)
    { 
      if(chanA==true)
      {
        offsetFreq=PSGsong.freqA;
        
        if (tmpValue==2) offsetFreq += 200;
        if (tmpValue==3) offsetFreq += 400;
        if(offsetFreq>4095) offsetFreq=4095;
        
        //00,01 = Frequency channel A
        SetTonePeriod(AY_Channel_A,offsetFreq);
        //sound_set(0,offsetFreq & 0xFF);
        //sound_set(1,(offsetFreq & 0xFF00)/255);
        
        SetChannel(AY_Channel_A,PSGsong.toneA,PSGsong.noiseA);
        
        //08 = Volume channel A (0-15, 16=Envelope)
        if (tmpValue==6)
        {
          SetVolume(AY_Channel_A,0); //sound_set(8,0);
        }else{
          if (PSGsong.envelopeA==true){
            SetVolume(AY_Channel_A,16); //sound_set(8,16);
            playEnvelopNote = true;        
          }else SetVolume(AY_Channel_A,PSGsong.ampA); //sound_set(8,PSGsong.ampA);
        }             
      }   
    }
            
   
    tmpValue = PSGsong.patterns[currentPattern].trackB[playCounter];
    if (tmpValue>0)
    {
      if(chanB==true)
      {
        offsetFreq=PSGsong.freqB;
        
        if (tmpValue==2) offsetFreq += 200;
        if (tmpValue==3) offsetFreq += 400;
        if(offsetFreq>4095) offsetFreq=4095;
        
        //02,03 = Frequency channel B
        SetTonePeriod(AY_Channel_B,offsetFreq);
        //sound_set(2,offsetFreq & 0xFF);
        //sound_set(3,(offsetFreq & 0xFF00)/255);
        
        SetChannel(AY_Channel_B,PSGsong.toneB,PSGsong.noiseB);
        
        //09 = Volume channel B (0-15, 16=Envelope)
        if (tmpValue==6)
        {
          SetVolume(AY_Channel_B,0);  //sound_set(9,0);
        }else{
          if (PSGsong.envelopeB==true){
            SetVolume(AY_Channel_B,16);  //sound_set(9,16);
            playEnvelopNote = true;        
          }else SetVolume(AY_Channel_B,PSGsong.ampB);  //sound_set(9,PSGsong.ampB);          
        }              
      }             
    }
            
    //channelC
    
    tmpValue = PSGsong.patterns[currentPattern].trackC[playCounter];
    if (tmpValue>0)
    {
      if(chanC==true)
      {
        offsetFreq=PSGsong.freqC;
        
        if (tmpValue==2) offsetFreq += 200;
        if (tmpValue==3) offsetFreq += 400;
        if(offsetFreq>4095) offsetFreq=4095;
        
        //04,05 = Frequency channel C
        SetTonePeriod(AY_Channel_C,offsetFreq);
        //sound_set(4,offsetFreq & 0xFF);
        //sound_set(5,(offsetFreq & 0xFF00)/255);
        
        SetChannel(AY_Channel_C,PSGsong.toneC,PSGsong.noiseC);
        
        //0A = Volume channel C (0-15, 16=Envelope)
        if (tmpValue==6)
        {
          SetVolume(AY_Channel_C,0); //sound_set(10,0);
        }else{
          if (PSGsong.envelopeC==true){
            SetVolume(AY_Channel_C,16); //sound_set(10,16);
            playEnvelopNote = true;                    
          }else SetVolume(AY_Channel_C,PSGsong.ampC); //sound_set(10,PSGsong.ampC);
        }
      }  
    }
    
    // control del disparo de sonidos
    if (playEnvelopNote==true){
      if (PSGsong.patterns[currentPattern].trackA[playCounter]==0)
      {
        if (PSGsong.envelopeA==true) SetVolume(AY_Channel_A,0); //sound_set(8,0);
      }
      if (PSGsong.patterns[currentPattern].trackB[playCounter]==0)
      {
        if (PSGsong.envelopeB==true) SetVolume(AY_Channel_B,0); //sound_set(9,0);
      }
      if (PSGsong.patterns[currentPattern].trackC[playCounter]==0)
      {
        if (PSGsong.envelopeC==true) SetVolume(AY_Channel_C,0); //sound_set(10,0);
      }
      
      //0D = Envelope shape (0-15)
      PlayEnvelope(PSGsong.envelopeType); //sound_set(13,PSGsong.envelopeType);  
    }
 
}


void stopSong()
{
  toSilencePSG();
  setStop=false;          
  isPlay=0;
  isPause=0;
  playCounter=0;
  setLastPos(lastPlayCounter, PSGsong.patterns[currentPattern].trackA, PSGsong.patterns[currentPattern].trackB, PSGsong.patterns[currentPattern].trackC);
  lastPlayCounter=15;
  VPOKE(0x1AFA,245);
  VPOKE(0x1A1E,245);
}



// ===========================================================================
// initialize PSG registers
// ===========================================================================
void initPSGregs()
{
  SetNoisePeriod(PSGsong.noiseFreq);  //sound_set(6,PSGsong.noiseFreq);
  setEnvelopeFreq();
  /*sound_set(11,PSGsong.envelopeFreq & 0xFF);
  sound_set(12,(PSGsong.envelopeFreq & 0xFF00)/255);*/
}


// ===========================================================================
// silence
// ===========================================================================
void toSilencePSG()
{
  SetVolume(AY_Channel_A,0);
  SetVolume(AY_Channel_B,0);
  SetVolume(AY_Channel_C,0);
  
  //sound_set(8,0);
  //sound_set(9,0);
  //sound_set(10,0);
}


void setAmpA()
{
  VPrintNumber(6,10, PSGsong.ampA, 2);
  if (isPlay==0) setEnvelopeA();
}


void setAmpB()
{
  VPrintNumber(17,10, PSGsong.ampB, 2);
  if (isPlay==0) setEnvelopeB();
}


void setAmpC()
{
  VPrintNumber(27,10, PSGsong.ampC, 2);
  if (isPlay==0) setEnvelopeC();
}

void setEnvelopeA()
{
  if (PSGsong.envelopeA==true) SetVolume(AY_Channel_A,16); //sound_set(8,16);       
  else SetVolume(AY_Channel_C,PSGsong.ampA); //sound_set(8,PSGsong.ampA);
}


void setEnvelopeB()
{
  if (PSGsong.envelopeB==true) SetVolume(AY_Channel_B,16); //sound_set(9,16);       
  else SetVolume(AY_Channel_B,PSGsong.ampB); //sound_set(9,PSGsong.ampB);
}


void setEnvelopeC()
{
  if (PSGsong.envelopeC==true) SetVolume(AY_Channel_C,16); //sound_set(10,16);       
  else SetVolume(AY_Channel_C,PSGsong.ampC); //sound_set(10,PSGsong.ampC);
}


//
void setFreqA()
{
  if(PSGsong.freqA>4095) PSGsong.freqA=4095;
  //if(PSGsong.freqA<0) PSGsong.freqA=0;
  VPrintNumber(4,7, PSGsong.freqA, 4);
  //if (isPlay==0)
  SetTonePeriod(AY_Channel_A,PSGsong.freqA);
  //sound_set(0,PSGsong.freqA & 0xFF);
  //sound_set(1,(PSGsong.freqA & 0xFF00)/255);
}


void setFreqB()
{
  if(PSGsong.freqB>4095) PSGsong.freqB=4095;
  //if(PSGsong.freqB<0) PSGsong.freqB=0;
  VPrintNumber(15,7, PSGsong.freqB, 4);
  //if (isPlay==0)
  SetTonePeriod(AY_Channel_B,PSGsong.freqB);
  //sound_set(2,PSGsong.freqB & 0xFF);
  //sound_set(3,(PSGsong.freqB & 0xFF00)/255);
}


void setFreqC()
{
  if(PSGsong.freqC>4095) PSGsong.freqC=4095;
  //if(PSGsong.freqC<0) PSGsong.freqC=0;
  VPrintNumber(25,7, PSGsong.freqC, 4);
  //if (isPlay==0)
  SetTonePeriod(AY_Channel_C,PSGsong.freqB);
  //sound_set(4,PSGsong.freqC & 0xFF);  
  //sound_set(5,(PSGsong.freqC & 0xFF00)/255);
}


void setEnvelopeFreq()
{
    SetEnvelopePeriod(PSGsong.envelopeFreq);
  //sound_set(11,PSGsong.envelopeFreq & 0xFF);
  //sound_set(12,(PSGsong.envelopeFreq & 0xFF00)/255);
}


// ===========================================================================
// inicializa los datos del PSG
// ===========================================================================
void initPSGData()
{
  char i,o;
  
  // inicializa datos del PSG
  PSGsong.freqA=1000;
  PSGsong.freqB=1000;
  PSGsong.freqC=1000;
  PSGsong.ampA=0;
  PSGsong.ampB=0;
  PSGsong.ampC=0;
  PSGsong.toneA=false;
  PSGsong.toneB=false;
  PSGsong.toneC=false;
  PSGsong.noiseA=false;
  PSGsong.noiseB=false;
  PSGsong.noiseC=false;
  PSGsong.noiseFreq=0;
  PSGsong.envelopeA=false;
  PSGsong.envelopeB=false;
  PSGsong.envelopeC=false;  
  PSGsong.envelopeFreq=1000;
  PSGsong.envelopeType=1;
  // sequencer
  PSGsong.speed = 4;
  PSGsong.length = 1;
                                                   
  for (o=0;o<16;o++)
  {  
    for (i=0;i<16;i++)
    { 
      PSGsong.patterns[o].trackA[i]=0;
      PSGsong.patterns[o].trackB[i]=0;
      PSGsong.patterns[o].trackC[i]=0;
    }
  }
      
  for (i=0;i<32;i++) PSGsong.sequence[i]=0;
  
  for (i=0;i<20;i++) PSGsong.name[i]=32;
  for (i=0;i<20;i++) PSGsong.author[i]=32;
}


// ===========================================================================
// initialize screen controllers
// ===========================================================================
void ShowEditData()
{
  char index;
  // inicializo valores de la pantalla
  VPrintNumber(4,7, PSGsong.freqA, 4);
  VPrintNumber(15,7, PSGsong.freqB, 4);
  VPrintNumber(25,7, PSGsong.freqC, 4);
  
  VPrintNumber(6,10, PSGsong.ampA, 2);
  VPrintNumber(17,10, PSGsong.ampB, 2);
  VPrintNumber(27,10, PSGsong.ampC, 2);   
  
  showCheckBox(7,5,PSGsong.toneA);
  showCheckBox(7,8,PSGsong.noiseA);
  showCheckBox(7,9,PSGsong.envelopeA);
  
  showCheckBox(18,5,PSGsong.toneB);
  showCheckBox(18,8,PSGsong.noiseB);
  showCheckBox(18,9,PSGsong.envelopeB);
  
  showCheckBox(28,5,PSGsong.toneC);
  showCheckBox(28,8,PSGsong.noiseC);
  showCheckBox(28,9,PSGsong.envelopeC);
  
  VPrintNumber(8,13, PSGsong.noiseFreq, 2);
  
  index = getEnvelopeIndex(PSGsong.envelopeType);
  ShowEnvelope(index);
  
  VPrintNumber(20,14, PSGsong.envelopeFreq, 5);
    
  VPrintNumber(7,21, bpms[PSGsong.speed], 3);
  
  VPrintNumber(8,18, 1, 2);
  VPrintNumber(8,19, PSGsong.sequence[0]+1, 2);
  VPrintNumber(27,17, 1, 2);
  VPrintNumber(8,20, PSGsong.length, 2);  
  
  showPatterns(currentPattern);
    
}


// ===========================================================================
//
// ===========================================================================
/*void setPSGdata()
{                                                    
  uint address = DATA_BUFFER;
  char value=0,i,o;

  address+=6;
  
  for (i=0;i<20;i++) PSGsong.name[i]=PEEK(address++);
  for (i=0;i<20;i++) PSGsong.author[i]=PEEK(address++);
  
  // inicializa datos del PSG
  PSGsong.freqA=PEEKW(address);
  address += 2;
  PSGsong.freqB=PEEKW(address);
  address += 2;
  PSGsong.freqC=PEEKW(address);
  address += 2;
  PSGsong.noiseFreq=PEEK(address++);
  
  value = PEEK(address++);
  
  if ((value&0b00000001)==0b00000001) PSGsong.toneA=true;
  else PSGsong.toneA=false;
  
  if ((value&0b00000010)==0b00000010) PSGsong.toneB=true;
  else PSGsong.toneB=false;
  
  if ((value&0b00000100)==0b00000100) PSGsong.toneC=true;
  else PSGsong.toneC=false;
  
  if ((value&0b00001000)==0b00001000) PSGsong.noiseA=true;
  else PSGsong.noiseA=false;
  
  if ((value&0b00010000)==0b00010000) PSGsong.noiseB=true;
  else PSGsong.noiseB=false;
  
  if ((value&0b00100000)==0b00100000) PSGsong.noiseC=true;
  else PSGsong.noiseC=false;

  value = PEEK(address++);
  if (value<16){
    PSGsong.ampA = value;
    PSGsong.envelopeA=false;
  }else{
    PSGsong.ampA = 0;
    PSGsong.envelopeA=true;  
  }
  
  value = PEEK(address++);
  if (value<16){
    PSGsong.ampB = value;
    PSGsong.envelopeB=false;
  }else{
    PSGsong.ampB = 0;
    PSGsong.envelopeB=true;  
  }
  
  value = PEEK(address++);
  if (value<16){
    PSGsong.ampC = value;
    PSGsong.envelopeC=false;
  }else{
    PSGsong.ampC = 0;
    PSGsong.envelopeC=true;  
  }  
   
  PSGsong.envelopeFreq=PEEKW(address);
  address += 2;
    
  PSGsong.envelopeType=PEEK(address++);  
    
  PSGsong.speed = PEEK(address++);
  
  PSGsong.length = PEEK(address++);
  
  for (i=0;i<32;i++) PSGsong.sequence[i]=PEEK(address++);
  
  for (o=0;o<16;o++)
  {
    for (i=0;i<16;i++)  PSGsong.patterns[o].trackA[i]=PEEK(address++);
    for (i=0;i<16;i++)  PSGsong.patterns[o].trackB[i]=PEEK(address++);
    for (i=0;i<16;i++)  PSGsong.patterns[o].trackC[i]=PEEK(address++);
  }
  // *tempo = PSGdata.speed + 2;


  return;
}*/


// ===========================================================================
//
// ===========================================================================
void showPatterns(char numPattern)
{
  showPattern(15,19,PSGsong.patterns[numPattern].trackA);
  showPattern(15,20,PSGsong.patterns[numPattern].trackB);
  showPattern(15,21,PSGsong.patterns[numPattern].trackC);
}


// ===========================================================================
//
// ===========================================================================
void showPattern(char column, char line, char *patternData)
{
  int tiladdre;
  char i;  
  tiladdre = GetVRAMaddressByPosition(column,line);
  for (i=0;i<16;i++)
  {
    VPOKE(tiladdre++,180+patternData[i]);
  }
  return;  
}



/*void setChannel(char NumChannel, boolean isTone, boolean isNoise)
{
  char newValue;
  newValue = sound_get(7);
  if(NumChannel==0) 
  {
      if(isTone==true){newValue&=254;}else{newValue|=1;}
      if(isNoise==true){newValue&=247;}else{newValue|=8;}
  }
  if(NumChannel==1)    
  {
      if(isTone==true){newValue&=253;}else{newValue|=2;}
      if(isNoise==true){newValue&=239;}else{newValue|=16;}
  }
  if(NumChannel==2)
  { 
      if(isTone==true){newValue&=251;}else{newValue|=4;}
      if(isNoise==true){newValue&=223;}else{newValue|=32;}
  }
  sound_set(7,newValue);
}*/


// ===========================================================================
//
// ===========================================================================
void setLastPos(char value, char *patternA, char *patternB, char *patternC)
{
  char position;
  uint tiladdre;
  
  position = 15+value; 
  tiladdre = GetVRAMaddressByPosition(position,19);
  VPOKE(tiladdre,180+patternA[value]);
  
  tiladdre = GetVRAMaddressByPosition(position,20);
  VPOKE(tiladdre,180+patternB[value]);
  
  tiladdre = GetVRAMaddressByPosition(position,21);
  VPOKE(tiladdre,180+patternC[value]);
        
  return;
}




/* =============================================================================
Show envelope wave selected in combobox
============================================================================= */
char ShowEnvelope(char index)
{
  uint offset;  
  
  offset = index*6;
  
  CopyToVRAM((uint) EnvCombobox + offset, BASE10 + (32*13)+20, 5);     //20,13
  
  return EnvelopeValue[index];
}



// ShowCombo(24,13);
signed char ShowCombo(char posx, char posy)
{
  char i,o;
  uint dpos=0;
  uint tiladdre;  
  uint vpos;
  char line=0, column=0;
  char pressKey;
  signed char isOut=0;

  tiladdre = GetVRAMaddressByPosition(posx,posy);
  vpos = tiladdre;

  CopyFromVRAM(tiladdre,(uint) SCR_BUFFER,0x140);

  for(i=0;i<9;i++)
  {
    for(o=0;o<6;o++) VPOKE(vpos++, EnvCombobox[dpos++]);
    vpos += 26;
  }
  
  while(1)
  {
    HALT;
    //if (isPlay==1) PlaySong();
    if (isPlay>0)
    {
      if (isPlay==1)
      {
        playSong();    
      }else{
        playPattern();
      }
    }
    
    if(playCounter != lastPlayCounter)// muestra info en la pantalla
    {            
      if (setStop==true)
      {
        setStop=false;
        toSilencePSG();          
        isPlay=0;          
      } 
    }
    
    pointerController();
    //showPointer();
    
    
    //control del boton de accion
    if (STRIG(trigController)<0)
    {
      line = y/8;
      column = x/8;
      
      /*if (column==6 && line==8)
      {
         isOut=2;                
      }*/
      if (column>posx-1 && column<posx+5 && line>posy-1 && line<posy+8)//ok
      {
         isOut=line-posy;
         break;                 
      }
    }
    
    // control del teclado
    pressKey = GetKeyMatrix(7);
    if ((pressKey|0x7F)==0x7F) //RETURN
    {
      isOut=1;
      break;  
    }
    if ((pressKey|0xFB)==0xFB) //ESC
    {
      isOut=-1;
      break; 
    }
  }
  
  CopyToVRAM((uint) SCR_BUFFER,tiladdre,0x140);
       
  return isOut;
}


// ===========================================================================
//
// ===========================================================================
/*char getEnvelopeByIndex(char index)
{
  int value=1;  
  switch (index) 
  {
    case 0:
      value = 1;  
      break;
    case 1:
      value = 4;  
      break;
    case 2:
      value = 8;  
      break;
    case 3:
      value = 10;  
      break;
    case 4:
      value = 11;  
      break;
    case 5:
      value = 12;  
      break;
    case 6:
      value = 13;  
      break;
    case 7:
      value = 14;  
      break;
  }
  return value;
}*/


// ===========================================================================
//
// ===========================================================================
char getEnvelopeIndex(char envelope)
{
  char value=0;
  switch (envelope) 
  {
    case 1:      
      value = 0;  
      break;
    case 4:
      value = 1;  
      break;
    case 8:
      value = 2;  
      break;
    case 10:
      value = 3;  
      break;
    case 11:
      value = 4;  
      break;
    case 12:
      value = 5;  
      break;
    case 13:
      value = 6;  
      break;
    case 14:
      value = 7;  
      break;
  }
  return value;
}


// ===========================================================================
//
// ===========================================================================
boolean reverseCheckBox(boolean checked, int tiladdre)
{  
  if (checked==true)
  {
    VPOKE(tiladdre++,189);
    VPOKE(tiladdre++,223);
    VPOKE(tiladdre,222);
    return false;
  }else{
    VPOKE(tiladdre++,190);
    VPOKE(tiladdre++,222);
    VPOKE(tiladdre,223);
    return true;
  }
}


// ===========================================================================
//
// ===========================================================================
void showCheckBox(char column, char line, boolean checked)
{
  int tiladdre;  
  tiladdre = GetVRAMaddressByPosition(column,line);
  if (checked==false)
  {
    VPOKE(tiladdre++,189);
    VPOKE(tiladdre++,223);
    VPOKE(tiladdre,222);
  }else{
    VPOKE(tiladdre++,190);
    VPOKE(tiladdre++,222);
    VPOKE(tiladdre,223);
  }
  return;
}


// ===========================================================================
// Borra la pantalla a partir de un color indicado 
// ===========================================================================
void FastClsSc2(char color)
{
  FillVRAM(6144, 768, 32);
  FillVRAM(8448, 9, color);
  FillVRAM(10496, 9, color);
  FillVRAM(12544, 9, color);
  return;
}





void config()
{
  char line=0, column=0, numlin=0;
  char pressKey;
  char isOut=0;
  char i=0;
  //char t_PSG_type = getPSGtype();
  char t_VDP_type = VDP_type;
  char t_controller = controller;
  
  toSilencePSG();
  
  CopyFromVRAM(0x1864,(uint) SCR_BUFFER,0x21C);
  ShowWindow(4,3,24,17,(uint) config_WIN);
  //ShowConfigWin();
  
  if(VDP_type==0) VPOKE(0x18CD,190);
  else VPOKE(0x18ED,190);
  
  if(isAYextern==false) VPOKE(0x192D,190);
  else VPOKE(0x194D,190);
  
  if(controller==0) VPOKE(0x198D,190);
  if(controller==1) VPOKE(0x19AD,190);
  if(controller==2) VPOKE(0x19CD,190);
  if(controller==3) VPOKE(0x19ED,190);
  if(controller==4) VPOKE(0x1A0D,190);
     
  while(isOut == 0)
  {
    HALT;
    pointerController();
    //showPointer();
    
    //control del boton de accion
    if (STRIG(trigController)<0)
    {
      line = y/8;
      column = x/8;
      
      if (column>12 && column<21 && line==6)// VDP TMS9918
      {
         VPOKE(0x18CD,190);
         VPOKE(0x18ED,189);
         t_VDP_type=0;       
      }
      if (column>12 && column<19 && line==7)// VDP V9938+
      {
         VPOKE(0x18CD,189);
         VPOKE(0x18ED,190);
         t_VDP_type=1;      
      }
      
      if (column>12 && column<22 && line==9)// PSG internal
      {
         VPOKE(0x192D,190);
         VPOKE(0x194D,189);
         isAYextern = false;       
      }
      if (column>12 && column<27 && line==10)// PSG MEGAFLASHROM SCC+
      {
         VPOKE(0x192D,189);
         VPOKE(0x194D,190);
         isAYextern = true;       
      }
      
      if (column>12 && column<25 && line==12)// control key
      {
         VPOKE(0x198D,190);
         VPOKE(0x19AD,189);
         VPOKE(0x19CD,189);
         VPOKE(0x19ED,189);
         VPOKE(0x1A0D,189);
         t_controller=0;        
      }
      if (column>12 && column<24 && line==13)// control joy
      {
         VPOKE(0x198D,189);
         VPOKE(0x19AD,190);
         VPOKE(0x19CD,189);
         VPOKE(0x19ED,189);
         VPOKE(0x1A0D,189);
         t_controller=1;       
      }
      if (column>12 && column<24 && line==14)// control mouse
      {
         VPOKE(0x198D,189);
         VPOKE(0x19AD,189);
         VPOKE(0x19CD,190);
         VPOKE(0x19ED,189);
         VPOKE(0x1A0D,189);
         t_controller=2;       
      }
      if (column>12 && column<21 && line==15)// control mouse
      {
         VPOKE(0x198D,189);
         VPOKE(0x19AD,189);
         VPOKE(0x19CD,189);
         VPOKE(0x19ED,190);
         VPOKE(0x1A0D,189);
         t_controller=3;       
      }
      if (column>12 && column<21 && line==16)// control mouse
      {
         VPOKE(0x198D,189);
         VPOKE(0x19AD,189);
         VPOKE(0x19CD,189);
         VPOKE(0x19ED,189);
         VPOKE(0x1A0D,190);
         t_controller=4;       
      }      
      
      if (column==4 && line==3)
      {
         isOut=2;                
      }
      if (column>12 && column<19 && line==18)//ok
      {
         isOut=1;                
      }
      if (column>19 && column<27 && line==18)//cancel
      {
         isOut=2;                
      }
    }
    
    // control del teclado
    pressKey = GetKeyMatrix(7);
    if ((pressKey|0x7F)==0x7F) //RETURN
    {
      isOut=1; 
    }
    if ((pressKey|0xFB)==0xFB) //ESC
    {
      isOut=2; 
    }
    
  }
  
  if (isOut==1)
  {
    //VDP
    if (t_VDP_type!=VDP_type)
    {
      VDP_type=t_VDP_type;
      if (VDP_type==1) SetPalette(1);
      else SetPalette(2);
    }
    
    //PSG
    //setPSGtype(PSG_type);
    initPSGregs();
    
    //CONTROL
    controller=t_controller;
    if (t_controller>2)
    {
      Mouse_port = controller-2;    //POKE(MOUSE,controller-2);
      trigController=controller-2;
    }else{
      trigController=t_controller;
    }
    
  } 
  
  CopyToVRAM((uint) SCR_BUFFER,0x1864,0x21C);
}






void PSGdump()
{
  char line=0, column=0, numlin=0;
  char pressKey;
  boolean trigbool=false;
  boolean playPSGkeybool=false;
  char isOut=0;
  char i=0;
  char newValue=0;
  char t_ampA = PSGsong.ampA;
  char t_ampB = PSGsong.ampB;
  char t_ampC = PSGsong.ampC;
    
  toSilencePSG();
  
  CopyFromVRAM(0x1863,(uint) SCR_BUFFER,0x21C);
  ShowWindow(3,3,26,17,(uint) PSGregs_WIN);
  //ShowPSGDumpWin();
    
  if (PSGsong.envelopeA==true) t_ampA = 16;
  if (PSGsong.envelopeB==true) t_ampB = 16;
  if (PSGsong.envelopeC==true) t_ampC = 16;
                          
  //newValue = sound_get(7);
  if(PSGsong.toneA==true){newValue&=254;}else{newValue|=1;}
  if(PSGsong.noiseA==true){newValue&=247;}else{newValue|=8;}
    
  if(PSGsong.toneB==true){newValue&=253;}else{newValue|=2;}
  if(PSGsong.noiseB==true){newValue&=239;}else{newValue|=16;}

  if(PSGsong.toneC==true){newValue&=251;}else{newValue|=4;}
  if(PSGsong.noiseC==true){newValue&=223;}else{newValue|=32;}
                          
  VPrintNumber(8,7, PSGsong.freqA & 0xFF, 3);
  VPrintNumber(16,7, (PSGsong.freqA & 0xFF00)/255, 3);
  VPrintNumber(25,7, t_ampA, 3);
  
  
  VPrintNumber(8,9, PSGsong.freqB & 0xFF, 3);
  VPrintNumber(16,9, (PSGsong.freqB & 0xFF00)/255, 3);
  VPrintNumber(25,9, t_ampB, 3);
  
  VPrintNumber(8,11, PSGsong.freqC & 0xFF, 3);
  VPrintNumber(16,11, (PSGsong.freqC & 0xFF00)/255, 3);  
  VPrintNumber(25,11, t_ampC, 3);   

  VPrintNumber(16,12, PSGsong.noiseFreq, 3);//reg 6
  
  VPrintNumber(16,13, newValue, 3); //reg7 mixer
  
  VPrintNumber(16,15, PSGsong.envelopeFreq & 0xFF, 3);
  VPrintNumber(25,15, (PSGsong.envelopeFreq & 0xFF00)/255, 3);
  
  VPrintNumber(16,16, PSGsong.envelopeType, 3);//reg 13
      
  while(isOut == 0)
  {
    HALT;
    pointerController();
    //showPointer();
    
    //control del boton de accion
    

      
    
    if (STRIG(trigController)<0)
    {
      if (trigbool==false)
      {
        trigbool = true;
        
        line = y/8;
        column = x/8;        
        
        if (column==3 && line==3)  //exit
        {
           isOut=2;                
        }
        if (column==4 && line==18)  //play
        {
           playPSGregs();                
        }
        if (column==6 && line==18)  //stop
        {
           toSilencePSG();                
        }
        if (column>20 && column<28 && line==18)//cancel
        {
           isOut=2;                
        }
      }
    }else{
      trigbool = false;
    }
    
    // control del teclado
    pressKey = GetKeyMatrix(7);
    
    if ((pressKey|0xEF)==0xEF) //STOP
    {
       toSilencePSG();
    }
    
    if ((pressKey|0xFB)==0xFB) //ESC
    {
      isOut=2; 
    }
    
    pressKey = GetKeyMatrix(8);
    if ((pressKey|0xFB)==0xFB) //INS
    {
      if (playPSGkeybool==false)
      {
        playPSGkeybool=true;
        playPSGregs();
      } 
    }else{playPSGkeybool=false;}
    
  }
  
  toSilencePSG();
  
  CopyToVRAM((uint) SCR_BUFFER,0x1863,0x21C);
}


void playPSGregs()
{
  //char newValue;
  /*char t_ampA = PSGdata.ampA;
  char t_ampB = PSGdata.ampB;
  char t_ampC = PSGdata.ampC;
        
  if (PSGdata.envelopeA==true) t_ampA = 16;
  if (PSGdata.envelopeB==true) t_ampB = 16;
  if (PSGdata.envelopeC==true) t_ampC = 16;*/
  
  /*newValue = sound_get(7);
  if(PSGdata.toneA==true){newValue&=254;}else{newValue|=1;}
  if(PSGdata.noiseA==true){newValue&=247;}else{newValue|=8;}
    
  if(PSGdata.toneB==true){newValue&=253;}else{newValue|=2;}
  if(PSGdata.noiseB==true){newValue&=239;}else{newValue|=16;}

  if(PSGdata.toneC==true){newValue&=251;}else{newValue|=4;}
  if(PSGdata.noiseC==true){newValue&=223;}else{newValue|=32;}*/
  
  
  SetTonePeriod(AY_Channel_A,PSGsong.freqA);
  SetTonePeriod(AY_Channel_B,PSGsong.freqB);
  SetTonePeriod(AY_Channel_C,PSGsong.freqC);
  
  SetNoisePeriod(PSGsong.noiseFreq);  
  
  //sound_set(0,PSGsong.freqA & 0xFF);
  //sound_set(1,(PSGdata.freqA & 0xFF00)/255);  
  //sound_set(2,PSGdata.freqB & 0xFF);
  //sound_set(3,(PSGdata.freqB & 0xFF00)/255);  
  //sound_set(4,PSGdata.freqC & 0xFF);
  //sound_set(5,(PSGdata.freqC & 0xFF00)/255);  
  //sound_set(6,PSGdata.noiseFreq);
    
  setEnvelopeA();
  setEnvelopeB();
  setEnvelopeC();
  
  SetChannel(AY_Channel_A,PSGsong.toneA,PSGsong.noiseA);
  SetChannel(AY_Channel_B,PSGsong.toneB,PSGsong.noiseB);
  SetChannel(AY_Channel_C,PSGsong.toneC,PSGsong.noiseC);
  
  /*sound_set(8,t_ampA);
  sound_set(9,t_ampB);
  sound_set(10,t_ampC);*/

  /*sound_set(11,PSGdata.envelopeFreq & 0xFF);
  sound_set(12,(PSGdata.envelopeFreq & 0xFF00)/255);*/
  
  setEnvelopeFreq();
  
  PlayEnvelope(PSGsong.envelopeType);  //sound_set(13,PSGdata.envelopeType);

}


// ===========================================================================
// funcion que gestiona la ventana que muestra/edita
// el nombre y autor de un song
// ===========================================================================
void info()
{
  char line=0, column=0, numlin=0;
  char pressKey; 
  
  toSilencePSG();
   
  CopyFromVRAM(0x1960,(uint) SCR_BUFFER,0xA0);
  ShowInfoWin();
  
  VPRINTN(8, 13, PSGsong.name,20);
  VPRINTN(8, 14, PSGsong.author,20);
  
  while(1)
  {
    HALT;
    pointerController();
    //showPointer();
    
    //control del boton de accion
    if (STRIG(trigController)<0)
    {
      line = y/8;
      column = x/8;
           
      if (column==0 && line==11)
      {
         break;                
      }
      if (column>7 && column<28 && line==13)
      {
        VPOKE(0x19BC,157);
        vInput(8,13,PSGsong.name,20);
        VPOKE(0x19BC,32);                        
      }
      if (column>7 && column<28 && line==14)
      {
        VPOKE(0x19DC,157);
        vInput(8,14,PSGsong.author,20);
        VPOKE(0x19DC,32);                        
      }
    }
    
    // control del teclado
    pressKey = GetKeyMatrix(7);
    if ((pressKey|0xFB)==0xFB) //ESC
    {
      break;
    }
    
    
  }
  
  CopyToVRAM((uint) SCR_BUFFER,0x1960,0xA0);
  
  return;
}


void vInput(char posx, char posy, char* text, char length)
{
  char aKey;
  char size=0;
  uint posi;
  char cx=0, cy=0;
  
  cx = posx*8;
  cy = (posy*8)-2;
  
  posi = GetVRAMaddressByPosition(posx, posy);
  
  SetSpritePattern(0,31);  
  SetSpritePattern(1,31);
  
  SetSpritePattern(4,4);
  SetSpriteColor(4,6);
  
  KillBuffer();
  
  while(1)
  {        
    //put_sprite(4, cx, cy, 6);// square
    SetSpritePosition(4,cx,cy);
    
    aKey = INKEY();
    //VPrintNumber(11,12, aKey, 3);
    
    /*if (aKey==27)//ESC
    {
      output=2;
    }*/
    if (aKey==13)//RETURN
    {
      break;
    }
    if (aKey==8 && size>0)//BS
    {
      size--;
      posi--;
      cx-=8;
      VPOKE(posi,32);
      text[size]=32; 
    }
    if (aKey==29 && size>0)//left arrow
    {            
      size--;
      posi--;
      cx-=8;
    }
    if (aKey==28 && size<length)//left arrow
    {            
      size++;
      posi++;
      cx+=8;
    }
    if(aKey>31 && aKey<123 && size<length)
    {
      if (aKey>96) aKey-=32; //lowercase
      text[size]=aKey;
      size++;
      cx+=8;
      VPOKE(posi++,aKey);
    }
  }
  //put_sprite(4, 0, 193, 0);
  SetSpritePattern(4,31);
  SetSpritePattern(0,0); 
  SetSpritePattern(1,1);
  return; 
}



/* =============================================================================
 Muestra una ventana modal de confirmacion
 ejem: response=showModalWin("ARE YOU SURE\nYOU WANT TO\nDELETE SONG?");
============================================================================= */
boolean showModalWin(char* aText)
{
  char line=0, column=0;
  char pressKey;
  boolean result = false;
  char i=0;
  
  toSilencePSG();
  
  CopyFromVRAM(0x1906,(uint) SCR_BUFFER,0xD4);
  ShowWindow(6,8,20,7,(uint) confirm_WIN);
  //ShowConfirmWin();
  
  PUTSPRITE(5, 56, 71, 1,5);
  
  VPRINT(10,9,aText);
                                
  
  while(1)
  {
    HALT;
    pointerController();
    //showPointer();
    
    //control del boton de accion
    if (STRIG(trigController)<0)
    {
      line = y/8;
      column = x/8;
      
      if (column>12 && column<19 && line==13){result=true;break;}  //(Yes)

      if (column>19 && column<25 && line==13){result=false;break;} //(No)                

    }
    
    // control del teclado
    pressKey = GetKeyMatrix(7);
    if ((pressKey|0x7F)==0x7F){result=true;break;}  //(Yes) RETURN

    if ((pressKey|0xFB)==0xFB){result=false;break;} //(No) ESC

  }
  
  CopyToVRAM((uint) SCR_BUFFER,0x1906,0xD4);
  
  //put_sprite(5, 0, 193, 0);
  SetSpritePattern(5,31);
 
  return result;
}





/* =============================================================================
copyPatternTool                                
Function : Herramienta que copia patrones. Copy to.
Input    : - 
Output   : -
============================================================================= */
void copyPatternTool()
{
  char line=0, column=0;
  char pressKey;
  char isOut=0;
  char i=0;
  //char sourcePattern=currentPattern+1;
  char newPattern=currentPattern+1;
  boolean trigbool=false;
  
  toSilencePSG();
    
  CopyFromVRAM(0x1A52,(uint) SCR_BUFFER,0x4E);
  ShowCopyWin();
  
  VPrintNumber(22,19, newPattern, 2);
                                
  
  while(isOut == 0)
  {
    HALT;
    pointerController();
    //showPointer();
    
    //control del boton de accion
    if (STRIG(trigController)<0)
    {
      if (trigbool==false)
      {
        trigbool = true;
        line = y/8;
        column = x/8;
              
        if (line==19)
        {          
          if (column==24)//(+)
          {
            if (newPattern<16)
            {
              newPattern++;
              VPrintNumber(22,19, newPattern, 2);
            }
          }
          if (column==25)//(-)
          {
            if (newPattern>1)
            {
              newPattern--;
              VPrintNumber(22,19, newPattern, 2);
            }
          }          
        }
          
        
        if (column==18 && line==18)
        {
           isOut=2;                
        }
        if (column>25 && column<31 && line==19)//ok
        {
           isOut=1;                
        }
      }
    }else{
      trigbool = false;
    }
    
    // control del teclado
    pressKey = GetKeyMatrix(7);
    if ((pressKey|0x7F)==0x7F) //RETURN
    {
      isOut=1; 
    }
    if ((pressKey|0xFB)==0xFB) //ESC
    {
      isOut=2; 
    }
  }
  
  CopyToVRAM((uint) SCR_BUFFER,0x1A52,0x4E);
  
  if (isOut==1)
  {
    newPattern--;
    if (newPattern!=currentPattern)
    {
      for (i=0;i<16;i++)
      { 
        PSGsong.patterns[newPattern].trackA[i]=PSGsong.patterns[currentPattern].trackA[i];
        PSGsong.patterns[newPattern].trackB[i]=PSGsong.patterns[currentPattern].trackB[i];
        PSGsong.patterns[newPattern].trackC[i]=PSGsong.patterns[currentPattern].trackC[i];
      }
      showPatterns(currentPattern);
    }
  }  
     
  return;
}



/* =============================================================================
 muestra un texto en la pantalla de varias lineas separadas con \n
 ejem: VPrintLines(10,10,"ARE YOU SURE\nYOU WANT TO\nDELETE SONG?");
============================================================================= */
void VPRINT(char posx, char posy, char* text)
{
  char character;

  uint vaddr = GetVRAMaddressByPosition(posx,posy); // calcula la posicion en la VRAM

  while(*(text))
  {
    character=*(text++);
    if (character=='\n'){
      posy++;
      vaddr = GetVRAMaddressByPosition(posx,posy);
    }
    else{
      VPOKE(vaddr++,character);
    }    
  }
  return;
}



/* =============================================================================
 Prints a string at the indicated screen position

 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
   text (char*) string
   length (uint) length of the string to print
============================================================================= */
void VPRINTN(char column, char line, char* text, unsigned int length)
{
  unsigned int vaddr = GetVRAMaddressByPosition(column, line);
  char size=0;
  while(text[size]) size++;
  if (length>size) length=size;
  CopyToVRAM((unsigned int) text, vaddr, length);
  //while(*(text) && length-->0)  VPOKE(vaddr++,*(text++));
}



/* =============================================================================
   VPrintNumber
   Prints a number at the specified position on the screen.
   
   Inputs:
     [char] column 0 - 31
     [char] line   0 - 23
     [unsigned int] number
     [char] length
============================================================================= */
void VPrintNumber(char column, char line, unsigned int value, char length)
{
  unsigned int vaddr = GetVRAMaddressByPosition(column, line);

  VPrintNum(vaddr, value, length);
}



/* =============================================================================
   VPrintNum
   Prints a number at the specified position on the screen.
   
   Inputs:
     [unsigned int] VRAM address in Pattern Name Table.
     [unsigned int] number
     [char] length
============================================================================= */
void VPrintNum(unsigned int vaddr, unsigned int value, char length)
{
  char pos=5-length;
  char text[]="     ";

  num2Dec16(value, text); 
  
  //while (length-->0){ VPOKE(vaddr++,text[pos++]);}
  CopyToVRAM((unsigned int) text + (5-length), vaddr, length);
}


// ===========================================================================
// 16-bit Integer to ASCII (decimal)
// by baze 
// Input: HL = number to convert, DE = location of ASCII string
// Output: ASCII string at (DE)
// ===========================================================================
void num2Dec16(int aNumber, char *address) __naked
{
  aNumber;
  address;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld l,4(ix)
  ld h,5(ix)
  
  ld e,6(ix)
  ld d,7(ix)
  	
  ld	bc,#-10000
  call	$Num1
  ld	bc,#-1000
  call	$Num1
  ld	bc,#-100
  call	$Num1
  ld	c,#-10
  call	$Num1
  ld	c,b
  call	$Num1
    
  pop  IX
  ret
    
$Num1:	
  ld	a,#-1 ;ASCII-1 47 
$Num2:	
  inc	a
	add	hl,bc
	jr	c,$Num2
	sbc	hl,bc
	ld	(de),a
	inc	de
	ret
$Num3:	
  
__endasm;
}



/* =============================================================================
 It provides the address of the video memory map tiles, from the screen position
 indicated.

 Inputs:
   column (byte) 0 - 31
   line (byte) 0 - 23
============================================================================= */
unsigned int GetVRAMaddressByPosition(char column, char line)
{
   return BASE10 + (line*32)+column;
}









// ############################################################################# HELP



/* =============================================================================
 Muestra la ventana de ayuda desde la pantalla de principal
============================================================================= */
/*void toHelpFromEdit()
{
  toSilencePSG(); 
  // guarda la pantalla principal     
  CopyFromVRAM(BASE10,(uint) SCR_BUFFER,0x300);
  help();
  //recupera la pantalla principal
  CopyToVRAM((uint) SCR_BUFFER,BASE10,0x300);
}*/


/* =============================================================================
 Muestra la ventana de ayuda desde la pantalla de carga
============================================================================= */
/*void toHelpFromLoad()
{
  CopyFromVRAM(BASE10,(uint) LOAD_SCR_BUFFER,0x300);
  help();
  //recupera la ventana principal
  CopyToVRAM((uint) LOAD_SCR_BUFFER,BASE10,0x300);
}*/


/* ===========================================================================
help                                
Function : Controls the functionality of the help.
Input    : -
Output   : -
=========================================================================== */
void Help()
{
  char line=0, column=0, numlin=0;
  char pressKey;
  //boolean trigbool=false;
  
  toSilencePSG(); 
  
  // guarda la pantalla principal     
  CopyFromVRAM(BASE10,(uint) SCR_BUFFER,0x300);

    
  ShowHelpScreen();
    
  showHelpPage(0);
  //showScrollBar(0, helpLines);
    
  while(1)
  {
    HALT;
    pointerController();
    
    //control del boton de accion
    if (STRIG(trigController)<0)
    {
      //if (trigbool==false)
      //{
        //trigbool = true;
        line = y/8;
        column = x/8;
          
        if (column>29 && line>3 && line<12) // scroll arriba
        {
          if (numlin>0) showHelpPage(--numlin);                 
        }
        
        if (column>29 && line>15 && line<22) // scroll abajo
        {
          if (numlin<(helpLines-18)) showHelpPage(++numlin);                 
        }
        
        if (column>24 && line==23)
        {
           break;                
        }
        
      //}
      
    }else{
      //trigbool = false;
    }
    
    // control del teclado
    pressKey = GetKeyMatrix(7);
    if ((pressKey|0xFB)==0xFB) //ESC
    {
      break;
    }   
  }
  
  SetSpritePattern(2,31);  
  SetSpritePattern(3,31);  
  
  //recupera la pantalla principal
  CopyToVRAM((uint) SCR_BUFFER,BASE10,0x300); 
}







/* ===========================================================================
showScrollBar                                
Function : shows the scroll bar 
Input    : numlin (char) =  position
           maxNumLines (char) = maximum number of lines
Output   : -
=========================================================================== */
void showScrollBar(char numlin, char maxNumLines)
{
  char i, size;
  uint vramAdrs;
  size = (13 / (maxNumLines/18))+1;  // +1 para compensar el redondeo
  
  numlin = 6 + (numlin/(maxNumLines/13)); //
  
  if (numlin>(20-size)){
    return;
  }
  
  // borra la barra
  vramAdrs = BASE10 + (32*6) + 30;
  for (i=0;i<14;i++)
  {        
    VPOKE(vramAdrs,249);
    vramAdrs += 32;
  }

  // muestra la sección de la barra    
  vramAdrs = BASE10 + (32*numlin) + 30; 
  for (i=0;i<size;i++)
  {    
    VPOKE(vramAdrs,248);
    vramAdrs += 32;
  }
  
  return;
}



/* ===========================================================================
initHelpData                                
Function : initialize help data 
Input    :  -
Output   :  -
=========================================================================== */
void initHelpData()
{

  //setHelpData();
  
  //HELP_ADDR = PEEKW(0xE007);
  helpLines = 75;

  //CopyRAM(HELP_ADDR, HELP_DATA, (helpLines*29)+1);

}
// END HELP ##################################################################



// ###########################################################################
// L O A D
// ###########################################################################


/*void initDemoData()
{
  demofiles[0]=*demo01;
  demofiles[1]=*demo02;
  demofiles[2]=*demo03;
} */

/*void initDemoData()
{
// ficheros de demos
// temporal (incoporar en ROM)
unsigned int addressDemo = DEMO_DATA; //0xCB00;     
unsigned int i=0; 

unsigned int demoLength[2]={286,862};
//aorante    894 - 576 - 286

    char *demofiles[]={demo01, demo02};
    int length=FILESIZE*2;
    
    while (length-->0){POKE(addressDemo++,0);}
    
    addressDemo = DEMO_DATA;
    
    for (i=0;i<2;i++)
    {
      //length = getLength(demofiles[i]);
      CopyRAM(addressDemo, (uint) demofiles[i], (uint) demoLength[i]+1);
      addressDemo += FILESIZE;
    }
}*/

/*int getLength(char *str) 
{
    int i = 0;
    while(*(str++)) i++;        //if(i == INT_MAX) return -1;
    return i;
}*/


//
/*void copyMem(int sourceAddress, int toAddress, int size)
{
  while (size-->0)
  { 
    POKE(toAddress++,peek(sourceAddress++)); 
  }
}*/



//
boolean load()
{
  char isOut = 0;
  char selected = 0;
  char pressKey;
  char line=0, column=0;
  boolean trigbool=false;
  boolean escKeybool = false;
  boolean result = false;
  //unsigned char *data;
  //unsigned int addressDemo = DEMO_DATA;
  
  
  ShowLoadScreen();
  
  
  VPRINT(1,4,"›DEMO0001.PSG");
  VPRINT(1,5,"›MINITECH.PSG");
  
  while(isOut == 0)
  {
    HALT;
    pointerController();
    
    //control del boton de accion
    if (STRIG(trigController)<0)
    {
      line = y/8;
      column = x/8;
      
      /*if (column<6 && line==23) //Help
      {
        toHelpFromLoad();  
      }*/
              
      if (column>17 && column<24 && line==23) //OK
      {
        //CopyRAM(addressDemo+(FILESIZE*selected),DATA_BUFFER,FILESIZE+1);
        //CopyRAM(DATA_BUFFER, demofiles[selected], FILESIZE+1);
        isOut = 1;         
      }
      
      
      
      if (trigbool==false)
      {
        trigbool = true;
        
        if (column>24 && line==23) //ESC
        {
          isOut = 2;  
        }
              
        if (column>0 && column<15) 
        {
          if (line==4) 
          {
            VPRINT(2,20,"DEMO001");
            selected=1;         
          }
          if (line==5) 
          {
            VPRINT(2,20,"MINIMAL TECH");
            selected=2;         
          }
          /*if (line==12) 
          {
            VPRINT(2,19,"DEMO-003.PSG");
            selected=2;         
          } */
        }       
        
      }
    
    }else{
      trigbool=false;
    }
        
    // control del teclado
    pressKey = GetKeyMatrix(7);    
    if ((pressKey|0x7F)==0x7F) //RETURN
    {
      //lee el fichero    
      //CopyRAM(addressDemo+(FILESIZE*selected),DATA_BUFFER,FILESIZE+1);
      //CopyRAM(DATA_BUFFER, demofiles[selected], FILESIZE+1);    
      //end
      isOut = 1;
    }
         
    if ((pressKey|0xFB)==0xFB) //ESC
    {
      if (escKeybool==false)
      {
        escKeybool = true;
        isOut = 2;
      }
    }else{escKeybool=false;}

    //pressKey = GetKeyMatrix(6);    
    /*if ((pressKey|0xDF)==0xDF) //F1
    {      
      toHelpFromLoad();
      escKeybool = true;
    }*/    
    
  }

  
  SetSpritePattern(2,31);  
  SetSpritePattern(3,31);
  
  if (selected>0)
  {
    LoadDemo(selected-1);
    result = true;
  }  
  return result;
}



void LoadDemo(char index)
{

  switch (index) 
  {
    case 0:
      unRLEWBtoRAM((uint) DEMO01, (uint) PSGsong); 
      break;
    case 1:
      unRLEWBtoRAM((uint) DEMO02, (uint) PSGsong);
      break;
  }
}




// END LOAD ##################################################################



/* ===========================================================================
pointerController                               
Function:  
=========================================================================== */
void pointerController()
{
  if (controller>2) mouseController(); 
  else joyController();
  
  // control de la posicion del puntero
  if (x<0) x=0;
  if (x>250) x=250;
  if (y<0) y=0;
  if (y>187) y=187;
  
  //showPointer();
  SetSpritePosition(0,x,y);
  SetSpritePosition(1,x,y);
}



void joyController()
{    
  char joyval;

  // control del puntero (cursor o joysctick)
  joyval=STICK(controller);
  if(joyval>0)
  {
    // aceleracion
    if (acel<40) acel++;
    speed = acel/10;      
    
    // movimiento  
    if (joyval>1 && joyval<5) x=x+speed;
    if (joyval>5 && joyval<9) x=x-speed;
    if (joyval>3 && joyval<7) y=y+speed;
    if (joyval>0 && joyval<3) y=y-speed;      
    if (joyval==8) y=y-speed;
    
  }else{
     acel=10;
  }
          
  return;
}


// ===========================================================================
//
// ===========================================================================
void mouseController()
{
  // control del puntero 
  MouseRead();
  
  x=x + Mouse_X;
  y=y + Mouse_Y;

}


// muestra el puntero
void showPointer()
{                      
  SetSpritePosition(0,x,y);
  SetSpritePosition(1,x,y);
}



/* ===========================================================================
showHelpText                                
Function : Shows a section of text from the line number given.
Input    : numlin (char) =  text position
Output   : -
=========================================================================== */
void showHelpPage(char numlin)
{
  showScrollBar(numlin, helpLines);
  showHelpText(numlin);
    
  return;
}



//void setHelpData()
void showHelpText(char numlin) __naked
{
numlin;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(ix)
  ld   IX,#DATA  ; suma resultado a puntero de buffer del texto
  or   A
  jr   NZ,search
  
  ld   B,A  
  jr   init  

search:
  ld   B,A  
loop1:
  push BC  
  ld   B,#29
    
loop2:  
  ld   A,(IX)
  cp   #0x0D  ; return char
  jr   NZ,nxt
  ld   B,#1
  
nxt:    
  inc  IX
  djnz loop2  
  
  pop bc
  djnz loop1  

init:    ; muestra el texto en el area de la pantalla
  ld   HL,#0x1881

  ld   B,#18
  
loop_ay:
  push BC
  
  ld   B,#29
  
  ld   A,#0
  
loop_ax:
  cp   #13
  call Z,VPOKE32
  jr   Z,next
  ld   A,(IX)
  cp   #13
  call Z,VPOKE32
  jr   Z,jump
  
  call 0x004D
  
jump:  
  inc  IX
  
next:    
  inc  HL
  djnz loop_ax
  
  ld   DE,#3  ; salta a la siguiente linea
  add  HL,DE
  
  pop  BC
  djnz loop_ay

  pop  IX
  ret

VPOKE32:
  push AF
  ld   A,#32
  call 0x004D
  pop  AF
  ret


DATA:
.db 0x50,0x53,0x47,0x65,0x64,0x20,0x56,0x30,0x2E,0x38,0x42,13
.db 0x43,0x4F,0x50,0x59,0x4C,0x45,0x46,0x54,0x20,0x32,0x30,0x31,0x30,0x20,0x61,0x6F,0x72,0x61,0x6E,0x74,0x65,0x2F,0x33,0x30,0x33,0x62,0x63,0x6E,13
.db 0x48,0x54,0x54,0x50,0x3A,0x2F,0x2F,0x50,0x53,0x47,0x45,0x44,0x2E,0x42,0x4C,0x4F,0x47,0x53,0x50,0x4F,0x54,0x2E,0x43,0x4F,0x4D,0x2F,13
.db 13
.db 0x50,0x53,0x47,0x65,0x64,0x20,0x49,0x53,0x20,0x41,0x20,0x50,0x52,0x4F,0x47,0x52,0x41,0x4D,0x20,0x46,0x4F,0x52,0x20,0x4D,0x53,0x58,13
.db 0x43,0x4F,0x4D,0x50,0x55,0x54,0x45,0x52,0x53,0x2C,0x20,0x57,0x49,0x54,0x48,0x20,0x54,0x48,0x45,13
.db 0x46,0x55,0x4E,0x43,0x54,0x49,0x4F,0x4E,0x41,0x4C,0x49,0x54,0x59,0x20,0x54,0x4F,0x20,0x50,0x4C,0x41,0x59,0x20,0x57,0x49,0x54,0x48,13
.db 0x54,0x48,0x45,0x20,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x53,0x20,0x46,0x52,0x4F,0x4D,0x20,0x50,0x53,0x47,13
.db 0x28,0x41,0x59,0x2D,0x33,0x2D,0x38,0x39,0x31,0x30,0x29,0x2E,0x20,0x49,0x54,0x20,0x49,0x53,0x20,0x44,0x45,0x53,0x49,0x47,0x4E,0x45,0x44,13
.db 0x57,0x49,0x54,0x48,0x20,0x41,0x20,0x47,0x52,0x41,0x50,0x48,0x49,0x43,0x41,0x4C,0x20,0x55,0x53,0x45,0x52,13
.db 0x49,0x4E,0x54,0x45,0x52,0x46,0x41,0x43,0x45,0x20,0x45,0x41,0x53,0x59,0x20,0x54,0x4F,0x20,0x55,0x53,0x45,0x2E,13
.db 13
.db 0x70,0x72,0x65,0x73,0x65,0x6E,0x74,0x61,0x74,0x69,0x6F,0x6E,0x20,0x61,0x6E,0x64,0x20,0x6D,0x65,0x6E,0x75,13
.db 0x43,0x55,0x52,0x53,0x4F,0x52,0x2F,0x4A,0x4F,0x59,0x53,0x54,0x49,0x43,0x4B,0x20,0x55,0x50,0x20,0x26,0x20,0x44,0x4F,0x57,0x4E,0x20,0x54,0x4F,13
.db 0x53,0x45,0x4C,0x45,0x43,0x54,0x20,0x54,0x48,0x45,0x20,0x44,0x45,0x56,0x49,0x43,0x45,0x20,0x59,0x4F,0x55,0x20,0x57,0x41,0x4E,0x54,13
.db 0x54,0x4F,0x20,0x55,0x53,0x45,0x20,0x41,0x4E,0x44,0x20,0x53,0x50,0x41,0x43,0x45,0x2F,0x4A,0x4F,0x59,0x53,0x54,0x49,0x43,0x4B,13
.db 0x42,0x55,0x54,0x54,0x4F,0x4E,0x20,0x47,0x4F,0x20,0x54,0x4F,0x20,0x54,0x48,0x45,0x20,0x45,0x44,0x49,0x54,0x4F,0x52,0x2E,13
.db 0x54,0x48,0x45,0x20,0x4D,0x4F,0x55,0x53,0x45,0x20,0x4F,0x50,0x54,0x49,0x4F,0x4E,0x2C,0x20,0x4A,0x55,0x53,0x54,13
.db 0x44,0x49,0x53,0x50,0x4C,0x41,0x59,0x20,0x49,0x46,0x20,0x49,0x54,0x27,0x53,0x20,0x44,0x45,0x54,0x45,0x43,0x54,0x45,0x44,0x2E,13
.db 0x41,0x43,0x43,0x45,0x50,0x54,0x53,0x20,0x41,0x4E,0x59,0x20,0x4F,0x46,0x20,0x54,0x48,0x45,0x20,0x54,0x57,0x4F,0x20,0x50,0x4F,0x52,0x54,0x53,13
.db 13
.db 0x65,0x64,0x69,0x74,0x6F,0x72,13
.db 0x73,0x65,0x71,0x75,0x65,0x6E,0x63,0x65,0x72,13
.db 0x54,0x48,0x45,0x20,0x53,0x45,0x51,0x55,0x45,0x4E,0x43,0x45,0x52,0x20,0x54,0x4F,0x20,0x43,0x52,0x45,0x41,0x54,0x45,13
.db 0x53,0x4F,0x4E,0x47,0x53,0x20,0x4F,0x46,0x20,0x55,0x50,0x20,0x54,0x4F,0x20,0x33,0x32,0x20,0x53,0x54,0x45,0x50,0x53,13
.db 0x55,0x53,0x49,0x4E,0x47,0x20,0x54,0x48,0x45,0x20,0x31,0x36,0x20,0x50,0x4F,0x53,0x53,0x49,0x42,0x4C,0x45,13
.db 0x50,0x41,0x54,0x54,0x45,0x52,0x4E,0x53,0x20,0x54,0x48,0x41,0x54,0x20,0x43,0x41,0x4E,0x20,0x45,0x44,0x49,0x54,0x2E,13
.db 13
.db 0x69,0x6E,0x66,0x6F,0x28,0xD8,0x29,0x2E,0x20,0x56,0x49,0x45,0x57,0x2F,0x45,0x44,0x49,0x54,0x20,0x54,0x48,0x45,0x20,0x53,0x4F,0x4E,0x47,13
.db 0x49,0x4E,0x46,0x4F,0x52,0x4D,0x41,0x54,0x49,0x4F,0x4E,0x2E,13
.db 0x6E,0x65,0x77,0x2E,0x20,0x53,0x54,0x41,0x52,0x54,0x53,0x20,0x41,0x4C,0x4C,0x20,0x50,0x53,0x47,0x45,0x44,0x20,0x44,0x41,0x54,0x41,0x2E,13
.db 0x53,0x48,0x4F,0x57,0x53,0x20,0x41,0x20,0x43,0x4F,0x4E,0x46,0x49,0x52,0x4D,0x41,0x54,0x49,0x4F,0x4E,0x20,0x57,0x49,0x4E,0x44,0x4F,0x57,13
.db 0x70,0x6F,0x73,0x2E,0x20,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x20,0x54,0x48,0x45,0x20,0x50,0x4F,0x49,0x4E,0x54,0x45,0x52,0x20,0x49,0x4E,13
.db 0x54,0x48,0x45,0x20,0x53,0x45,0x51,0x55,0x45,0x4E,0x43,0x45,0x2E,13
.db 0x69,0x6E,0x73,0x65,0x72,0x74,0x28,0xD4,0x29,0x2E,0x20,0x49,0x4E,0x53,0x45,0x52,0x54,0x20,0x41,0x20,0x53,0x54,0x45,0x50,0x20,0x49,0x4E,13
.db 0x54,0x48,0x45,0x20,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x20,0x57,0x48,0x45,0x52,0x45,0x20,0x59,0x4F,0x55,13
.db 0x41,0x52,0x45,0x2C,0x20,0x57,0x49,0x54,0x48,0x20,0x54,0x48,0x45,0x20,0x43,0x55,0x52,0x52,0x45,0x4E,0x54,13
.db 0x50,0x41,0x54,0x54,0x45,0x52,0x4E,0x20,0x4E,0x55,0x4D,0x42,0x45,0x52,0x2E,13
.db 0x64,0x65,0x6C,0x65,0x74,0x65,0x28,0xD5,0x29,0x2E,0x20,0x52,0x45,0x4D,0x4F,0x56,0x45,0x53,0x20,0x54,0x48,0x45,0x20,0x53,0x54,0x45,0x50,13
.db 0x46,0x52,0x4F,0x4D,0x20,0x54,0x48,0x45,0x20,0x43,0x55,0x52,0x52,0x45,0x4E,0x54,0x20,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x2E,13
.db 0x70,0x61,0x74,0x74,0x65,0x72,0x6E,0x2E,0x20,0x50,0x41,0x54,0x54,0x45,0x52,0x4E,0x20,0x4E,0x55,0x4D,0x42,0x45,0x52,13
.db 0x41,0x53,0x53,0x4F,0x43,0x49,0x41,0x54,0x45,0x44,0x20,0x57,0x49,0x54,0x48,0x20,0x54,0x48,0x45,0x20,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x2E
.db 0x6C,0x65,0x6E,0x67,0x74,0x68,0x2E,0x20,0x54,0x4F,0x54,0x41,0x4C,0x20,0x4E,0x55,0x4D,0x42,0x45,0x52,0x20,0x4F,0x46,0x20,0x53,0x54,0x45,0x50,0x53
.db 0x54,0x48,0x41,0x54,0x20,0x48,0x41,0x53,0x20,0x54,0x48,0x45,0x20,0x53,0x4F,0x4E,0x47,0x2E,13
.db 0x62,0x70,0x6D,0x2E,0x20,0x54,0x45,0x4D,0x50,0x4F,0x20,0x49,0x4E,0x20,0x42,0x45,0x41,0x54,0x53,0x20,0x50,0x45,0x52,13
.db 0x4D,0x49,0x4E,0x55,0x54,0x45,0x20,0x28,0x4F,0x4E,0x4C,0x59,0x20,0x50,0x41,0x4C,0x29,0x2E,13
.db 13
.db 0x70,0x6C,0x61,0x79,0x62,0x61,0x63,0x6B,0x20,0x63,0x6F,0x6E,0x74,0x72,0x6F,0x6C,0x20,0x62,0x75,0x74,0x74,0x6F,0x6E,0x73,13
.db 0x9C,0x70,0x6C,0x61,0x79,0x2F,0x70,0x61,0x75,0x73,0x65,0x2E,0x20,0x53,0x54,0x41,0x52,0x54,0x53,0x20,0x50,0x4C,0x41,0x59,0x49,0x4E,0x47,13
.db 0x20,0x54,0x48,0x45,0x20,0x53,0x4F,0x4E,0x47,0x20,0x4F,0x52,0x20,0x50,0x41,0x55,0x53,0x45,0x44,0x2E,13
.db 0x9C,0x6C,0x6F,0x6F,0x70,0x2E,0x20,0x44,0x49,0x53,0x41,0x42,0x4C,0x45,0x53,0x2F,0x45,0x4E,0x41,0x42,0x4C,0x45,0x53,13
.db 0x20,0x4C,0x4F,0x4F,0x50,0x20,0x50,0x4C,0x41,0x59,0x42,0x41,0x43,0x4B,0x20,0x4F,0x46,0x20,0x54,0x48,0x45,0x20,0x53,0x4F,0x4E,0x47,0x2E,13
.db 0x9C,0x73,0x74,0x6F,0x70,0x2E,0x20,0x53,0x54,0x4F,0x50,0x20,0x50,0x4C,0x41,0x59,0x42,0x41,0x43,0x4B,0x2E,0x20,0x49,0x46,13
.db 0x20,0x54,0x48,0x45,0x20,0x50,0x4C,0x41,0x59,0x45,0x52,0x20,0x49,0x53,0x20,0x53,0x54,0x4F,0x50,0x50,0x45,0x44,0x2C,0x20,0x50,0x55,0x54,13
.db 0x20,0x54,0x48,0x45,0x20,0x53,0x45,0x51,0x55,0x45,0x4E,0x43,0x45,0x20,0x54,0x4F,0x20,0x53,0x54,0x41,0x52,0x54,0x49,0x4E,0x47,13
.db 0x20,0x50,0x4F,0x53,0x49,0x54,0x49,0x4F,0x4E,0x2E,13
.db 13
.db 0x73,0x68,0x6F,0x72,0x74,0x63,0x75,0x74,0x73,0x20,0x6B,0x65,0x79,0x73,13
.db 0x5B,0x46,0x31,0x5D,0x20,0x48,0x45,0x4C,0x50,0x2E,13
.db 0x5B,0x46,0x32,0x5D,0x20,0x43,0x4F,0x4E,0x46,0x49,0x47,0x55,0x52,0x41,0x54,0x49,0x4F,0x4E,0x20,0x57,0x49,0x4E,0x44,0x4F,0x57,0x2E,13
.db 0x5B,0x46,0x33,0x5D,0x20,0x41,0x43,0x43,0x45,0x53,0x53,0x20,0x54,0x4F,0x20,0x54,0x48,0x45,0x20,0x4C,0x4F,0x41,0x44,0x49,0x4E,0x47,13
.db 0x20,0x20,0x20,0x20,0x20,0x53,0x43,0x52,0x45,0x45,0x4E,0x2E,13
.db 0x5B,0x46,0x35,0x5D,0x20,0x50,0x53,0x47,0x20,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x53,0x20,0x44,0x55,0x4D,0x50,0x2E,13
.db 0x5B,0x52,0x45,0x54,0x55,0x52,0x4E,0x5D,0x20,0x50,0x4C,0x41,0x59,0x2F,0x50,0x41,0x55,0x53,0x45,0x20,0x53,0x4F,0x4E,0x47,0x2E,13
.db 0x5B,0x53,0x45,0x4C,0x45,0x43,0x54,0x5D,0x20,0x50,0x4C,0x41,0x59,0x2F,0x50,0x41,0x55,0x53,0x45,0x20,0x50,0x41,0x54,0x54,0x45,0x52,0x4E,0x2E,13
.db 0x5B,0x49,0x4E,0x53,0x5D,0x20,0x50,0x4C,0x41,0x59,0x20,0x50,0x53,0x47,0x20,0x52,0x45,0x47,0x49,0x53,0x54,0x45,0x52,0x53,13
.db 0x20,0x20,0x20,0x20,0x20,0x20,0x53,0x49,0x4D,0x55,0x4C,0x54,0x41,0x4E,0x45,0x4F,0x55,0x53,0x4C,0x59,0x2E,13
.db 0x5B,0x53,0x54,0x4F,0x50,0x5D,0x20,0x53,0x54,0x4F,0x50,0x53,0x20,0x50,0x4C,0x41,0x59,0x42,0x41,0x43,0x4B,0x2E,13
.db 0x5B,0x31,0x5D,0x2C,0x5B,0x32,0x5D,0x20,0x26,0x20,0x5B,0x33,0x5D,0x20,0x43,0x48,0x41,0x4E,0x4E,0x45,0x4C,0x20,0x4D,0x55,0x54,0x45,0x2E,13
.db 13
.db 0x61,0x63,0x6B,0x6E,0x6F,0x77,0x6C,0x65,0x64,0x67,0x65,0x6D,0x65,0x6E,0x74,0x73,13
.db 0x4E,0x45,0x55,0x52,0x4F,0x46,0x4C,0x49,0x50,0x2C,0x20,0x4A,0x41,0x4D,0x51,0x55,0x45,0x2C,0x20,0x41,0x4E,0x44,0x52,0x45,0x41,0x52,0x2C,13
.db 0x4E,0x45,0x52,0x4C,0x41,0x53,0x4B,0x41,0x2C,0x20,0x50,0x45,0x4E,0x54,0x41,0x43,0x4F,0x55,0x52,0x2C,0x20,0x52,0x41,0x4D,0x4F,0x4E,0x45,0x53,13
.db 0x2C,0x20,0x41,0x56,0x45,0x4C,0x49,0x4E,0x4F,0x20,0x48,0x45,0x52,0x52,0x45,0x52,0x41,0x2C,0x20,0x42,0x41,0x5A,0x45,0x20,0x26,13
.db 0x46,0x52,0x49,0x45,0x4E,0x44,0x53,0x20,0x4F,0x46,0x20,0x4B,0x41,0x52,0x4F,0x53,0x48,0x49,0x20,0x43,0x4F,0x4D,0x4D,0x55,0x4E,0x49,0x54,0x59,13

                                                                                                                                                                                                                                                                           
__endasm;

}



void ShowInitScreen()
{
    COLOR(15,5,5);
    SetSpritesSize(0);
  
    unRLEWBtoVRAM((uint) LOGO_TSET, BASE12 + BANK0 + 256); //32 * 8
    unRLEWBtoVRAM((uint) LOGO_TSET, BASE12 + BANK1 + 256);
    unRLEWBtoVRAM((uint) LOGO_TSET, BASE12 + BANK2 + 256);
    
    unRLEWBtoVRAM((uint) LOGO_TSET_COL, BASE11 + BANK0 + 256);
    unRLEWBtoVRAM((uint) LOGO_TSET_COL, BASE11 + BANK1 + 256);
    unRLEWBtoVRAM((uint) LOGO_TSET_COL, BASE11 + BANK2 + 256);
    
    unRLEWBtoVRAM((uint) title_SCR, BASE10);
    
    unRLEWBtoVRAM((uint) logo_SPRITESET, BASE14);
    
    PUTSPRITE(0, 128,39, 4,0);
    PUTSPRITE(1, 120,50, 4,1);
    PUTSPRITE(2, 119,67, 13,2);
}



void ShowMainScreen()
{
    ClearSprites();
    
    COLOR(15,14,14);
    SetSpritesSize(1);   

    unRLEWBtoVRAM((uint) main_TSET, BASE12 + BANK0);
    unRLEWBtoVRAM((uint) main_TSET, BASE12 + BANK1);
    unRLEWBtoVRAM((uint) main_TSET, BASE12 + BANK2);
    
    unRLEWBtoVRAM((uint) main_TSET_COLOR, BASE11 + BANK0);
    unRLEWBtoVRAM((uint) main_TSET_COLOR, BASE11 + BANK1);
    unRLEWBtoVRAM((uint) main_TSET_COLOR, BASE11 + BANK2);
    
    unRLEWBtoVRAM((uint) main_SPRITESET, BASE14);
    
    unRLEWBtoVRAM((uint) main_SCR, BASE10); 
    
}



void ShowHelpScreen()
{
  unRLEWBtoVRAM((uint) help_SCR, BASE10+64);
  
  PUTSPRITE(2, 242,  34, 5,2);
  PUTSPRITE(3, 242, 170, 5,3);
}



void ShowLoadScreen()
{
  unRLEWBtoVRAM((uint) load_SCR, BASE10+64);
  
  PUTSPRITE(2, 122,  33, 5,2);
  PUTSPRITE(3, 122, 137, 5,3);
}




void ShowInfoWin()
{
  unRLEWBtoVRAM((uint) info_WIN, 0x1960); //BASE + (11*32)+0
}





/* =============================================================================

============================================================================= */
/*void ShowConfirmWin()
{
  byte i,o;
  uint pos=0;
  uint VRAMaddr=0x1906;
 
  for(i=8;i<15;i++)
  {
    for(o=6;o<26;o++) VPOKE(VRAMaddr++, confirm_WIN[pos++]);
    VRAMaddr += 12;
  }
  
  PUTSPRITE(5, 56, 71, 1,5);

}*/






/* =============================================================================

============================================================================= */
void ShowWindow(char posX, char posY, char width, char height, uint RAMaddr)
{
  char i;
  uint VRAMaddr=GetVRAMaddressByPosition(posX,posY);
    
  for(i=0;i<height;i++)
  {
    CopyToVRAM(RAMaddr,VRAMaddr,width);
    RAMaddr += width;
    VRAMaddr += 32;
  }
}






/* =============================================================================

============================================================================= */
/* void ShowConfigWin()
{
//ShowWindow(4,4,24,17,(uint) config_WIN);
  char i;
  uint VRAMaddr=0x1864; //4,4
  uint RAMaddr=(uint) config_WIN;
    
  for(i=0;i<17;i++)
  {
    CopyToVRAM(RAMaddr,VRAMaddr,24);
    RAMaddr += 24;
    VRAMaddr += 32;
  }
}*/



/* =============================================================================

============================================================================= */
/*void ShowPSGDumpWin()
{
  byte i,o;
  uint pos=0;
  uint VRAMaddr=0x1863; //3,4

  for(i=3;i<20;i++)
  {
    for(o=3;o<29;o++) VPOKE(VRAMaddr++, PSGregs_WIN[pos++]);
    VRAMaddr += 6;
  }
}*/



/* =============================================================================

============================================================================= */
void ShowCopyWin()
{
  byte i,o;
  uint pos=0;
  uint VRAMaddr=0x1A52;  // x=18 y=18
  
  for(i=18;i<21;i++)
  {
    for(o=18;o<32;o++) VPOKE(VRAMaddr++, copy_WIN[pos++]);
    VRAMaddr += 18;
  }
}
 
 
 
/* -----------------------------------------------------------------------------
 set V9938 Palettes
----------------------------------------------------------------------------- */
void SetPalette(char number) __naked
{
number;
__asm
  push IX
  ld   IX,#0
  add  IX,SP
  
  ld   A,4(ix)  ;<-- numero de paleta

;recoge la posicion de los datos del mapa usando un indice 
  ld   IX,#palIndex
  
;get ADDR of page data
  or   A
  jr   Z,readPALaddr    ;si es 0 no calcula nada, recoge el primer valor
  sla  A
  ld   E,A
  ld   D,#0
  add  IX,DE

readPALaddr:
  ld   H,1(IX)
  ld   L,(IX)
    
  xor	 A
  di
  out	 (#0x99),A
  ld	 A,#144
  out	 (#0x99),A
  ld	 BC,#0x209A
  otir
  ei
  
  pop  IX  
  ret 
  
palIndex:
.dw _logo_PAL,_new_PAL,_TMS_PAL

__endasm;
}                                                                                                         
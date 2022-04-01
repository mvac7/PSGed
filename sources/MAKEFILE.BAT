@echo off
REM MAKEFILE for 32K ROMs using SDCC compiler
REM CFILENAME = Source name without extension
REM ROMFILENAME = ROM file name without extension
SET CFILENAME=PSGed
SET ROMFILENAME=PSGED83B
SET CODELOC=0x4050
SET CRT=libs\crt0_MSX32kROM4000.rel
SET LIB0=libs\memory.rel 
SET LIB1=libs\string.rel  
SET LIB2=libs\keyboard_MSXROM.rel
SET LIB3=libs\joystick_MSXROM.rel
SET LIB4=libs\mouse.rel
SET LIB5=libs\AY38910.rel
SET LIB6=libs\VDP_TMS9918A_MSXROM.rel
SET LIB7=libs\VDP_SPRITES_MSXROM.rel
SET LIB8=libs\unRLEWBtoVRAM.rel  
SET LIB9=libs\unRLEWBtoRAM.rel
SET LIB10=
SET LIB11=
SET LIB12=
if exist build\ goto NEXTSTEP1
echo MAKEDIR build
md build
:NEXTSTEP1
if exist bin\ goto NEXTSTEP2
echo MAKEDIR bin
md bin
:NEXTSTEP2
REM get SDCC version
sdcc -v
if not exist build\%CFILENAME%.ihx goto COMPILA
del build\%CFILENAME%.ihx
:COMPILA
echo Compiling...
sdcc -mz80 -o build\ --code-loc %CODELOC% --data-loc 0xC000 --use-stdout --no-std-crt0 %CRT% %LIB0% %LIB1% %LIB2% %LIB3% %LIB4% %LIB5% %LIB6% %LIB7% %LIB8% %LIB9% %LIB10% %LIB11% %LIB12% src\%CFILENAME%.c
if not exist build\%CFILENAME%.ihx goto SALIDA
hex2bin -e bin -s 4000 -l 8000 build\%CFILENAME%.ihx
copy build\%CFILENAME%.bin bin\%ROMFILENAME%.ROM
pause
exit /b
:SALIDA
echo ERROR
pause
exit /b

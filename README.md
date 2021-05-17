# PSGed

```
Author: aorante (aka mvac7)
Architecture: MSX
Format: ROM 32K
Programming language: C and Z80 assembler
```


Includes a *minimalTech* demo song by [neuroflip](https://twitter.com/neuroflip)


## Sorry!: This text is pending correction of the English translation.


## Description:

PSGed is an experimental application for MSX computers to make music working directly with the PSG registers (AY-3-8910).

It is designed with a graphical user interface easy to use.



## Author's Notes

This application was my first experience learning C development and MSX Assembler.

The purpose was to develop an application to create music under a Groovebox type interface (such as the electrives or the 303), 
but when working directly editing the AY parameters instead of using instruments, it did not provide the desired functionality.

For that reason I left it as an AY sound editor that includes a simple sequencer and did not add disk access functionality.

But it gave me ideas for a new, more complete version with improvements in the sequencer, 
where it allowed the entry of notes and changes in the sound parameters at the pattern level.

PSGed v2 reached an advanced stage of development but was more complex to use than the previous version, so I left the project.
I made the mistake of continuing to not include instrument-based operation.

![PSGed v2.0b](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/PSGed_v20b_GUI.png)

After some time of this failure, I started version 0.9 of the initial PSGed with improvements based on v2: 
new graphic interface design and patterns with musical note input.

![PSGed v0.90b](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/PSGed_v090b_main_v2.png)

Continuing with the idea of creating a Groovebox type application, 
I started looking at the existing MSX players such as PT3 and WYZ and in the end I decided to try the WYZ one.
The WYZ player provides three channels to compose the melody plus a channel to launch effects that was mixed with one of the three channels, 
allowing to provide the feeling of having 4 channels. 
The FX channel allowed it to be used for percussion or gaming effects.
It also allows the use of the AY-3-8910 effect with the low frequency envelope, 
to create a sound with a sawtooth-like waveform, providing a great bass sound ideal for developing a Groovebox.
 
I contacted the programmer [(WYZ)](https://sites.google.com/site/wyzplayer/) and developed a demo of an application with a button-based interface designed to launch patterns. 
I named it grooveSX. [(New in MSX Resource Center)](https://www.msx.org/news/en/groovesx-live-v004b)

![GrooveSXlive v0.03b](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/GrooveSXlive_v003b_play.png)

The application only included the music reproduction part. 
It consists of a matrix of buttons designed to launch the patterns of a module, and allow playing with the live playback designed for chiptune DJing. 

The problem that I found is that the player did not serve my purposes. 
It was designed to reproduce songs efficiently in games or demos and did not allow to work directly from an editor so it required a version with important changes.

To develop the Groovebox application that I had devised, I needed to program a specific player and at that moment I did not find the level of assembly programming for it, so I went to other projects.

Along the way, I have developed several music shows, [TARP](https://github.com/mvac7/TARP) (The Alan Randoms Project) being the first one that I have published fully finished. 
I also developed a drum machine as part of a new vision of GrooveSX, this time in MSX-DOS format and with disk access.

![Drumm](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/drumm04.2.png)

I decided to open this project on GitHub, thanks to [Retro DeLuxe](https://github.com/retrodeluxe), since he showed interest in this project.

In the short term I want to publish version 0.9 of PSGed and later an application resulting from the evolution of TARP but with editing (called TDB), 
and in the future I do not rule out being able to do the GrooveSX application.

*mvac7*



## Requirements: 

### To compile:

* SDCC v3.9 http://sdcc.sourceforge.net/ 
* Hex2bin v2.5 http://hex2bin.sourceforge.net/



### To run:

A standard MSX computer with 16K RAM.

* You can record it to a FlashROM 
* From disk using tools like ODO, loadrom, execrom, etc. 
* From cassette using WAVeR or a similar tool.
 

## Acknowledgements
 
I want to thank the following persons, who directly or indirectly helped me to achieve the development of this project.

* [Neuroflip](https://twitter.com/neuroflip)/303bcn: beta testing & ideas.
* [Andrear](http://andrear.altervista.org/home/index.php): beta testing & ideas.
* [Nerlaska](http://albertodehoyonebot.blogspot.com.es): Code support.
* [Marq](http://www.kameli.net/marq/)/[Lieves!Tuore](http://www.kameli.net/lt/): Code support.
* [Avelino Herrera](http://msx.atlantes.org/index_es.html): Code support.
* [Pentacour](http://pentacour.com/blog/): nMSXtiles tool.
* Ramones: mouse code. [(MSXblog)](https://www.msxblog.es/tutoriales-de-programacion-en-ensamblador-ramones/) - [(MSXbanzai)](http://msxbanzai.tni.nl/dev/faq.html)
* [Baze](http://baze.au.com/misc/z80bits.html): ASM Z80 routines
* [Fubukimaru](https://github.com/Fubukimaru) > [(Blog)](http://www.gamerachan.org/fubu/) 
* Friends of [Karoshi MSX Community](http://karoshi.auic.es/) 


## Task List


### PSGed v0.4b
- [x] change tempo to bpm (http://en.wikipedia.org/wiki/Tempo)
- [x] increasing acceleration of change value in large numbers.
- [x] load demos
- [x] redesign the presentation of the sequencer.
- [x] Add mouse control
- [x] Help functionality (scroll)
- [x] Pressing _Stop_, stop when you just play the end of the pattern.
<br/>


### PSGed v0.5b

- [x] On the menu when the mouse is detected, select your choice.
- [x] Redesigned main screen.
- [x] _Return_ Key to play song.
- [x] _Select_ key to play pattern.
- [x] Bug. Stop the sound, to show Help screen.
- [x] Bug. PLAY at the start of the interrupt / loop.
- [x] 16 patterns memory.
- [x] 32-step sequencer. 
- [x] Added Name and author information.
- [x] Info window.
- [x] Clear tool (CLR), delete the current pattern.
- [x] New tool (NEW), delete all the song information (resets).
- [x] Confirmation modal window (for New and CLR).
- [x] Copy tool (cpy), copy the current pattern to pattern indicated.
- [x] button (Set). Assigns the current pattern, at the position of the sequencer.
- [x] After Stop. If the player is in pause mode, the stop key terminates the playback of the song. Shown in standby mode.
- [x] Button enables/disables the loop mode of play.
- [x] Added three tones in the pattern editor (offset).
<br/>

### PSGed v0.7b

- [x] Mute channels by mouse click.
- [x] Added second button (mouse/joystick) or _Graph_ key to remove or add a silent item.
- [x] Fixed bug in the interpretation of the volume / envelope, in the data structure.
- [x] Fixed bug in the presentation of the playback position of the pattern.
- [x] Fixed bug in direct change so play / song to play / pattern.
- [x] Fixed problem with Sync (Using the asm statement: HALT).
- [x] Changing the color palette to detect the VDP 9938 or higher (MSX2).
- [x] Using PSG MegaFlashROM SCC+
<br/>

### PSGed v0.8b ROM edition

- [x] Added ComboBox control to select the envelope.
- [x] Asm routines added. Andrear adaptation of mouse routines by Ramones.
- [x] Editable fields in the Info window.
- [x] Separation of the main player code to a function.
- [x] Button to play all PSG registers [ins].
- [x] Added window that displays the registers of the PSG.
- [x] Separation of the stop code to a function.
- [x] Update help text (summary).
- [x] Help. Asm routine to return control line.
- [x] Added on-screen presentation, "AY-3-8910 sound editor"
- [x] Double-stop function. Place the sequencer to the first position.
- [x] Copy to function. Shows the initial value, the current pattern.
- [x] Sequencer. Added function insert step.
- [x] Sequencer. Added function delete step.
- [x] Edit screen. Added new features and changes for adjustment of noise and envelope areas.
- [x] Bug. Update data at the end of song play without loop.
- [x] Bug. After play in stop mode, when there will be a combobox, the music plays...
- [x] Compress Data: demos, help.
- [x] New version logo.
- [x] Added demo minimalTech / neuroFlip. 
- [x] Remove Demo3 to gain memory space.
- [x] Changing the load screen. Added name and author of the demos.
<br/>

### Future releases 

- [ ] Convert application to MSXDOS. (.COM)
- [ ] Add exit button.
- [ ] Add confirmation modal window, exit the application.
- [ ] Calculation of the BPMs for NTSC.
- [ ] Sequencer. Return Function Pattern (neuroflip). The number selected will be the pattern where the player becomes, when you finish the playback of the sequence and loop mode is enabled. 
- [ ] Sequencer. Add 64 steps.
- [ ] Sequencer. Change current system notes.
- [ ] Save info registers in each pattern.
- [ ] Editor. Changing tone frequency value for offset. (-128>+128)
- [ ] Change in real time registers of tone frequency... 
- [ ] control parameters from 6 pots + 2 buttons (1 joystick input)
- [ ] Disk file access.
- [ ] get dir
- [ ] load file
- [ ] save file
- [ ] Help. By clicking on the bar jump pages.
- [ ] Delete record dump. (V0.8b ROM functionality only).








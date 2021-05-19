# PSGed

```
Author: aorante (aka mvac7)
Architecture: MSX
Format: ROM 32K
Programming language: C and Z80 assembler
```


Includes a *minimalTech* demo song by [neuroflip](https://twitter.com/neuroflip)

---

<br/>

## Description:

PSGed is an experimental application for MSX computers to make music working directly with the PSG registers (AY-3-8910).

It is designed with a graphical user interface easy to use.

<br/>
<br/>

## The little history of PSGed

> ### Talking about PSGed is talking about procrastination, electronic music, energy drinks and chocolate cream sandwiches.

<br/>

### Chapter 1. How do I do this?

I will start by confessing that PSGed was my first attempt to develop an application for MSX computers using C and Z80 assembler.

At this time, I knew the MSX hardware because when I was a teenager I had programmed in BASIC and I had even done some assembly routines, 
but everything seemed very hard to me.

...until at some MSX user meeting, where I met Mr. Alberto de Hoyo (Nerlaska). He told me that it could be programmed in C using the SDCC cross-development compiler.

I found in Alberto's WEB a tutorial to create a ROM and doing a lot of research on the internet (there wasn't much about development with SDCC for MSX), 
and I was making my way through the fog of ignorance to the person I am today: *A Smoke seller!*

To share my experience with the world, I opened an exclusive Blog for PSGed, where I posted everything I learned.
Mysteriously, one day a UFO abducted the blog and disappeared. If anyone knows anything, report it to agents Mulder and Scully. *The truth is out there!*

The idea of PSGed was to develop an application to create music under a Groovebox-type interface (such as electrives or 303),
but when working directly editing the AY parameters instead of using instruments, it did not provide the desired functionality.
For that reason I left it as an AY sound editor that includes a simple sequencer and did not add disk access functionality.

![PSGed v0.1](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/PSGed_15.png)
<br/>*PSGed v0.1b*

![PSGed v0.1](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/PSGed_v082b_main.png)
<br/>*PSGed v0.82b*

But it gave me ideas for a new, more complete version with improvements in the sequencer, 
where it allowed the entry of notes and changes in the sound parameters at the pattern level.

<br/>

### Chapter 2. What if it can be done better?

PSGed v2 reached an advanced stage of development but was more complex to use than the previous version, so I left the project.
I made the mistake of continuing to not include instrument-based operation.

![PSGed v2.0b](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/PSGed_v20b_GUI.png)
<br/>*PSGed v2.0b*

After some time of this failure, I started version 0.9 of the initial PSGed with improvements based on v2: 
new graphic interface design and patterns with musical note input.

![PSGed v0.90b](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/PSGed_v090b_main_v2.png)
<br/>*PSGed v0.90b*

<br/>

### Chapter 3. The crazy ones of the chiptune.

But now I wantes to pause. However, during this time, because of my friend Neuroflip I got into the chiptune scene in Barcelona. 
A marginal musical subculture that moved through small spaces in the marginal neighborhoods of Barcelona.
We formed a group called 303bcn and we started with Neuroflip's C64 where he was DJing his music, while I was doing VJing from an MSX connected to a video projector. 
To do this, I developed an application for VJing where using the keyboard you could visually interpret the music, showing different graphic effects.

![veejingSX](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/veejingSX_v09b.png)
<br/>[*veejingSX*](http://www.lowtoy.com/veejingsx/)

Later we went on to use Commodore Amiga computers, specifically three Amigas 1200: (2 for DJing and 1 for VJing). 
We needed an extra-long table to fit so many A1200s. But this is another story.

> Those times were very crazy. Sex (on the internet), Cola-drinks and above all a lot of Noise!!!! **YEAH!**

This experience made me think of an application oriented to live playback and this led to the next chapter:

<br/>

### Chapter 4. Drum and Bass Machinator!

Continuing with the idea of creating a Groovebox type application, 
I started looking at the existing MSX players such as PT3 and WYZ and in the end I decided to try the WYZ one.

The WYZ player provides three channels to compose the melody plus a channel to launch effects (this is mixed with one of the three channels), 
allowing to provide the feeling of having 4 channels. 
The FX channel allowed it to be used for percussion or gaming sound effects.
It also allows the use of the AY-3-8910 effect with the low frequency envelope, 
to create a sound with a sawtooth-like waveform, providing a great bass sound ideal for developing a Groovebox.
 
I contacted the programmer [(WYZ)](https://sites.google.com/site/wyzplayer/) and I developed a demo that includes a matrix of buttons to launch patterns and to be able to perform or improvise the live playback (DJing). 
I named it grooveSX. [(New in MSX Resource Center)](https://www.msx.org/news/en/groovesx-live-v004b)

![GrooveSXlive v0.03b](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/GrooveSXlive_v003b_play.png)
<br/>*GrooveSXlive v0.03b*

The problem that I found is that the WYZplayer did not serve my purposes. 
It was designed to reproduce songs efficiently in games or demos and did not allow to work directly from an editor so it required a version with important changes.

To develop the Groovebox application that I had devised, I needed to program a specific player and at that moment I didn't had the level of assembly programming for it, so I went to other projects.

Along the way, I have developed several music shows, [TARP](https://github.com/mvac7/TARP) (The Alan Randoms Project) being the first one that I have published fully finished. 
I also developed a drum machine as part of a new vision of GrooveSX, this time in MSX-DOS format and with disk access.

![TARP](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/TARPv1_main.png)
<br/>*The Alan Randoms Project*

![Drumm](https://raw.githubusercontent.com/mvac7/PSGed/main/screenshots/drumm04.2.png)
<br/>*GrooveSX Drumm*

<br/>

### Chapter 5. Back to the future.

In all this time I have also programmed some open source [libraries](https://github.com/mvac7/SDCC_MSX_fR3eL), [graphical tools](https://github.com/mvac7/mSXdevtools) (via Infinite Development System) and [Random Monkey Generators](https://github.com/mvac7/MSX_monKEY_Lib). 
Nothing useful. 
For this reason, I now roam in the underworld of losers developers. 
If you need me, *Who you gonna call?* **Ghostbusters!**

I decided to open this project on GitHub, thanks to [Retro DeLuxe](https://github.com/retrodeluxe), since he showed interest in this project.

It is possible that PSGed version 0.9 will be published soon and later an application called TDB based on the evolution of TARP but with editing. 
I do not rule out in the future being able to make the GrooveSX application, although it may already be published in a parallel universe.

**Note:** The images included are just a small sample of all the designs I have created. 
Shame haunts me for not finishing my projects. 
I deserve to be publicly punished with tarring and feathering for my offense to the MSX user community. 

*mvac7 (aka aorante)*

<br/>

## Requirements: 

### To compile:

* SDCC v3.9 http://sdcc.sourceforge.net/ 
* Hex2bin v2.5 http://hex2bin.sourceforge.net/



### To run:

A standard MSX computer with 16K RAM.

* You can record it to a FlashROM 
* From disk using tools like ODO, loadrom, execrom, etc. 
* From cassette using WAVeR or a similar tool.
 
<br/>

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


<br/>
---


## Task List


### PSGed v0.4b ROM (16/11/2010)
- [x] change tempo to bpm (http://en.wikipedia.org/wiki/Tempo)
- [x] increasing acceleration of change value in large numbers.
- [x] load demos
- [x] redesign the presentation of the sequencer.
- [x] Add mouse control
- [x] Help functionality (scroll)
- [x] Pressing _Stop_, stop when you just play the end of the pattern.
<br/>


### PSGed v0.5b ROM

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

### PSGed v0.7b ROM

- [x] Mute channels by mouse click.
- [x] Added second button (mouse/joystick) or _Graph_ key to remove or add a silent item.
- [x] Fixed bug in the interpretation of the volume / envelope, in the data structure.
- [x] Fixed bug in the presentation of the playback position of the pattern.
- [x] Fixed bug in direct change so play / song to play / pattern.
- [x] Fixed problem with Sync (Using the asm statement: HALT).
- [x] Changing the color palette to detect the VDP 9938 or higher (MSX2).
- [x] Using PSG MegaFlashROM SCC+
<br/>

### PSGed v0.8b ROM (20/11/2010)

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
- [x] New version logo.
- [x] Added demo minimalTech / neuroFlip. 
- [x] Remove Demo3 to gain memory space.
- [x] Changing the load screen. Added name and author of the demos.
<br/>


### PSGed v0.82b ROM (26/01/2012)
- [x] Real-time modification of AY records in playback mode.
<br/>


### PSGed v0.83b ROM GitHub Release

- [x] Replacement of old functions and libraries with the latest [fR3el libraries](https://github.com/mvac7/SDCC_MSX_fR3eL).
- [x] Reverse engineering to recover the graphics [(PSGed/GFX/)](https://github.com/mvac7/PSGed/tree/main/GFX).
- [x] RLEWB compression of graphic and demo datas.
- [x] New Load screen.
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
- [ ] Delete record dump.


---

## More and better

If you want to make music for MSX computers from an MSX, you have the great TriloTracker but if you are looking for the comfort of your daily computer, 
you have a great variety of good cross-trackers for different operating systems:

* [TriloTracker](http://www.trilobyte-msx.com/TriloTracker/index.php/Main_Page) [(GitGub)](https://github.com/cornelisser/TriloTracker) [MSX] 
* [WYZTracker](https://github.com/AugustoRuiz/WYZTracker) by Augusto Ruiz [Windows]
* [Vortex Tracker II](https://bulba.untergrund.net/vortex_e.htm) [Windows]
* [Arkos Tracker 2](https://www.julien-nevo.com/arkostracker/) [Windows/Linux/MacOSX]

And if you want to use your MSX as a musical instrument, you can try the [MSX Synth](http://paxangasoft.retroinvaders.com/othersoft/msxsynth.html) application from Paxanga soft.


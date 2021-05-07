#ifndef  __MOUSE_H__
#define  __MOUSE_H__

#define INITMEM 0xF87F		// 0xF87F fkeys 160 bytes 0xE000
#define MOUSE INITMEM
#define MOUSEX MOUSE + 1
#define MOUSEY MOUSEX + 1
#define MOUSEBUTTON1 MOUSEY + 1
#define MOUSEBUTTON2 MOUSEBUTTON1 + 1
#define MSXMODEL MOUSEBUTTON2 + 1

extern void checkmouse(void);
extern void mouseread(void);

#endif
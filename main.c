#if 1					// header files

#include "ngpc.h"		// required
#include "carthdr.h"	// required
#include "library.h"	// NGPC routines
#include "core/rom/flash.h"
// #include <stdlib.h>		// std C routines
// #include <stdio.h>

#endif

#define P1_PAL1 0
#define MEM_POS 4

extern u32 gamedata[256];

void main()
{

	InitNGPC();
	SetBackgroundColour(RGB(68,68,68));
	SysSetSystemFont();

	InitFlash();
	SetPalette(SCR_1_PLANE, P1_PAL1, 4, RGB(15,15,15), RGB(15,15,15), RGB(15,15,15));
	PrintString(SCR_1_PLANE,P1_PAL1,7,7, "Hello!");
	PrintString(SCR_1_PLANE,P1_PAL1,3,9, "NGPC Developer");

	/* #region No Escape! */
	while(1) { 
		
		WaitVsync(); 
	}
	/* endregion */
}
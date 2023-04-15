/*								*
 *		source code optimized for SciTE			*
 * ------------------------------------------------------------ */

#if 1		// include files
#include "ngpc.h"
#include "library.h"

#include "library.inc"
#endif

#if 1		// simple variables

volatile u8 VBCounter;

// SFX related variables and registers
u8 SFXInstalled;
#define SFXPulse       (*(u8 *)0x7010)
#define SFXPlayBuffer  (*(u8 *)0x7011)
#define SFXLoadPreset  (*(volatile u8 *)0x7012)
#define SFXPlayPreset  (*(u8 *)0x7013)
#define SFXStopAll     (*(u8 *)0x7014)
#define SFXBuffer      (u8 *)0x7020

#define RAND_MAX 32767
volatile u32 RAND_RandomData;
u8 RandomNumberCounter;

#define NEOTRACKER_ACK  (*(u8*)0x70DE)

#define	MAGIC_NB	0xcafebabe	// java rules !

#define BitBase_2 71			// for 7 color bitmaps

#endif

#if 1		// data structures

const char DayNames[7][10] = { 
   "Sunday", "Monday", "Tuesday", "Wednesday", 
   "Thursday", "Friday", "Saturday"
};

// Bitmap masks
const unsigned short BitmapMask[8] = {
0x3fff, 0xcfff, 0xf3ff, 0xfcff, 0xff3f, 0xffcf, 0xfff3, 0xfffc};

const unsigned short BitmapNewMask[4][8] = {
   {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000},
   {0x4000, 0x1000, 0x0400, 0x0100, 0x0040, 0x0010, 0x0004, 0x0001},
   {0x8000, 0x2000, 0x0800, 0x0200, 0x0080, 0x0020, 0x0008, 0x0002},
   {0xc000, 0x3000, 0x0c00, 0x0300, 0x00c0, 0x0030, 0x000c, 0x0003}
};

const unsigned short OffsetValues[8]= {
0x1, 0x4, 0x10, 0x40, 0x100, 0x400, 0x1000, 0x4000};

#endif

void __interrupt DummyFunction(void) {
// dummy Interrupt function used for interrupt that are currently
// unused. To add an interrupt simply supply your own routine in the
// InitNGPC routine in the interrupt vector table initialisation

}


void SysShutdown(); // forward reference
void __interrupt VBInterrupt(void) {
//////////////////////////////////////////////////////////////////////////////
// VBInterrupt
// Handles the default work required by the VBInterrupt (watchdog,
// shutdown and a standard counter)
//////////////////////////////////////////////////////////////////////////////
   // clear the watchdog
   WATCHDOG = WATCHDOG_CLEAR;

   // check for power down
   if (USR_SHUTDOWN)
   {
	  SysShutdown();
	  while (1);
   }
   
   // increment a counter
   VBCounter++;

   if (SFXInstalled)
	  SFXPulse = 1;
   
   // TODO: add any other VBI code here.
}

void InitNGPC(void) {
//////////////////////////////////////////////////////////////////////////////
// InitNGPC
// This should be the first function called in main() it initialises the
// NGPC hardware (interrupt vector tables etc)
//////////////////////////////////////////////////////////////////////////////
   // Set NGP or NGPC mode
   if (OS_VERSION == 0)
   {
	  (*(u8*)0x6f83) &= ~8; // res 3
	  (*(u8*)0x6da0) = 0;
   }

   // set user answer
   USR_ANSWER |= 64; // set 6

   // User hasn't called InstallSoundDriver yet
   SFXInstalled = 0;
   
   // install user interrupt vectors
   SWI3_INT = DummyFunction;
   SWI4_INT = DummyFunction;
   SWI5_INT = DummyFunction;
   SWI6_INT = DummyFunction;
   RTCI_INT = DummyFunction;
   VBL_INT = VBInterrupt;
   Z80_INT = DummyFunction;
   HBL_INT = DummyFunction;
   TI0_INT = DummyFunction;
   TI1_INT = DummyFunction;
   TI2_INT = DummyFunction;
   TI3_INT = DummyFunction;
   STX_INT = DummyFunction;
   SRX_INT = DummyFunction;
   DMA0_INT = DummyFunction;
   DMA1_INT = DummyFunction;
   DMA2_INT = DummyFunction;
   DMA3_INT = DummyFunction;

   ENABLE_INTERRUPTS;
   
   // set screen size
   WIN_X = 0;
   WIN_Y = 0;
   WIN_W = 160;
   WIN_H = 152;

}


void SysShutdown(void) {
   __asm(" ld rw3,0"); // 0 = VECT_SHUTDOWN

   // do the system call
   __asm(" ldf 3");
   __asm(" add w,w");
   __asm(" add w,w");
   __asm(" ld xix,0xfffe00");
   __asm(" ld xix,(xix+w)");
   __asm(" call xix");
}


void ResumeOff(void) {
// turns off system after setting resume bit
// this skips the intro and can be detected to
// resume the game where it left off if programmed
// properly

	USR_ANSWER |= 128;
	SysShutdown();
};

void SysSetSystemFont(void) {
   __asm(" ld ra3,3"); 
   __asm(" ld rw3,5"); // VECT_SYSFONTSET

   // do the system call
   __asm(" ldf 3");
   __asm(" add w,w");
   __asm(" add w,w");
   __asm(" ld xix, 0xfffe00");
   __asm(" ld xix,(xix+w)");
   __asm(" call xix");
}

void ClearScreen(u8 Plane) {
//////////////////////////////////////////////////////////////////////////////
// ClearScreen
// Inputs:
//		Plane - Scroll Plane to clear SCR_1_PLANE or SCR_2_PLANE
//////////////////////////////////////////////////////////////////////////////
   int i;
   u16 * Screen;

   switch(Plane)
   {
	  case SCR_1_PLANE:
		 Screen = SCROLL_PLANE_1;
		 break;
	  case SCR_2_PLANE:
		 Screen = SCROLL_PLANE_2;
		 break;
	  default:
		 return;
   }

   for (i = 0; i < 32*32; i ++)
	  Screen[i] = 0;
}

void SetColors(u8 Plane, u8 PaletteNo, u16 Col1, u16 Col2, u16 Col3) {
// like SetPalette() but ignores unusable entry 0

   u16 * thePalette;
   u8 Offset = PaletteNo * 4;

   switch (Plane)   {
	  case SCR_1_PLANE:
		 thePalette = SCROLL_1_PALETTE;
		 break;
	  case SCR_2_PLANE:
		 thePalette = SCROLL_2_PALETTE;
		 break;
	  case SPRITE_PLANE:
		thePalette = SPRITE_PALETTE;
		break;
	  default:
		 return;
   }

   thePalette[Offset+1] = Col1;
   thePalette[Offset+2] = Col2;
   thePalette[Offset+3] = Col3;
}

void SetPalette(u8 Plane, u8 PaletteNo, u16 Col0, u16 Col1, u16 Col2, u16 Col3) {
//////////////////////////////////////////////////////////////////////////////
// SetPalette
// Inputs:
//		Plane - Scroll Plane to clear SCR_1_PLANE or SCR_2_PLANE
//		PalleteNo - 0-15 the palette no to set
//      Col0-Col3 - The RGB values for the 4 colours within the palette
//////////////////////////////////////////////////////////////////////////////
   SetColors(Plane, PaletteNo, Col1, Col2, Col3);
}

void PutTile(u8 Plane, u8 Palette, u8 XPos, u8 YPos, u16 TileNo) {
//////////////////////////////////////////////////////////////////////////////
// PutTile
// Inputs:
//		Plane - Scroll Plane to clear SCR_1_PLANE or SCR_2_PLANE
//		PalleteNo - 0-15 the palette number to set
//      XPos - X Position (0 to 19)
//      YPos - Y Position (0 to 18)
//      TileNo - Tile Number (0 to 511)
//////////////////////////////////////////////////////////////////////////////
   u16 * ScreenPlane1 = SCROLL_PLANE_1;
   u16 * ScreenPlane2 = SCROLL_PLANE_2;

   u16 Offset = ((u16)YPos * 32) + XPos;
   u16 Value = TileNo + ((u16)Palette << 9);

   switch(Plane)
   {
	  case SCR_1_PLANE:
		 ScreenPlane1[Offset] = Value;
		 break;
	  case SCR_2_PLANE:
		 ScreenPlane2[Offset] = Value;
		 break;
	  default:
		 break;
   }
}


void GetTile(u8 Plane, u8 *Palette, u8 XPos, u8 YPos, u16 *TileNo) {
//////////////////////////////////////////////////////////////////////////////
// GetTile
// Inputs:
//		Plane - Scroll Plane to clear SCR_1_PLANE or SCR_2_PLANE
//      XPos - X Position (0 to 19)
//      YPos - Y Position (0 to 18)
// Outputs:
//		PalleteNo - 0-15 the palette number to set
//      TileNo - Tile Number (0 to 511)
//////////////////////////////////////////////////////////////////////////////
   u16 * ScreenPlane1 = SCROLL_PLANE_1;
   u16 * ScreenPlane2 = SCROLL_PLANE_2;

   u16 Offset = ((u16)YPos * 32) + XPos;
  
   switch(Plane)
   {
      case SCR_1_PLANE:
	 *Palette = (u8)(ScreenPlane1[Offset] >> 9);
	 *TileNo = (ScreenPlane1[Offset] & 511);
	 break;
      case SCR_2_PLANE:
	 *Palette = (u8)(ScreenPlane2[Offset] >> 9);
	 *TileNo = (ScreenPlane2[Offset] & 511);
	 break;
      default:
	 break;
   }
}


void CpuSpeed(u8 spd) {
// changes system clock speed
// pass 0 (full) to 4 (slowest)


   __asm("ld rw3, 1");		// VECT_CLOCKGEARSET
   __asm("ld xde, (xsp+4)");	// get spd off stack
   __asm("ld b, e");
   __asm("ld c, 0");		// no speedup on joypad

   // do the system call
   __asm(" ldf 3");
   __asm(" add w,w");
   __asm(" add w,w");
   __asm(" ld xix,0xfffe00");
   __asm(" ld xix,(xix+w)");
   __asm(" call xix");
};

void WaitVsyncHARD(void) {
// put CPU to sleep until the next Vsync starts
	u16 i;

	i = VBCounter;
	while (VBCounter == i)
		__asm(" halt");
};

void WaitVsync(void) {
// wait around doing nothing until the next Vsync starts

    u16 i;

    i = VBCounter;
    while (VBCounter == i);
};

void WaitVblank(void) {
// wait around doing nothing until V Blanking

    if (STS_RG & 0x40 != 0)
	return;			// return immediately if already in Vblank

    while (STS_RG & 0x40 == 0);
};

void Sleep(u8 VBlanks) {
//////////////////////////////////////////////////////////////////////////////
// Sleep
// Waits for specified number of VBlanks (60ths of a second)
//////////////////////////////////////////////////////////////////////////////
   VBCounter = 0;
   CpuSpeed(4);		// run slow while doing nothing

   while (VBCounter < VBlanks);
   CpuSpeed(0);		// resume fullspeed
}


void PrintDecimal(u8 Plane, u8 PaletteNo, u8 x, u8 y, u16 Value, u8 Len) {
//////////////////////////////////////////////////////////////////////////////
// PrintDecimal
// Displays a decimal number of the screen padded with leading zeros
// Inputs:
//		Plane - Scroll Plane to clear SCR_1_PLANE or SCR_2_PLANE
//		PalleteNo - 0-15 the palette number to set
//      x - X Position (0 to 19)
//      y - Y Position (0 to 18)
//      Value - The number to display
//      Len - The number of tiles to fill
//            e.g. Value = 15, Len = 4 will display 0015
//////////////////////////////////////////////////////////////////////////////
   u8 i;

   for (i = Len; i > 0; i--)
   {
	  PutTile(Plane, PaletteNo, x+i-1, y, '0' + (Value % 10));
	  Value /= 10;
   }
}

void PrintNumber(u8 Plane, u8 PaletteNo, u8 x, u8 y, u16 Value, u8 Len) {
// Similar to PrintDecimal, but doesn't print leading zeroes
// prints spaces instead to cover any previous numbers in that tile

   u8 i;

   for (i = Len; i > 0; i--)
   {
	if (Value != 0 || i == Len)
	  PutTile(Plane, PaletteNo, x+i-1, y, '0' + (Value % 10));
	else
	  PutTile(Plane, PaletteNo, x+i-1, y, ' ');
	Value /= 10;
   }
};

void PrintHex(u8 Plane, u8 PaletteNo, u8 x, u8 y, u16 Value, u8 Len) {
// Similar to PrintDecimal, but prints in Hexadecimal
// works well with the BCD values returned by GetTime

   u8 i;

   for (i = Len; i > 0; i--)
   {
	if ((Value & 15) < 10)
	  PutTile(Plane, PaletteNo, x+i-1, y, '0' + (Value & 15));
	else
	  PutTile(Plane, PaletteNo, x+i-1, y, '7' + (Value & 15));

	  Value = Value >> 4;
   }
};

void PrintHex2(u8 Plane, u8 PaletteNo, u8 x, u8 y, u32 Value, u8 Len) {
// Similar to PrintHex, but prints a 32 bit number

   u8 i;

   for (i = Len; i > 0; i--)
   {
	if ((Value & 15) < 10)
	  PutTile(Plane, PaletteNo, x+i-1, y, '0' + (u16)(Value & 15));
	else
	  PutTile(Plane, PaletteNo, x+i-1, y, '7' + (u16)(Value & 15));

	  Value = Value >> 4;
   }
};

void PrintString(u8 Plane, u8 Palette, u8 XPos, u8 YPos, const char * theString) {
//////////////////////////////////////////////////////////////////////////////
// PrintString
// Displays a string on the screen at the specified location
// Inputs:
//		Plane - Scroll Plane to clear SCR_1_PLANE or SCR_2_PLANE
//		PalleteNo - 0-15 the palette number to set
//      XPos - X Position (0 to 19)
//      YPos - Y Position (0 to 18)
//      theString - The string to be displayed
//////////////////////////////////////////////////////////////////////////////
   u16 * Screen;

   switch (Plane)
   {
	  case SCR_1_PLANE:
		 Screen = SCROLL_PLANE_1;
		 break;

	  case SCR_2_PLANE:
		 Screen = SCROLL_PLANE_2;
		 break;

	  default:
		 return;
   }

   while (*theString)
   {
	  u16 Offset = ((u16)YPos * 32) + XPos;
	  u16 Value = *theString + ((u16)Palette << 9);
	  Screen[Offset] = Value;

	  theString++;
	  XPos++;
   }
}

void InstallTileSetAt(const unsigned short Tiles[][8], u16 Len, u16 Offset) {
// similar to InstallTiles but allows you to
// adds tiles to a non-zero location in tile RAM
// useful if a font is installed in the lower tiles
   u16 i;
   u16 * TileRam = TILE_RAM + (Offset * 8);
   u16 * theTiles = (u16 *)Tiles;

   for (i = 0; i < Len; i ++)
   {
      TileRam[i] = *theTiles++;
   }
}


void InstallTileSet(const unsigned short Tiles[][8], u16 Len) {
//////////////////////////////////////////////////////////////////////////////
// InstallTileSet
// Copies your tile data to the NGPC tileram. Takes a 2D array and a
// length. The length field will equal "sizeof(Tiles)/2" which is the
// number of words in the 2D array.
// This style array can be exported directly from Steve Robb's NeoTile
// NGPC Tile Editing tool
//////////////////////////////////////////////////////////////////////////////
	InstallTileSetAt(Tiles, Len, 0);
}

void SetBackgroundColour(u16 Col) {
//////////////////////////////////////////////////////////////////////////////
// SetBackgroundColour
// Changes the background colour of NGPC screen.
// The macro RGB() can be used to specify Col.
// e.g SetBackgroundColour(RGB(15,0,0)); will set the background red
//////////////////////////////////////////////////////////////////////////////
   BG_PAL = Col;
   BG_COL = 0x80;
}




void SetWindowColor(u16 Col) {
// set the color used for outside the viewable display

   OUTSIDE = Col;
}

void SetSprite(u8 SpriteNo, u16 TileNo, u8 Chain, u8 XPos, u8 YPos, u8 PaletteNo) {
//////////////////////////////////////////////////////////////////////////////
// SetSprite
// Initialise a sprite by mapping a tile number and sprite palette no
// to a sprite number and allowing chaining to be set up.
// Inputs:
//		SpriteNo - 0-63 the sprite to move
//      XPos - X Position (0 to 255)
//      YPos - Y Position (0 to 255)
//////////////////////////////////////////////////////////////////////////////
   u16 SprCtrlReg;
   u8 * theSprite = SPRITE_RAM;
   u8 * theSpriteCol = SPRITE_COLOUR;
   
   theSprite += (SpriteNo * 4);
   theSpriteCol += SpriteNo;

   SprCtrlReg = 24; // topmost priority
   if (Chain)
      SprCtrlReg += 6; // v and h chaining

   *(theSprite)   = TileNo;
   *(theSprite+1) = SprCtrlReg+(TileNo>>8);
   *(theSprite+2) = XPos;
   *(theSprite+3) = YPos;

   *theSpriteCol = PaletteNo;
}

void SpriteControl(u8 SpriteNo, u8 Priority, u8 Flips) {
// sets sprites priority or off - defines are in library.h

   u8 * theSprite = SPRITE_RAM + (SpriteNo * 4);

   *(theSprite+1) = *(theSprite+1) & 0x27 | Priority | Flips ;
};

void UnsetSprite(u8 SpriteNo) {
	SpriteControl(SpriteNo, SPR_OFF, 0);
};

void SetSpritePosition(u8 SpriteNo, u8 XPos, u8 YPos) {
//////////////////////////////////////////////////////////////////////////////
// SetSpritePosition
// Moves a already initialise sprite
// Inputs:
//		SpriteNo - 0-63 the sprite to move
//      XPos - X Position (0 to 255)
//      YPos - Y Position (0 to 255)
//////////////////////////////////////////////////////////////////////////////
   u8 * theSprite = SPRITE_RAM;
   theSprite += (SpriteNo * 4);

   *(theSprite+2) = XPos;
   *(theSprite+3) = YPos;
}

void BlockCopy(u8 * Dest, const u8 * Source, u16 n) {
   u8 *p1, *p2;

   p1 = Dest;
   p2 = (u8 *)Source;
   
   while (n--)
	  *p1++ = *p2++;
}



void InstallSoundDriver(void) {
//////////////////////////////////////////////////////////////////////////////
// InstallSoundDriver
// To be called once at the start of the code. It copies the Z80 pre
// assembled SFX driver into the Z80 memory space.
//////////////////////////////////////////////////////////////////////////////
   u8 * pZ80RAM = Z80_RAM;
   u8 i;

   // first stop the z80
   SOUNDCPU_CTRL = 0xAAAA;

   // copy driver
   BlockCopy(pZ80RAM, SFXDriver_Z80, sizeof(SFXDriver_Z80));

   // restart z80
   SOUNDCPU_CTRL = 0x5555;

   // allows the VBI interrupt to start pulsing the Z80
   SFXInstalled = 1;
}


void InstallSounds(const SOUNDEFFECT SoundData[], u8 NumSounds) {
//////////////////////////////////////////////////////////////////////////////
// InstallSounds
// Copies the user defined sounds to the Z80 memory
//////////////////////////////////////////////////////////////////////////////
   u8 Preset;
   u8 i;
   u8 * SFXBuff1 = (u8 *)0x7020;
   u8 * SFXBuff2 = (u8 *)0x7023;
   
   for (Preset = 0; Preset < NumSounds; Preset++)
   {
	   // copy SFX
	  u8 * SData = (u8 *)&SoundData[Preset];

	  // this is yuk but I couldn't find a way to stop the C compiler
	  // doing word alignment on the SoundEffects structure. This
	  // removes the excess padding.
	  BlockCopy(SFXBuff1, SData, 3);
	  SData+=4;
	  BlockCopy(SFXBuff2, SData, sizeof(SOUNDEFFECT)-4);

	  // instruct driver to store it
	  SFXLoadPreset = Preset+1;

	   // wait for completion
	  while(SFXLoadPreset)
		 ;
   }
}

void PlaySound(u8 SoundNumber) {
//////////////////////////////////////////////////////////////////////////////
// PlaySound
// Plays one of the user defined sounds
// Inputs:
//          SoundNumber - the sound preset to play (range 1 to 31)
//
//////////////////////////////////////////////////////////////////////////////
   SFXPlayPreset = SoundNumber;
}

void StopAllSound(void) {
//////////////////////////////////////////////////////////////////////////////
// StopAllSound
// Call this to stop the sound playing in all NGPC sound channels
//////////////////////////////////////////////////////////////////////////////
   SFXStopAll = 1;
}



void SeedRandom(void) {
//////////////////////////////////////////////////////////////////////////////
// SeedRandom
// Unfortunately we need a pseudo random number in order to seed our
// pseudo random number generator otherwise the numbers would always
// come out the same.
// To solve this I use VBCounter that is incremented every VB
// Interrupt. If this function was called at initialisation the same
// problem would arrise as VBCounter will have always hit the same
// value.
// Call SeedRandom just after the user has pressed fire/start from the
// title screen of your game.
//////////////////////////////////////////////////////////////////////////////
   RAND_RandomData = VBCounter;
   GetRandom(100);
}



s32 Multiply32bit(s32 Value1, s32 Value2) {
// helper function as there is no 32bit multiply natively supported by
// the compiler :-0
   __ASM("  LD      XWA, (XSP+4)");
   __ASM("  LD      XBC, (XSP+8)");
   __ASM("  LD		HL,QWA");
   __ASM("  MUL		XHL,BC");
   __ASM("  LD		DE,QBC");
   __ASM("  MUL		XDE,WA");
   __ASM("  ADD		XHL,XDE");
   __ASM("  LD		QHL,HL");
   __ASM("  LD		HL,0");
   __ASM("  MUL		XWA,BC");
   __ASM("  ADD		XHL,XWA");

   return __XHL;
}

u16 GetRandom(u16 Value) {
//////////////////////////////////////////////////////////////////////////////
// GetRandom
// Returns a pseudo random number in the range of 0 to the specified
// input value.
// Inputs:
//     Value = the upper limit for the random number (must be less than 32767)
//////////////////////////////////////////////////////////////////////////////
   RAND_RandomData = Multiply32bit(RAND_RandomData,20077);
   RAND_RandomData += 12345;
   
   return (Multiply32bit(((RAND_RandomData >> 16) & RAND_MAX), Value) >> 15);
}


void InitialiseQRandom(void) {
   RandomNumberCounter=0;
}

unsigned char QRandom(void) {
    //Um. RandomNumberCounter is an unsigned char...
    //which means that this routine will never get beyond the first
    //255 elements of the random number array?
    //Oh well. Should be good enough for my uses anyway...
   return QuickRandomNumbers[RandomNumberCounter++];
}


signed char Sin(unsigned char dAngle) {
   return SineTable256[dAngle];
}

signed char Cos(unsigned char dAngle) {
   return SineTable256[dAngle+64];
}

void GetTime(TIME * pTime) {
   __asm(" ld rw3, 2");		// VECT_RTCGET
   __asm(" ld xde, (xsp+4)");	// get ptr off stack
   __asm(" ld xhl3, xde");	// pass in this register

   // do the system call
   __asm(" ldf 3");
   __asm(" add w,w");
   __asm(" add w,w");
   __asm(" ld xix, 0xfffe00");
   __asm(" ld xix,(xix+w)");
   __asm(" call xix");
}

void SetWake(ALARM * pAlarm) {
// sets system to wakeup if powered off
// pass in a pointer to a alarm structure

   __asm(" ld rw3, 11");	// VECT_ALARMDOWNSET
   __asm(" ld xiy, (xsp+4)");	// get ptr
   __asm(" ldf 3");		// params go in reg bank 3
                // could probably do a 32 bit copy
                // to xbc but this looks better
   __asm(" ld h, (xiy +)");	// day of month
   __asm(" ld qc, h");
   __asm(" ld b, (xiy +)");	// hour
   __asm(" ld c, (xiy +)");	// min

   // do the system call
   __asm(" add w,w");
   __asm(" add w,w");
   __asm(" ld xix,0xfffe00");
   __asm(" ld xix,(xix+w)");
   __asm(" call xix");

   // returns success in A3 someday
//   __asm(" ld (xiy), a");
}

void SetAlarm(ALARM * pAlarm) {
// sets an alarm while powered up
// I don't see much use in this one, but...
// need to setup interrupt first to catch this one
// and do something with it
// pass in a pointer to a alarm structure

   __asm(" ld rw3, 9");		// VECT_ALARMSET
   __asm(" ld xiy, (xsp+4)");	// get ptr
   __asm(" ldf 3");		// params go in reg bank 3
                // could probably do a 32 bit copy
                // to xbc but this looks better
   __asm(" ld h, (xiy +)");	// day of month
   __asm(" ld qc, h");
   __asm(" ld b, (xiy +)");	// hour
   __asm(" ld c, (xiy +)");	// min

   // do the system call
   __asm(" add w,w");
   __asm(" add w,w");
   __asm(" ld xix,0xfffe00");
   __asm(" ld xix,(xix+w)");
   __asm(" call xix");

// return success in A3 someday
//   __asm(" ld (xiy+3), a3");
}


void SysPlayWave(u8 *wave) {
// play 8KHz wave samples

	__asm(" extern large WAVE_OUT");
	__asm(" ld xwa,(XSP+0x4)");
	__asm(" ld xhl3,xwa");
	__asm(" ldb	ra3,1");
	__asm(" calr WAVE_OUT");
}

void NeoTracker_InstallDriver(void) {
//////////////////////////////////////////////////////////////////////////////
// NeoTracker_InstallDriver
// To be called once at the start of the code. It copies the Z80 pre
// assembled SFX driver into the Z80 memory space.
//////////////////////////////////////////////////////////////////////////////
   u8 * pZ80RAM = Z80_RAM;
   u8 i;

   // first stop the z80
   SOUNDCPU_CTRL = 0xAAAA;

   // copy driver
   BlockCopy(pZ80RAM, NeoTracker_Driver, sizeof(NeoTracker_Driver));

   // restart z80
   SOUNDCPU_CTRL = 0x5555;

   TRUN &= 0xF7;    // turn off timer 3
   T23MOD &= 0x33;  // set timers to 8 bit
   T23MOD |= 4;     // t3 internal clock = t1
   TFFCR &= 0x0f;   // clear timer 3 settings
   TFFCR |= 0xb0;   // clear flip flop and flip on timer 3
   TREG3 = 0x62;    // set comparitor
   TRUN = 0x88;     // turn timer 3 back on
   
}

void NeoTracker_SendGroup(const u8 theGroup[], const int Len) {
// sends a group the z80 driver
   u8 i;
   volatile u8 ack;
   u8 * pZ80Mem = Z80_RAM;
   pZ80Mem += sizeof(NeoTracker_Driver);
   
	// stop any sound playing
   NeoTracker_StopAll();
		 
	// wait a while for the driver to ack the stop command
   for (i = 0; i < 100; i++)
   {
	  volatile u8 Value = Z80_COMM;
	  if (Value == 0)
		 break;
   }
   // if no ack then reinstall the driver
   if (i == 100)
	  NeoTracker_InstallDriver();

   // copy the group
   BlockCopy(pZ80Mem, theGroup, Len);

   // wait for acknowledge
   ack = NEOTRACKER_ACK;
   do
   {
	  Sleep(1);
   } while (ack == NEOTRACKER_ACK);
}


void NeoTracker_PlayMusic(const int Number) {
// plays specified music
   u8 RealNum = (Number << 2) | 3;
   // instruct driver
   Z80_COMM = RealNum;
   Z80_NMI = 1;
}

void NeoTracker_PlaySFX(const int Number) {
// plays a SFX - may temp interrupt the music
   u8 RealNum = (Number << 2) | 2;
   // instruct driver
   Z80_COMM = RealNum;
   Z80_NMI = 1;
}

void NeoTracker_StopAll(void) {
// Stops any sound playing (music and SFX)
   // instruct driver
   Z80_COMM = 0xff;
   Z80_NMI = 1;
}


/*
//////////////////////////////////////////////////////////////////////////////
// Bitmap Mode support - Chris Ahchay
//////////////////////////////////////////////////////////////////////////////

//Bit of a rethink here. This is a vanilla bitmap function.
//It works, up to a point, but there are a ton of problems with it
//for real world use:
//1, You can only create a single bitmap.
//2. It's slow (probably not much I can do about that)
//3. Blt'ing bits is painful.
//So...
//Retool this to...

//Pass a memory address into the functions. This replaces the top/left
//co-ordinates.
//Everything works relative to this address, rather than fixed to BitmapTileBase
//Write a third function (or, more accurately, redo CopySingleTile()) to blt
//one bitmap into another (i.e. move from an array to the screen)

//The bitmap passed must be big enough to store (Width*Height)+8) elements
//defined as (to store a 160 by 96 size bitmap (160/8)*(96/8)=240 for the
//bitmap tiles and eight elements for the bitmap properties.
//u16 Bitmap[241][8]
//or
//u16 Bitmap[1928]
//Doesn't matter which, but the first is the easiest to use IMHO...
*/
void CreateBitmap(u16 * BitmapAddress, u8 Width, u8 Height) {
	u8 iLoopX;
	u8 iLoopY;
	u16 * MyAddress= BitmapAddress;
	u8 iFillLoop;

	//Create an array of tiles for a bitmap operation thang

	//Save the bitmap dimensions for later
	//Use the *first* element of the bitmap array to store the properties of the
	//bitmap.
	MyAddress[0]=Width;
	MyAddress[1]=Height;
	//Can put other stuff in here
	//MyAddress[2]=BackgroundColour
	//MyAddress[3]=Colour 1
	//MyAddress[4]=Colour 2
	//MyAddress[5]=Colour 3
	//MyAddress[6]=X Position
	//MyAddress[7]=Y Position
	MyAddress+=8;

	for (iLoopX=0;iLoopX<(Width/8);iLoopX++)
	{
		for (iLoopY=0;iLoopY<(Height/8);iLoopY++)
		{
			//Copies a blank tile into the bitmap
			for (iFillLoop=0;iFillLoop<8;iFillLoop++)
			{
				MyAddress++[0]=0;
			}

		}
	}
}

#if 0	// old version
void SetPixel(u16 * BitmapAddress, u16 xPosition, u16 yPosition, u16 Colour) {
	//Set the selected pixel in the bitmap
	u16 CurrentTile;
	//u16 TileRow;
	u16 * TileRam = BitmapAddress+8;

	//Trouble is. This needs to know the width and height of the bitmap (well, probably not the height)

	//Fist of all, identify the tile that the specified bitmap lives inside...
	//This can be derived from xPosition and yPosition

	CurrentTile=(xPosition/8)+((yPosition/8)*(BitmapAddress[0]/8));
	//For now, copy our "rubbish" tile into the tile space.
	//This is just to prove that I'm in the right general area...
	//CopySingleTile(RubbishTile, CustomTileBase+CurrentTile);

	//Having got the tile number. We now need to calculate the *new* tile
	//to put into here.

	//Basic method.
	//Get the existing tile into an unsigned short [8] Array
	//I don't need the whole tile. Just the row (yPosition % 8)
	//Grab a pointer to the relevant tile.
	//TileRam += (BitmapTileBase * 8);
	TileRam += (CurrentTile * 8)+(yPosition%8);
	//& Then... Grab the value held in this address...
	//TileRow = *TileRam;


	//Do clever stuff with AND's and OR's with the existing tile and the BitmapMask
	//array(s?)
	//Yer basic method;
	//AND MaskRow[xPosition%8] TileRow
	//OR ColourRow[xPosition%8] TileRow
	//Where MaskRow & ColourRow are 16bit jobbies showing a "holey" bitmap and a "coloured" bitmap
	//respectively. (Load the values in bitmap.h into neotile to see what I mean)

	//Updated to work directly on the tileram. Slightly quicker...
	TileRam[0]=TileRam[0] & BitmapMask[xPosition%8];
	TileRam[0]=TileRam[0] | BitmapNewMask[Colour][xPosition%8];

	//Put the tile back...
	//TileRam[0] = TileRow;

}
#endif
void SetPixel(u16 * BitmapAddress, u16 xPosition, u16 yPosition, u16 Colour) {
	//Set the selected pixel in the bitmap
	// multiplies and divides replaced by shifts and logic operations
	u16 CurrentTile;
	u16 * TileRam = BitmapAddress+8;

	CurrentTile=(xPosition>>3)+((yPosition>>3)*(BitmapAddress[0]>>3));
	TileRam += (CurrentTile<<3)+(yPosition&7);
	TileRam[0]=TileRam[0] & BitmapMask[xPosition&7];
	TileRam[0]=TileRam[0] | BitmapNewMask[Colour][xPosition&7];

};

void QuickSetPixel(u16 * BitmapAddress, u16 xPosition, u16 yPosition, u16 Colour) {
	//To be used when we can guarantee that only a single pixel in a tile row is affected.
	u16 CurrentTile;
	u16 * TileRam = BitmapAddress+8;

	CurrentTile=(xPosition/8)+((yPosition/8)*(BitmapAddress[0]/8));
	TileRam += (CurrentTile * 8)+(yPosition%8);
	TileRam[0] = BitmapNewMask[Colour][xPosition%8];
}

//Don't worry about GetPixel() yet...
/*
u8 GetPixel(u16 xPosition, u16 yPosition)
{
	//Get the selected pixel in the bitmap

	//Sort of based on SetPixel() Let's get that working first..
	u16 CurrentTile;
	u16 TileRow;
	u16 * TileRam = TILE_RAM;
	u8 iReturn;
	u8 iOffset;
	u16 lHighBit;
	u16 lLowBit;
	//Fist of all, identify the tile that the specified bitmap lives inside...
	//This can be derived from xPosition and yPosition

	CurrentTile=(xPosition/8)+((yPosition/8)*(CurrentBitmapWidth/8));

	//Basic method.
	//Get the existing tile into an unsigned short [8] Array
	//I don't need the whole tile. Just the row (yPosition % 8)
	TileRam += (BitmapTileBase * 8);
	TileRam += (CurrentTile * 8)+(yPosition%8);

	//Fetch the actual value of the tile row...
	TileRow = *TileRam;

	//Work out which *bits* are used for this pixel
	iOffset=(7-(xPosition%8));
	lLowBit = OffsetValues[iOffset];
	lHighBit = (lLowBit * 2);
	//Right then. Are any of these set?
	iReturn=0;
	if ((TileRow & (lHighBit+lLowBit))==(lHighBit+lLowBit))
		iReturn=3;
	else if ((TileRow & (lHighBit))==(lHighBit))
		iReturn=2;
	else if ((TileRow & (lLowBit))==(lLowBit))
		iReturn=1;

	//Send something back...
	return iReturn;
}
*/

void PutBitmapTile(u16 * BitmapAddress, u16 InputTile, u16 OutputTile) {
//Try it this way...
//Blts a single "bitmap" tile into TILE_RAM (one way?)
//Could we use CopySpriteTile()? Looks pretty similar to me.
	//A generic loop variable
	u16 i;
	//TileRam is a pointer to the beginning of tile memory
	u16 * TileRam = TILE_RAM;
	//theTiles is a pointer to the beginning of our Tiles array.
	//Copy a single tile into the specified tile number
	u16 * theTiles = (u16 *)BitmapAddress;

	TileRam += (OutputTile * 8);
	theTiles += (InputTile * 8);

	for (i = 0; i < 8; i ++)
	{
		//Copy the tile data
		TileRam[i] = *theTiles++;
	}
}

void CopyBitmap(u16 * BitmapAddress, u16 TileNumber) {
	u8 iLoopX;
	u8 iLoopY;
	u8 TileWidth;
	u8 TileHeight;
	u16 * MyAddress= BitmapAddress+8;

	TileWidth=BitmapAddress[0]/8;
	TileHeight=BitmapAddress[1]/8;
	//Copy the bitmap to the screen.
	for (iLoopX=0;iLoopX<TileWidth;iLoopX++)
	{
		for (iLoopY=0;iLoopY<TileHeight;iLoopY++)
		{
			PutBitmapTile((u16 *)MyAddress, (iLoopY*TileWidth)+iLoopX, TileNumber+(iLoopY*TileWidth)+iLoopX);
		}
	}
}

void DrawBitmap(u16 * BitmapAddress, u8 xPosition, u8 yPosition, u16 TileNumber) {
	u8 iLoopX;
	u8 iLoopY;
	u8 TileWidth;
	u8 TileHeight;

	TileWidth=BitmapAddress[0]>>3;
	TileHeight=BitmapAddress[1]>>3;

	//Copy the bitmap tiles into tile memory and blatt them onto the screen.
	for (iLoopX=0;iLoopX<TileWidth;iLoopX++)
	{
		for (iLoopY=0;iLoopY<TileHeight;iLoopY++)
		{
			PutTile(SCR_1_PLANE, 0, iLoopX+xPosition, iLoopY+yPosition, TileNumber+(iLoopY*TileWidth)+iLoopX);
		}
	}

}
void DrawLine(u16 * BitmapAddress, u8 x1, u8 y1, u8 x2, u8 y2, u8 Colour) {
	//Stolen wholesale from gamedev...
	u8 xinc1;
	u8 xinc2;
	u8 yinc1;
	u8 yinc2;
	u8 den;
	u8 num;
	u8 numadd;
	u8 numpixels;
	u8 curpixel;
	u8 deltax;
	u8 deltay;
	u8 x;
	u8 y;

	//There appears to be an absence of an abs() function.
	if (x2>x1)
		deltax = x2 - x1;        // The difference between the x's
	else
		deltax = x1 - x2;
	if (y2>y1)
		deltay = y2 - y1;        // The difference between the y's
	else
		deltay = y1 - y2;

	x = x1;                       // Start x off at the first pixel
	y = y1;                       // Start y off at the first pixel

	if (x2 >= x1)                 // The x-values are increasing
	{
		xinc1 = 1;
		xinc2 = 1;
	}
	else                          // The x-values are decreasing
	{
		xinc1 = -1;
		xinc2 = -1;
	}

	if (y2 >= y1)                 // The y-values are increasing
	{
		yinc1 = 1;
		yinc2 = 1;
	}
	else                          // The y-values are decreasing
	{
		yinc1 = -1;
		yinc2 = -1;
	}

	if (deltax >= deltay)         // There is at least one x-value for every y-value
	{
		xinc1 = 0;                  // Don't change the x when numerator >= denominator
		yinc2 = 0;                  // Don't change the y for every iteration
		den = deltax;
		num = deltax / 2;
		numadd = deltay;
		numpixels = deltax;         // There are more x-values than y-values
	}
	else                          // There is at least one y-value for every x-value
	{
		xinc2 = 0;                  // Don't change the x for every iteration
		yinc1 = 0;                  // Don't change the y when numerator >= denominator
		den = deltay;
		num = deltay / 2;
		numadd = deltax;
		numpixels = deltay;         // There are more y-values than x-values
	}

	for (curpixel = 0; curpixel <= numpixels; curpixel++)
	{
		SetPixel((u16 *)BitmapAddress, x, y, Colour);             // Draw the current pixel
		num += numadd;              // Increase the numerator by the top of the fraction
		if (num >= den)             // Check if numerator >= denominator
		{
			num -= den;               // Calculate the new numerator value
			x += xinc1;               // Change the x as appropriate
			y += yinc1;               // Change the y as appropriate
		}
		x += xinc2;                 // Change the x as appropriate
		y += yinc2;                 // Change the y as appropriate
	}
}


//Two specific cases for vertical and horizontal lines
void DrawLineVertical(u16 * BitmapAddress, u16 x, u16 y1, u16 y2, u16 Colour) {
	u16 y;
	s8 yinc;

	if (y1>y2)
		yinc=-1;
	else
		yinc=1;

	for (y=y1;y!=y2;y+=yinc)
	{
		SetPixel((u16 *)BitmapAddress, x, y, Colour);
	}
}


void DrawLineHorizontal(u16 * BitmapAddress, u16 x1, u16 x2, u16 y, u16 Colour) {
	u16 x;
	s8 xinc;

	if (x1>x2)
		xinc=-1;
	else
		xinc=1;

	for (x=x1;x!=x2;x+=xinc)
	{
		SetPixel((u16 *)BitmapAddress, x, y, Colour);
	}
};

void ClearBitmap_2(void) {
// clear all bitmapped tileram

	u16 * TileRam = TILE_RAM;
	u16 k;

	for (k=BitBase_2*8;k<20*11*8*2+BitBase_2*8;k++)
		TileRam[k] = 0;
};

void CreateBitmap_2(void) {

	u16  k, l;
	u16 * Screen1 = SCROLL_PLANE_1;
	u16 * Screen2 = SCROLL_PLANE_2;
	u16 * TileRam = TILE_RAM;

//	put sequential tiles into both text areas
//	both use palette 0 of respective plane
    for (k=0;k<11;k++) {
	for (l=0;l<20;l++) {
	    Screen1[l] = k*20 +l +BitBase_2;
	    Screen2[l] = k*20 +l +220 +BitBase_2;
	}
	Screen1 += 32;
	Screen2 += 32;
    };

// clear text line - sets to space
// clear rest of tiles too - just because
	for (k=0;k<21;k++) {
		for (l=0;l<20;l++) {
			Screen1[l] = 0;
			Screen2[l] = 0;
		}
		Screen1 += 32;
		Screen2 += 32;
	};
};

void SetPixel_2(u8 xp, u8 yp, u8 col) {
// set a single pixel in bitmapped mode
// colors are 0-6, x is 0-159, y is 0-87

u16 * BitRam1 = (u16*)(0xa000 + BitBase_2*16);
u16 * BitRam2 = (u16*)(0xa000 + BitBase_2*16 + 220*16);

	u16 yl = (u16)yp;
	u16 xl = (u16)xp;
	u16 mem;

// ???
// y = 0,1,2,3,4,5,6,7,16,17,18...
// x = 0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1...

// 8 bits X = next word and one Y = 20 words/8
// clear both planes
//	mem = (yl/8)*160 + (yl%8) + (xl/8)*8;
	mem = (yl>>3)*160 + (yl&7) + (xl&65528);
	BitRam1[mem] &= BitmapMask[xp&7];
	BitRam2[mem] &= BitmapMask[xp&7];

	if (col == 0) return;
	if (col > 6)  return;

// colors are 0,1,2,3,1,2,3
	if (col > 3)
		BitRam2[mem] |= BitmapNewMask[col-3][xp&7];
	else
		BitRam1[mem] |= BitmapNewMask[col][xp&7];
};

void SetMoreColors(u8 Plane, u8 PalStart, u8 PalSize, const u16 Palette[][3]) {
// like SetColors() but sets many colors at once

    u16 * thePalette;
    u8 Offset = PalStart * 4;
    u8 i;

   switch (Plane)   {
	  case SCR_1_PLANE:
		 thePalette = SCROLL_1_PALETTE;
		 break;
	  case SCR_2_PLANE:
		 thePalette = SCROLL_2_PALETTE;
		 break;
	  case SPRITE_PLANE:
		thePalette = SPRITE_PALETTE;
		break;
	  default:
		 return;
   }

    for (i=0;i < PalSize; i++) {
       thePalette[Offset+1] = Palette[i][0];
       thePalette[Offset+2] = Palette[i][1];
       thePalette[Offset+3] = Palette[i][2];
       Offset +=4;
    }
}

void FillScreen(u8 Plane, u16 tilenum, u8 palette) {
// like ClearScreen except it allows any value to be
// put into the 32*32 area of a scrolling plane

   int i;
   u16 * Screen;
   u16 val = tilenum + ((u16)palette << 9);

   switch(Plane)
   {
	  case SCR_1_PLANE:
		 Screen = SCROLL_PLANE_1;
		 break;
	  case SCR_2_PLANE:
		 Screen = SCROLL_PLANE_2;
		 break;
	  default:
		 return;
   }

   for (i = 0; i < 32*32; i ++)
	  Screen[i] = val;
}

void TileScreen(u8 Plane, const u16 text[19][20]) {
// fills the 19*20 area of a scrolling lane with an
// array of values

   int i, j;
   u16 * Screen;

   switch(Plane)
   {
	  case SCR_1_PLANE:
		 Screen = SCROLL_PLANE_1;
		 break;
	  case SCR_2_PLANE:
		 Screen = SCROLL_PLANE_2;
		 break;
	  default:
		 return;
   }

    for (i = 0; i < 19; i ++) {
	for (j = 0; j < 20; j++)
	    Screen[j] = text[i][j];
	Screen += 32;
    }
}

void SetScreen(u8 lx, u8 ly, u8 mx, u8 my) {
// sets the viewable screen size

   WIN_X = lx;
   WIN_Y = ly;
   WIN_W = mx;
   WIN_H = my;
}

void ShiftScroll(u8 Plane, u8 hz, u8 vt) {
// move the scrolling plane to a new screen location

   switch(Plane)
   {
	  case SCR_1_PLANE:
		SCR1_X = hz;
		SCR1_Y = vt;
		 break;
	  case SCR_2_PLANE:
		SCR2_X = hz;
		SCR2_Y = vt;
		 break;
	  default:
		 return;
    }
};

u8 SwapPlanes(void) {
	SCR_PRIORITY ^= 0x80;
	return SCR_PRIORITY;
};

void Flash(void *data) {
	__ASM("SAVEOFFSET	EQU	0x1e0000");

	__ASM("BLOCK_NB		EQU	30");
	__ASM("VECT_FLASHWRITE	EQU	6");
	__ASM("VECT_FLASHERS	EQU	8");
	__ASM("rWDCR		EQU	0x6f");
	__ASM("WD_CLR		EQU	0x4e");

	// Erase block first (mandatory) : 64kb for only 256 bytes
	__ASM("	ld	ra3,0");
	__ASM("	ld	rb3,BLOCK_NB");
	__ASM("	ld	rw3,VECT_FLASHERS");
	__ASM("	ld	(rWDCR),WD_CLR");
	__ASM("	swi	1");

	// Then write data
	__ASM("	ld	ra3,0");
	__ASM("	ld	rbc3,1");	// 256 bytes
	__ASM("	ld	xhl,(xsp+4)");
	__ASM("	ld	xhl3,xhl");
	__ASM("	ld	xde3,SAVEOFFSET");
	__ASM("	ld	rw3,VECT_FLASHWRITE");
	__ASM("	ld	(rWDCR),WD_CLR");
	__ASM("	swi	1");

	__ASM("	ld	(rWDCR),WD_CLR");
}

void GetSavedData(void *data) {
	u32 *ptr = (u32*)(0x200000+0x1e0000);
	u32 *ptrData = (u32*)data;
	u8 i;
	if (*ptr == MAGIC_NB) // Data saved
	{
		for (i=0;i<64;i++)
			ptrData[i] = ptr[i];
	}
	else // No data
	{
		ptrData[0] = MAGIC_NB;
		for (i=1;i<64;i++)
			ptrData[i] = 0;
	}
}

// EOF

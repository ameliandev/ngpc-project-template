/*****************************************************************
 *
 * C Library of useful, generic NGPC routines
 *
 *****************************************************************/

/* --Routines in this module----------------------------------- *

SYSTEM Functions
   InitNGPC()			set interrupts and system registers
   ResumeOff()			turns system off with resume option

TIME Functions
   GetTime()			gets time from hardware RTC
   SetWake()			sets time to poweron when off
   SetAlarm()			sets alarm while on
   CpuSpeed()			changes CPU clock from 6MHz thru 384KHz
   WaitVsync()			slows cpu and waits for next vertical sync
   WaitVblank()			wait for vertical blank (exits if already there)
   WaitVsyncHARD()		halts cpu until vertical sync (real hardware only)
   Sleep()			slows CPU and waits time in 1/60 seconds

TEXT Functions
   PrintDecimal()		print a number with leading zeroes
   PrintNumber()		print a number on the display
   PrintHex()			print a number in hexadecimal
   PrintHex2()			print a 32 bit number in hexadecimal
   PrintString()		print a text message

TILE Functions
   InstallTileSet()		install tilemaps at base
   InstallTilesSetAt()		installs tilemaps anywhere
   SysSetSystemFont()		install font tiles
   TileScreen()			fills 19x20 screen with characters
   FillScreen()			sets all tiles of 19x20 screen to one tile
   GetTile()
   PutTile()			set one tile

SCREEN Functions
   ClearScreen()		sets all tiles of a plane to tile 0 (not always blank)
   SetScreen()			sets active screen size
   ShiftScroll()		moves scrolling planes
   SwapPlanes()			exchanges foreground/background planes							*

COLOR Functions
   SetBackgroundColour()	set color of background
   SetPalette()			sets 4 color palette (like GB)
   SetColors()			sets three color palette
   SetMoreColors()		sets many palettes at once
   SetWindowColor()		set color for outside viewable window

SPRITE Functions
   SetSprite()			initialize a sprite
   SetSpritePosition()		move a sprite
   SpriteControl()		change sprite priority
   UnsetSprite()		turn off sprite

RANDOM Functions
   SeedRandom()			init random number genrator
   GetRandom()			get a random number
   InitialiseQRandom()		init quick random numbers
   QRandom()			get a random number fast

MATH Functions
   Multiply32bit()		32 bit signed multiply
   Sin()			primitive sine
   Cos()			primitive cosine

SOUND Functions - Basic
   InstallSoundDriver()
   InstallSounds()
   PlaySound()
   StopAllSounds()

SOUND Functions - NeoTracker
   NeoTracker_InstallDriver()
   NeoTracker_SendGroup()
   NeoTracker_PlayMusic()
   NeoTracker_PlaySFX();
   NeoTracker_StopAll();

BITMAP MODE Functions
   CreateBitmap()
   SetPixel()
   PutBitmapTile()
   DrawLine()

   CreateBitmap_2()
   SetPixel_2()
   ClearBitmap_2()

FLASH SAVE Functions
   Flash()
   GetSavedData()

	untested ones
   GetPixel()
   CopySingleTile()

 * ------------------------------------------------------------ */

#ifndef _LIBRARY_H
#define _LIBRARY_H

/*
 * Defines used by the library
 */
#define SCR_1_PLANE	(1)
#define SCR_2_PLANE	(2)
#define SPRITE_PLANE	(3)

// get true battery reading (0x0000-0x03FF)
#define BATTERY			(*(u16*)0x6F80)
// true color register for outside displayed area
#define OUTSIDE			(*(u16*)0x83F0)

#define MAGIC_NB 0xcafebabe	// java rules !
/*
 * Public variables
 */
extern volatile u8 VBCounter;
extern const char DayNames[7][10];
extern const unsigned short BitmapMask[8];
extern const unsigned short BitmapNewMask[4][8];

// data structures

typedef struct tagALARM
{
   u8 Day;
   u8 Hour;
   u8 Min;
   u8 Code;
} ALARM;

typedef struct tagTIME
{
   u8 Year;
   u8 Month;
   u8 Day;
   u8 Hour;
   u8 Minute;
   u8 Second;
   u8 LeapYear:4;
   u8 Weekday:4;
} TIME;


typedef struct tagSoundEffect
{
   u8 Channel;
   u8 Length;
   u8 Repeat;
   u16 InitialTone;
   u16 ToneStep;
   u8 ToneSpeed;
   u8 ToneOWB;
   u16 ToneLowerLimit;
   u16 ToneUpperLimit;
   u8 InitialVol;
   u8 VolStep;
   u8 VolSpeed;
   u8 VolOWB;
   u8 VolLowerLimit;
   u8 VolUpperLimit;
} SOUNDEFFECT;

/*
 * Function Prototypes contained within the library
 */

void InitNGPC(void);
void InstallTileSet(  const unsigned short Tiles[][8], u16 Len);
void InstallTileSetAt(const unsigned short Tiles[][8], u16 Len, u16 Offset);

void ClearScreen(u8 ScrollPlane);
void SysSetSystemFont(void);

void SetBackgroundColour(u16 Col);
void SetWindowColor(u16 Col);
void SetPalette(u8 ScrollPlane, u8 PaletteNo, u16 Col0, u16 Col1, u16 Col2, u16 Col3);
void SetColors(u8 Plane, u8 PaletteNo, u16 Col1, u16 Col2, u16 Col3);
void SetMoreColors(u8 Plane, u8 PalStart, u8 PalSize, const u16 Palette[][3]);

void WaitVsync(void);
void WaitVblank(void);
void Sleep(u8 VBLanks);
void WaitVsyncHARD(void);

void GetTile(u8 ScrollPlane, u8 *PaletteNo, u8 XPos, u8 YPos, u16 *TileNo);
void PutTile(u8 ScrollPlane, u8 PaletteNo, u8 XPos, u8 YPos, u16 TileNo);

void PrintDecimal(u8 Plane, u8 PaletteNo, u8 x, u8 y, u16 Value, u8 Len);
void PrintNumber( u8 Plane, u8 PaletteNo, u8 x, u8 y, u16 Value, u8 Len);
void PrintHex( u8 Plane, u8 PaletteNo, u8 x, u8 y, u16 Value, u8 Len);
void PrintHex2(u8 Plane, u8 PaletteNo, u8 x, u8 y, u32 Value, u8 Len);
void PrintString(u8 Plane, u8 Palette, u8 XPos, u8 YPos, const char * theString);

void SetSprite(u8 SpriteNo, u16 TileNo, u8 Chain, u8 XPos, u8 YPos, u8 PaletteNo);
void SetSpritePosition(u8 SpriteNo, u8 XPos, u8 YPos);

s32 Multiply32bit(s32 Value1, s32 Value2);
void SeedRandom(void);
u16 GetRandom(u16 Value);
void InitialiseQRandom(void);
unsigned char QRandom(void);

signed char Sin(unsigned char dAngle);
signed char Cos(unsigned char dAngle);


void GetTime(TIME * pTime);
void SetWake(ALARM * pAlarm);
void SetAlarm(ALARM * pAlarm);

void CpuSpeed(u8 spd);
void ResumeOff(void);

// Sound support
void SysPlayWave(u8 *wave);
void InstallSoundDriver(void);
void InstallSounds(const SOUNDEFFECT SoundData[], u8 NumSounds);
void PlaySound(u8 SoundNumber);
void StopAllSounds(void);

// NeoTracker Support
void NeoTracker_InstallDriver(void);
void NeoTracker_SendGroup(const u8 theGroup[], const int Len);
void NeoTracker_PlayMusic(const int Number);
void NeoTracker_PlaySFX(const int Number);
void NeoTracker_StopAll(void);

// bitmap mode support
void CreateBitmap(u16 * BitmapAddress, u8 Width, u8 Height);
void SetPixel(u16 * BitmapAddress, u16 xPosition, u16 yPosition, u16 Colour);
//u8 GetPixel(u16 xPosition, u16 yPosition);
//void CopySingleTile(u8 TileGraphic, u16 TileNumber);
void PutBitmapTile(u16 * BitmapAddress, u16 InputTile, u16 OutputTile);
void DrawLine(u16 * BitmapAddress, u8 x1, u8 y1, u8 x2, u8 y2, u8 Colour);

void CreateBitmap_2(void);
void SetPixel_2(u8 xp, u8 yp, u8 col);
void ClearBitmap_2(void);

void FillScreen(u8 Plane, u16 tilenum, u8 palette);
void TileScreen(u8 Plane, const u16 text[19][20]);
void SetScreen(u8 lx, u8 ly, u8 mx, u8 my);

void Flash(void *data);
void GetSavedData(void *data);

void ShiftScroll(u8 Plane, u8 hz, u8 vt);

u8 SwapPlanes(void);

// sprite priorities
#define SPR_OFF		0
#define SPR_FURTHEST	(u8)(1<<3)
#define SPR_MIDDLE	(u8)(2<<3)
#define SPR_FRONT	(u8)(3<<3)
// sprite flips
#define SPR_VFLIP	(u8)(1<<6)
#define SPR_HFLIP	(u8)(2<<6)
#define SPR_BFLIP	(u8)(3<<6)

void SpriteControl(u8 SpriteNo, u8 Priority, u8 Flips);
void UnsetSprite(u8 Spriteno);

#endif

// EOF


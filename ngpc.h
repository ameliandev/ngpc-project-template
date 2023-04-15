//////////////////////////////////////////////////////////////////////////////
// ngpc.h                                                                   //
//////////////////////////////////////////////////////////////////////////////
#ifndef _NGPC_H
#define _NGPC_H

//////////////////////////////////////////////////////////////////////////////
// types                                                                    //
//////////////////////////////////////////////////////////////////////////////
typedef unsigned char	u_char;
typedef unsigned short	u_short;
typedef unsigned long	u_long;
typedef unsigned char	u8;
typedef unsigned short	u16;
typedef unsigned long	u32;
typedef signed   char	s8;
typedef signed   short	s16;
typedef signed   long	s32;

typedef u8 bool;

#define RGB(r,g,b) ((u16)(r)&15) | (((u16)(g)&15)<<4) | (((u16)(b)&15)<<8)

typedef void __interrupt Interrupt();
typedef void (*FuncPtr)(void);

#define TRUN                (*(u8*)0x0020)
#define TFFCR               (*(u8*)0x0025)
#define TREG3               (*(u8*)0x0027)
#define T23MOD              (*(u8*)0x0028)

#define Z80_NMI             		(*(u8*)0x00BA)
#define Z80_COMM            		(*(u8*)0x00BC)
#define WATCHDOG			(*(u8*)0x006F)
#define SOUNDCPU_CTRL       		(*(u16 *)0x00b8)
#define INT_ROM				(*(u8*)0xFF0000)
#define CART_ROM			(*(u8*)0x200000)
#define CHAR_RAM			(*(u8*)0xA000)
#define WORK_RAM			(*(u8*)0x4000)
#define MAIN_RAM			(*(u8*)0x4000)
#define BAT_VOLT			(*(u8*)0x6F80)
#define JOYPAD				(*(volatile u8*)0x6F82)
#define SYS_LEVER			(*(u8*)0x6F82)
#define USR_BOOT			(*(u8*)0x6F84)
#define USR_SHUTDOWN			(*(u8*)0x6F85)
#define USR_ANSWER			(*(u8*)0x6F86)
#define LANGUAGE			(*(u8*)0x6F87)
#define OS_VERSION			(*(u8*)0x6F91)
#define DISP_CTL0			(*(u8*)0x8000)
#define STS_RG				(*(u8*)0x8010)
#define LCD_CTR				(*(u8*)0x8012)
#define RESET				(*(u8*)0x87E0)
#define VERSION				(*(u8*)0x87FE)
#define SCRL_PRIO			(*(u8*)0x8030)
#define SPR_X				(*(u8*)0x8020)
#define SPR_Y				(*(u8*)0x8021)
#define SCR_PRIORITY			(*(u8*)0x8030)
#define SCR1_X				(*(u8*)0x8032)
#define SCR1_Y				(*(u8*)0x8033)
#define SCR2_X				(*(u8*)0x8034)
#define SCR2_Y				(*(u8*)0x8035)
#define WIN_X				(*(u8*)0x8002)
#define WIN_Y				(*(u8*)0x8003)
#define WIN_W				(*(u8*)0x8004)
#define WIN_H				(*(u8*)0x8005)
#define REF				(*(u8*)0x8006)
#define BG_COL				(*(u8*)0x8118)
#define RAS_H				(*(u8*)0x8008)
#define RAS_Y				(*(u8*)0x8009)
#define STATUS_2D			(*(u8*)0x8010)
#define CONTROL_2D			(*(u8*)0x8012)
#define BG_PAL				(*(u16*)0x83E0)
#define WIN_PAL				(*(u8*)0x83F0)
#define GE_MODE				(*(u8*)0x87E2)


// memory pointers (as opposed to registers)
#define SPRITE_PALETTE		(u16*)0x8200
#define SCROLL_1_PALETTE	(u16*)0x8280
#define SCROLL_2_PALETTE	(u16*)0x8300
#define SCROLL_PLANE_1		(u16*)0x9000
#define SCROLL_PLANE_2		(u16*)0x9800
#define TILE_RAM		(u16*)0xa000
#define SPRITE_RAM          	(u8*)0x8800
#define SPRITE_COLOUR       	(u8*)0x8c00
#define Z80_RAM			(u8*)0x7000

#define SWI3_INT			(*(Interrupt**)0x6FB8)
#define SWI4_INT			(*(Interrupt**)0x6FBC)
#define SWI5_INT			(*(Interrupt**)0x6FC0)
#define SWI6_INT			(*(Interrupt**)0x6FC4)
#define RTCI_INT			(*(Interrupt**)0x6FC8)
#define VBL_INT				(*(Interrupt**)0x6FCC)
#define Z80_INT				(*(Interrupt**)0x6FD0)
#define HBL_INT				(*(Interrupt**)0x6FD4)
#define TI0_INT				(*(Interrupt**)0x6FD4)
#define TI1_INT				(*(Interrupt**)0x6FD8)
#define TI2_INT				(*(Interrupt**)0x6FDC)
#define TI3_INT				(*(Interrupt**)0x6FE0)
#define STX_INT				(*(Interrupt**)0x6FE4)
#define SRX_INT				(*(Interrupt**)0x6FE8)
#define DMA0_INT			(*(Interrupt**)0x6FF0)
#define DMA1_INT			(*(Interrupt**)0x6FF4)
#define DMA2_INT			(*(Interrupt**)0x6FF8)
#define DMA3_INT			(*(Interrupt**)0x6FFC)

#define SCRN_W				160
#define SCRN_H				152
#define SCRN_TX				20
#define SCRN_TY				19

#define WATCHDOG_CLEAR			0x4E

#define J_UP				0x01
#define J_DOWN				0x02
#define J_LEFT				0x04
#define J_RIGHT				0x08
#define J_A				0x10
#define J_B				0x20
#define J_OPTION			0x40
#define J_POWER				0x80

#define VECT_SHUTDOWN			0			// Shutdown (Power OFF)
#define VECT_CLOCKGEARSET		1			// CPU operation clock setting
#define VECT_INTLVSET						// Interrupt level setting
#define VECT_RTCGET			2			// Real time clock - obtain time
#define VECT_ALARMSET						// Real time clock - alarm setting during	game operation
#define VECT_ALARMDOWNSET					// Real time clock - unit	start up alarm setting
#define VECT_SYSFONTSET			5			// System	font setting
#define VECT_FLASHWRITE						// Flash memory -	data write
#define VECT_FLASHALLERS					// Flash memory -	erase all blocks
#define VECT_FLASHERS						// Flash memory -	erase specified	blocks
#define VECT_FLASHPROTECT					// Flash memory -	protect	specified blocks
#define VECT_GEMODESET						// Color LCD color mode setting (color version only)
#define VECT_COMINIT						// Initialize	serial communication BIOS
#define VECT_COMSENDSTART					// Communication start send BIOS
#define VECT_COMRECIVESTART					// Communication start reception BIOS
#define VECT_COMCREATEDATA					// Communication create send data	BIOS
#define VECT_COMGETDATA						// Communication obtain reception	data BIOS
#define VECT_COMONRTS						// Communication allow RTS signal	send BIOS
#define VECT_COMOFFRTS						// Communication prohibit	RTS	signal send	BIOS
#define VECT_COMSENDSTATUS					// Communication obtain send status BIOS
#define VECT_COMRECIVESTATUS					// Communication obtain reception	status BIOS
#define VECT_COMCREATEBUFDATA					// Communication obtain create data buffer BIOS
#define VECT_COMGETBUFDATA					// Communication write reception obtaining buffer	BIOS

#define ENABLE_INTERRUPTS __asm("ei")


#endif	// _NGPC_H

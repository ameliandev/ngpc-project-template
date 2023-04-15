#include "core/rom/flash.h"

/*
 * Los bloques de memoria para guardar los datos del juego en el cartucho de la Memoria, son diferentes
 * Varían según la cantidad de Mb de cartucho, que oscila entre 4, 8, 16 y 32 (aunque este último no se encuentra en la documentación técnica)
 * Para los cartuchos de 16Mb, se usa el Offset 0x1FA000 y block_nb 0x21
 * Para los cartuchos de 32Mb, se usa el Offset 0x3FA000 y block_nb 0x44
*/
u32 gamedata[256];

//#define	MAGIC_NB	0xcafebabe	// java rules ! -> Ya existe este define en library.c

void SetGameData();

/**
 * Init the Save Game environtment and erase all previus data
 */
void InitFlash(void) {
	SetGameData();
}

u8 existsGameData()
{
	//16Mb:
	u32 *ptr = (u32*)(0x200000+0x1FA000); //Obtiene la posición exacta en memoria donde deben estar los datos. (Cartucho de 16Mb)
	//32Mb:
	//u32 *ptr = (u32*)(0x200000+0x3FA000);

	return *ptr == MAGIC_NB ? 1 : 0;
}

void SaveGame(void *gamedata)
{
	//16Mb
	__ASM("SAVEOFFSET	EQU	0x1FA000");
	__ASM("BLOCK_NB		EQU	0x21");

	//32Mb
	//__ASM("SAVEOFFSET	EQU	0x3FA000");
	//__ASM("BLOCK_NB		EQU	0x44");
	
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
	__ASM("	ld	rbc3,1");		// 256 bytes
	__ASM("	ld	xhl,(xsp+4)"); 	//Aquí se obtiene *data como parámetro (no me digas como..) y se pasa a la memoria
	__ASM("	ld	xhl3,xhl"); 
	__ASM("	ld	xde3,SAVEOFFSET");
	__ASM("	ld	rw3,VECT_FLASHWRITE");
	__ASM("	ld	(rWDCR),WD_CLR");
	__ASM("	swi	1");

	__ASM("	ld	(rWDCR),WD_CLR");
}

void LoadGame()
{
	u8 i;
	//16Mb:
	u32 *ptr = (u32*)(0x200000+0x1FA000); //Obtiene la posición exacta en memoria donde deben estar los datos. (Cartucho de 16Mb)
	//32Mb:
	//u32 *ptr = (u32*)(0x200000+0x3FA000);
	
	if(existsGameData() == 1) {
		for (i=0;i<64;i++) {
			gamedata[i] = ptr[i];
		}
	}
}

/**
 * * Éste método siempre debe ejecutarse al cargar el juego por primera vez, si se desea usar el sistema de 'guardado'
 */
void SetGameData() 
{
	u8 i;
	gamedata[0] = MAGIC_NB;
	
	for (i = 1; i < 64; i++) {
		gamedata[i] = 0;
	}
}


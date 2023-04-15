#include "ngpc.h"
#include "library.h"

extern u32 gamedata[256];

void InitFlash(void);
void SaveGame(void *data);
void LoadGame();
//void SetGameData();
u8 existsGameData();

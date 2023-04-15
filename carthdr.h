/**********************************************************
 *
 * Standard NGPC Cartridge Header
 *
 **********************************************************/
extern void main(void); // used in main.c for cartridge header


const char Licensed[28] = " LICENSED BY SNK CORPORATION";
const FuncPtr ptr = main;
const short CartID = 0;
// System is actually 00 for CartVer and x10 for System but I
// couldn't work out how to set the alignment to 1 for consts
const short System = 0x1000; 

// You game name should go in CartTitle. It must be 12 characters
// long so pad with spaces
const char CartTitle[12] = "FRAMEWORK   ";
const long Reserved[4] = {0,0,0,0};

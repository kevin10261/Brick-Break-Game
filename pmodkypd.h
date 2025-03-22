#ifndef PMODKYPD_H
#define PMODKYPD_H

/****************************** Include Files ***************************/

#include "xil_io.h"
#include "xstatus.h"
#include "xil_types.h"

/**************************** Type Definitions **************************/

typedef struct PmodKYPD {
   u32 GPIO_addr;
   u8  keytable[16];
   u32 keytable_loaded;
} PmodKYPD;

#define KYPD_NO_KEY     0
#define KYPD_SINGLE_KEY 1
#define KYPD_MULTI_KEY  2

/************************** Function Definitions ************************/

void KYPD_begin(PmodKYPD *InstancePtr, u32 GPIO_Address);
void KYPD_loadKeyTable(PmodKYPD *InstancePtr, u8 keytable[16]);
void KYPD_setCols(PmodKYPD *InstancePtr, u32 cols);
u32 KYPD_getRows(PmodKYPD *InstancePtr);
u16 KYPD_getKeyStates(PmodKYPD *InstancePtr);
u32 KYPD_getKeyPressed(PmodKYPD *InstancePtr, u16 keystate, u8 *cptr);

#endif // PmodKYPD_H

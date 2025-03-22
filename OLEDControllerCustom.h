#ifndef OLEDCONTROLLERCUSTOM_H
#define OLEDCONTROLLERCUSTOM_H

#include "PmodOLED.h"

// Function prototypes
void OLED_DrawLineTo(PmodOLED *InstancePtr, int xco, int yco);
void OLED_RectangleTo(PmodOLED *InstancePtr, int xco, int yco);
int grphClampXco(int xco);
int grphClampYco(int yco);
int grphAbs(int foo);

#endif // OLEDCONTROLLERCUSTOM_H

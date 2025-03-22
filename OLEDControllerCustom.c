#include "OLEDControllerCustom.h"


void OLED_DrawLineTo(PmodOLED *InstancePtr, int xco, int yco)
{
    int    err;
    int    del;
    int    lim;
    int    cpx;
    int    dxco;
    int    dyco;
    OLED  *OledPtr = &(InstancePtr->OLEDState);

    /* Clamp the point to be on the display.
    */
    xco = grphClampXco(xco);
    yco = grphClampYco(yco);

    /* Determine which octant the line occupies
    */
    dxco = xco - OledPtr->xcoOledCur;
    dyco = yco - OledPtr->ycoOledCur;
    if (grphAbs(dxco) >= grphAbs(dyco)){
        /* Line is x-major
        */
        lim = grphAbs(dxco);
        del = grphAbs(dyco);
    } else {
        /* Line is y-major
        */
        lim = grphAbs(dyco);
        del = grphAbs(dxco);
	}

    /* Render the line. The algorithm is:
    **      Write the current pixel
    **      Move one pixel on the major axis
    **      Add the minor axis delta to the error accumulator
    **      if the error accumulator is greater than the major axis delta
    **          Move one pixel in the minor axis
    **          Subtract major axis delta from error accumulator
    */
    err = lim/2;
    cpx = lim;
    while (cpx > 0) {
    	OLED_MoveTo(InstancePtr, OledPtr->xcoOledCur, OledPtr->ycoOledCur);
        OLED_DrawPixel(InstancePtr);
        if (grphAbs(dxco) >= grphAbs(dyco)){
                /* Line is x-major
                */
        	if(OledPtr->xcoOledCur < xco){
        		OledPtr->xcoOledCur += 1;
        	} else {
        		OledPtr->xcoOledCur -= 1;
        	}

        } else {
                /* Line is y-major
                */
        	if(OledPtr->ycoOledCur < yco){
        		OledPtr->ycoOledCur += 1;
        	} else {
        		OledPtr->ycoOledCur -= 1;
        	}
        }
        err += del;
        if (err > lim){
            err -= lim;
            if (grphAbs(dxco) >= grphAbs(dyco)){
				/* Line is x-major
				*/
            	if(OledPtr->ycoOledCur > yco){
            		OledPtr->ycoOledCur -= 1;
            	} else {
            		OledPtr->ycoOledCur += 1;
            	}

			} else {
				/* Line is y-major
				*/
				if(OledPtr->xcoOledCur < xco){
					OledPtr->xcoOledCur += 1;
				} else {
            		OledPtr->xcoOledCur -= 1;
            	}
			}
        }
        cpx -= 1;
    }
 }


void OLED_RectangleTo(PmodOLED *InstancePtr, int xco, int yco)
{
    OLED *OledPtr = &(InstancePtr->OLEDState);
    int     xco1;
    int     yco1;

    /* Clamp the point to be on the display.
    */
    xco = grphClampXco(xco);
    yco = grphClampYco(yco);

    xco1 = OledPtr->xcoOledCur;
    yco1 = OledPtr->ycoOledCur;
    OLED_DrawLineTo(InstancePtr, xco, yco1);
    OLED_DrawLineTo(InstancePtr, xco, yco);
    OLED_DrawLineTo(InstancePtr, xco1, yco);
    OLED_DrawLineTo(InstancePtr, xco1, yco1);
}


int grphAbs(int foo)
{
    return (foo < 0) ? (foo * -1) : (foo);
}


int grphClampXco(int xco)
{
    if (xco < 0) {
        xco = 0;
    }
    if (xco >= ccolOledMax) {
        xco = ccolOledMax-1;
    }

    return xco;
}


int grphClampYco(int yco)
{
    if (yco < 0) {
        yco = 0;
    }
    if (yco >= crowOledMax) {
        yco = crowOledMax-1;
    }

    return yco;
}

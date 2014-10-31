#include <stdio.h>
#include "LPC17xx.H"                         // LPC17xx definitions          
#include "GLCD.h"

//This function draws a rectangle with a width and height, at an x location and a y location on the LCD
void Draw_Rect( int x0, int y0, int width, int height)
{
	int i;
	int j;
for(i=0; i<height; i++)
{ for(j=0; j<width; j++)
	{ GLCD_PutPixel(x0+j,y0+i);
	}
  }
}


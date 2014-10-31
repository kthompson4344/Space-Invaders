#include <stdio.h>
#include "LPC17xx.H"                         /* LPC17xx definitions           */
#include "GLCD_SPI.h"
#include "SI.h"
//LCD is 320x240

//Moving is done by drawing a one pixel wide rectangle in front of the direction
//of the object that is moving, in the objects color. Then a one pixel wide rectangle is drawn behind the object
//in black.  This ensures that the least amount of pixels possible are drawn when moving.  For the aliens, this
//would be done for the body, both eyes, and the mouth.

// This function draws the Laser Cannon at a specified x and y location
void Draw_Cannon(int x0, int y0)
{
    GLCD_SetTextColor(Green);
    Draw_Rect(x0,y0,30,10);
    Draw_Rect(x0+10,y0-10,10,10);
}

//This function draws an Alien at a specified x and y position
void Draw_Alien(int x0, int y0)
{
    GLCD_SetTextColor(White);
    Draw_Rect(x0,y0,20,20);
    GLCD_SetTextColor(Black);
    Draw_Rect(x0+3,y0+12,14,5);//mouth
    Draw_Rect(x0+3,y0+4,5,5);//left eye
    Draw_Rect(x0+12,y0+4,5,5);//right eye
}

//This function draws the Alien Ship at a specified x position
void Draw_AlienShip(int x0)
{
    int y0 = 50;
    GLCD_SetTextColor(Red);
    Draw_Rect(x0,y0,30,10);
	  Draw_Rect(x0+3,y0-5,24,5);
	  Draw_Rect(x0+6,y0-10,18,5);
}
//This function moves the Alien Ship to the left
void Move_AlienShip_Left(void)
{
	int moveAmount=4;//sets how many pixels the ship moves
	Alienship_Pos -= moveAmount;
    GLCD_SetTextColor(Black);
	Draw_Rect(Alienship_Pos+30,shipvert,moveAmount,10);
	Draw_Rect(Alienship_Pos+27,shipvert-5,moveAmount,5);
    Draw_Rect(Alienship_Pos+24,shipvert-10,moveAmount,5);
	GLCD_SetTextColor(Red);
    Draw_Rect(Alienship_Pos,shipvert,moveAmount,10);
    Draw_Rect(Alienship_Pos+3,shipvert-5,moveAmount,5);
    Draw_Rect(Alienship_Pos+6,shipvert-10,moveAmount,5);
}

//This function moves the cannon left
void Move_Cannon_Left(void)
{
	int moveAmount=1;//specifies how many pixels the cannon moves
	Cannon_Pos -= moveAmount;
  GLCD_SetTextColor(Black);
	Draw_Rect(Cannon_Pos+30,Cannon_Vert,moveAmount,10);
	Draw_Rect(Cannon_Pos+20,Cannon_Vert-10,moveAmount,10);
	GLCD_SetTextColor(Green);
	Draw_Rect(Cannon_Pos,Cannon_Vert,moveAmount,10);
	Draw_Rect(Cannon_Pos+10,Cannon_Vert-10,moveAmount,10);
}
//This function moves the cannon right
void Move_Cannon_Right(void)
{
	int moveAmount=1;//specifies how many pixels the cannon moves
	Cannon_Pos += moveAmount;
	GLCD_SetTextColor(Black);
	Draw_Rect(Cannon_Pos-moveAmount,Cannon_Vert,moveAmount,10);
	Draw_Rect(Cannon_Pos+10-moveAmount,Cannon_Vert-10,moveAmount,10);
	GLCD_SetTextColor(Green);
	Draw_Rect(Cannon_Pos+30-moveAmount,Cannon_Vert,moveAmount,10);
	Draw_Rect(Cannon_Pos+20-moveAmount,Cannon_Vert-10,moveAmount,10);
}
//This function moves all the aliens left
void Move_Aliens_Left(void)
{
	int i;
	int column=0;
	int row=0;
	int moveAmount=1;//specifies how many pixels the aliens move
	Alien_xPos-=moveAmount;
	for (row=0; row<3; row++) {
	for (column=0; column<6; column++) {
	if (AliensAlive[row][column]==1) {
		
	//draws black on the right side of body,mouth,left eye,right eye
	GLCD_SetTextColor(Black);
	Draw_Rect(Alien_Start_Horiz+(column*40)+20+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40,moveAmount,20);//body
	Draw_Rect(Alien_Start_Horiz+(column*40)+3+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+12,moveAmount,5);//mouth
	Draw_Rect(Alien_Start_Horiz+(column*40)+3+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//left eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+12+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//right eye
		
	GLCD_SetTextColor(White);
	//draws white on the left side of body,mouth,left eye,right eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40,moveAmount,20);//body
	Draw_Rect(Alien_Start_Horiz+(column*40)+17+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+12,moveAmount,5);//mouth
	Draw_Rect(Alien_Start_Horiz+(column*40)+8+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//left eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+17+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//right eye
	}
    else if (AliensAlive[row][column]==2) {
			  //covers black over a dead alien
        GLCD_SetTextColor(Black);
        Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos-2,Alien_Start_Vert+(row*40)+Alien_yPos-40,24,26);//body
		    AliensAlive[row][column]=0;
			  GLCD_SetBackColor(Black);
		    GLCD_SetTextColor(White);
		    GLCD_DisplayString(9, 6, 1, score1);
			  delay(100000);//ensures that alien is deleted //100000 seems to work

    }
	else {
	}
	}
}
}
//This function moves all the aliens right
void Move_Aliens_Right(void)
{
	int column=0;
	int row=0;
	int moveAmount=1;
	Alien_xPos+=moveAmount;
	for (row=0; row<3; row++) {
	for (column=0; column<6; column++) {
  if (AliensAlive[row][column]==1) {
		
	GLCD_SetTextColor(White);
	//draws white on the right side of body,mouth,left eye,right eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+19+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40,moveAmount,20);//body
	Draw_Rect(Alien_Start_Horiz+(column*40)+2+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+12,moveAmount,5);//mouth
	Draw_Rect(Alien_Start_Horiz+(column*40)+2+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//left eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+11+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//right eye
		
	GLCD_SetTextColor(Black);
	//draws black on the left side of body,mouth,left eye,right eye
	Draw_Rect(Alien_Start_Horiz+(column*40)-1+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40,moveAmount,20);//body
	Draw_Rect(Alien_Start_Horiz+(column*40)+16+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+12,moveAmount,5);//mouth
	Draw_Rect(Alien_Start_Horiz+(column*40)+7+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//left eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+16+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40+4,moveAmount,5);//right eye
	}
        else if (AliensAlive[row][column]==2) {
				//covers black over a dead alien
        GLCD_SetTextColor(Black);
        Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos-2,Alien_Start_Vert+(row*40)+Alien_yPos-40,24,26);//body
			  AliensAlive[row][column]=0;
			  GLCD_SetBackColor(Black);
		    GLCD_SetTextColor(White);
		    GLCD_DisplayString(9, 6, 1, score1);
				delay(100000);//ensures that alien is deleted
    }
	else {
	}
}
}
}
//This function moves all the aliens down
void Move_Aliens_Down(void) {
	int column=0;
	int row=0;
	int moveAmount=4;//specifies how many pixels down the aliens move
	Alien_yPos+=moveAmount;
	for (row=0; row<3; row++) {
	for (column=0; column<6; column++) {
  if (AliensAlive[row][column]==1) {
		
	GLCD_SetTextColor(White);
	//draws white at the bottom of the alien
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-20-moveAmount,20,moveAmount);//body
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos+3,Alien_Start_Vert+(row*40)+Alien_yPos-28-moveAmount,14,moveAmount);//mouth
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos+3,Alien_Start_Vert+(row*40)+Alien_yPos-36-moveAmount,5,moveAmount);//left eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos+12,Alien_Start_Vert+(row*40)+Alien_yPos-36-moveAmount,5,moveAmount);//right eye
	
  //draws black at the top of the alien		
 	GLCD_SetTextColor(Black);
 	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos,Alien_Start_Vert+(row*40)+Alien_yPos-40-moveAmount,20,moveAmount);//body
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos+3,Alien_Start_Vert+(row*40)+Alien_yPos-23-moveAmount,14,moveAmount);//mouth
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos+3,Alien_Start_Vert+(row*40)+Alien_yPos-31-moveAmount,5,moveAmount);//left eye
	Draw_Rect(Alien_Start_Horiz+(column*40)+Alien_xPos+12,Alien_Start_Vert+(row*40)+Alien_yPos-31-moveAmount,5,moveAmount);//right eye
}
}
}
}

//This function draws the green line below the ship
void Draw_Line(void)
{
	GLCD_SetTextColor(Green);
	Draw_Rect(0,240-28,320,3);
}

//This function draws the cannon's bullet
void Draw_Bullet(void)
{
	Bullet_yPos=179;
	GLCD_SetTextColor(White);
	Bullet_xPos=Cannon_Pos;
	Draw_Rect(Cannon_Pos+14,Bullet_yPos,2,6);
}
//This function draws the alien's bullet at a specified x and y location
void Draw_Alien_Bullet(int row,int column)
{
	Alien_Bullet_yPos=20+(40*row)+Alien_yPos+1;
	GLCD_SetTextColor(White);
	Alien_Bullet_xPos=column*40+Alien_xPos+19;
	Draw_Rect(Alien_Bullet_xPos,Alien_Bullet_yPos,2,6);
}
//This function moves the cannon's bullet up
void Move_Bullet_Up(void)
{
	Bullet_yPos--;
	GLCD_SetTextColor(White);
	Draw_Rect(Bullet_xPos+14,Bullet_yPos,2,1);
	GLCD_SetTextColor(Black);
	Draw_Rect(Bullet_xPos+14,Bullet_yPos+6,2,1);
}
//This function moves the alien's bullet down
void Move_Bullet_Down(void)
{
	Alien_Bullet_yPos++;
	GLCD_SetTextColor(White);
	Draw_Rect(Alien_Bullet_xPos,Alien_Bullet_yPos+5,2,1);
	GLCD_SetTextColor(Black);
	Draw_Rect(Alien_Bullet_xPos,Alien_Bullet_yPos-1,2,1);
}
//This function draws a barrier at a specified x position
void Draw_Barrier(int x0) {
	GLCD_SetTextColor(Green);
	Draw_Rect(x0,Barrier_yPos,30,12);
}
//This function removes different parts of a specific barrier given different numbers
//representing how many times parts of the barrier has already been destroyed 
//The number will be an element of an array BarrierHits[barrier]. Which contains how many sections
//of a specific barrier are left to be destroyed + 1. They start off as 7, then when one is hit moves to six,
//which is where the first part of the barrier is destroyed below.  It places four 4x4 rectangles in black to cover up the green.
void Destroy_Barrier(int barrier,int number) {
	if (number==6) {
		GLCD_SetTextColor(Black);
		Draw_Rect(Barrier_Left+80*barrier,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier+16,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier+12,Barrier_yPos+8,4,4);
		Draw_Rect(Barrier_Left+80*barrier+20,Barrier_yPos+4,4,4);
		number=0;
	}
	else if (number==5) {
		GLCD_SetTextColor(Black);
		Draw_Rect(Barrier_Left+80*barrier+28,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier+8,Barrier_yPos+4,4,4);
		Draw_Rect(Barrier_Left+80*barrier+4,Barrier_yPos+8,4,4);
		Draw_Rect(Barrier_Left+80*barrier+24,Barrier_yPos+8,4,4);
		number=0;
	}
	else if (number==4) {
		GLCD_SetTextColor(Black);
		Draw_Rect(Barrier_Left+80*barrier+8,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier+24,Barrier_yPos+4,4,4);
		Draw_Rect(Barrier_Left+80*barrier+8,Barrier_yPos+8,4,4);
		Draw_Rect(Barrier_Left+80*barrier+28,Barrier_yPos+8,4,4);		
	}
	else if (number==3) {
		GLCD_SetTextColor(Black);
		Draw_Rect(Barrier_Left+80*barrier+24,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier+12,Barrier_yPos+4,4,4);
		Draw_Rect(Barrier_Left+80*barrier,Barrier_yPos+8,4,4);
		Draw_Rect(Barrier_Left+80*barrier+20,Barrier_yPos+8,4,4);
	}
	else if (number==2) {
		GLCD_SetTextColor(Black);
		Draw_Rect(Barrier_Left+80*barrier+20,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier+4,Barrier_yPos+4,4,4);
		Draw_Rect(Barrier_Left+80*barrier+16,Barrier_yPos+4,4,4);
		Draw_Rect(Barrier_Left+80*barrier+28,Barrier_yPos+4,4,4);
	}
	else if (number==1) {
		GLCD_SetTextColor(Black);
		Draw_Rect(Barrier_Left+80*barrier+4,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier+12,Barrier_yPos,4,4);
		Draw_Rect(Barrier_Left+80*barrier,Barrier_yPos+4,4,4);
		Draw_Rect(Barrier_Left+80*barrier+16,Barrier_yPos+8,4,4);	
	}
}
//SPACE INVADERS
//By Kyle Thompson and Joseph Arnet
//May 2014

//This version is to be used with the MCB1700, NOT the Land Tiger in the Lab.
//That is why the GLCD_SPI.h and GLCD_SPI_LPC1700.c are used. 
//Also the GLCD_DisplayString needs a font index and joystick pins are different
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <LPC17xx.H>
#include "lpc17xx.h"
#include "CRIS_UTILS.h"
#include "GLCD_SPI.h"
#include "SI.h"
#include "joystick.h"
#include "ADC.h"
#define __FI   1    /* Font index 16x24  */ //Needed for GLCD_DisplayString on MCB1700
volatile int Cannon_Pos=145;//x position of cannon
int Cannon_Vert=195;//y position of cannon
bool Restart;
int shootcount=0;
bool sound=0;
int shootsound;
char Check_Aliens;
volatile int Bullet_yPos;
volatile int Bullet_xPos;
volatile int Alien_Bullet_yPos=246;
volatile int Alien_Bullet_xPos=0;
int Alienship_Pos;
int Alien_Start_Horiz=10;
volatile int Alien_xPos=0;
int Alien_Start_Vert=40;
volatile int Alien_yPos=0;
char moveCannon = 0;
char moveLeft=0;
bool Fire=0;
bool AlienFire=0;
char AliensAlive[3][6];
bool Delete=0;
int Aliensx;
int Aliensy;
int xm;
int ym;
int r;
char score=0;
bool Draw_Black_Bullet=0;
unsigned char score1[5];
char Alien_Bullet_Stop=204;
int AlienLeft;
int AlienRight;
char Alien_Bullet_yPos_Old;
volatile bool Play;
bool Draw_Ship=1;
char shipvert=20;
bool playshoot=0;
bool playcannonexplode=0;
bool playalienexplode=0;
int Barrier_yPos=165;
char BarrierHits[4];
bool Remove_Barrier=0;
char Barrier;
char Barrier_Left=25;
int main() {
    int i;
	  int Start=0;
	  int j;
	  int a;
	  int xm;
	  int ym;
	  int x0;
	  int y0;
	  int x1;
	  int y1;
	  int val;
      int AlienRow;
	  int AlienRow2;
      unsigned char *s1 = (unsigned char *)"Score: ";
	  unsigned char *s2 = (unsigned char *)"Lives: ";
	  unsigned char lives1[1];
	  int lives;
	  int Alien_Bullet_Left=0;
	  int Alien_Bullet_Right=0;
	  int Bottom_Aliens;
      //**********Things Needed For Audio
	  SystemInit(); 
      LPC_PINCON->PINSEL1 &=~((0x03<<18)|(0x03<<20));  
      /* P0.25, A0.0, function 01, P0.26 AOUT, function 10 */
      LPC_PINCON->PINSEL1 |= ((0x01<<18)|(0x02<<20));
        //************************************
	  ADC_init(); 
	  GLCD_Init();
      JOYSTICK_Init ();
		while(1) {
		for(i=0;i<=5;i++) {
			for(a=0;a<=2;a++) {
				AliensAlive[a][i]=1;
			}
		}
		for(i=0; i<=3; i++)
		BarrierHits[i]=7;
		lives=3;
		Restart=1;
		Bottom_Aliens=140;
		Start=0;
		GLCD_Clear(Black);
		GLCD_SetBackColor(Black);
		GLCD_SetTextColor(White);
		GLCD_DisplayString(4, 4, __FI, "Press Center");
		GLCD_DisplayString(5, 1, __FI, "Joystick to Begin");
		while (Start==0) {
			LPC_DAC->DACR = 0x00008000;		/* DAC Output set to Middle Point */
		if (JOYSTICK_GetKeys ()==JOYSTICK_CENTER) 
{
			Start=1;
			 }
		 }
		while (Restart==1) {//Restart is used to control the entire game with 3 lives
	  ADC_startCnv();                                 /* start A/D conversion */
      val = ((ADC_getCnv() >> 4) & 0xFF);             /* use upper 8 bits of 12 bit AD conversion */
      ADC_stopCnv();                                  /* stop A/D conversion */
			srand(val);//Use Noise from ADC to select seed of psuedo random number from rand() 
Bottom_Aliens=114 ;
Cannon_Pos=145;
Cannon_Vert=200;
Alien_Start_Horiz=10;
Alien_xPos=0;
Alien_Start_Vert=40;
Alien_yPos=0;
moveCannon = 0;
Fire=0;
AlienFire=0;
Delete=0;
AlienLeft=10;
AlienRight=310;
Alien_Bullet_yPos=246;
Alien_Bullet_xPos=0;
Alienship_Pos=320;
moveLeft=0;			
Bullet_yPos=0;
		GLCD_Clear(Black);
		Alien_yPos=Alien_Start_Vert;
    Draw_Line();
    Draw_Cannon(145,Cannon_Vert);
					for(i=0;i<=5;i++) {
			for(a=0;a<=2;a++) {
				if (AliensAlive[a][i]==1)
					Draw_Alien(i*40+Alien_Start_Horiz,a*40+Alien_Start_Vert);
			}
		}
			//************************************************************************

//************SET UP TIMERS*****************************************************		
	    /*The LPC1768 SOC has 4 timers. Each timer consists of match registers (called MR0, MR1, ...),
      a Match Control Register (MCR), and a Timer Control Register (TCR). The timers are 
      controlled through structs called LPC_TIM0, LPC_TIM1,... which contain the registers
      listed above.
      To enable a timer, first the timer must be connected to power and connected to a clock
      source. Please see http://www.nxp.com/documents/user_manual/UM10360.pdf for info on this.
      (Page 57 and 64 in particular)
      To power the timers, we use the LPC_SC->PCOMP register. Bit 1 is for timer 1, bit 2
      is for timer 2, bit 22 is for timer 3, and bit 23 is for timer 4. To connet the clocks,
      we use the PCLKSEL0 and PCLKSEL1 registers. 
      After this, we place match values in the MR0 registers for each timer. This causes
      the timers to interrupt the processor when they count up to the match values. 
      You should reduce this value to speed up the timer, and increase it to slow it down.
      The match control register has 2 bits of interst. Bit 0 is set to cause a match
      to generate an interrupt. Bit 1 is set to cause the timer to reset when a match occurs.
      The Timer Control register - bit 0 is the timer enable, and bit 1 is the timer reset. 
    */
    //Set up timer 0 //Aliens
    LPC_SC->PCONP |= 1 << 1; // Power up Timer 0 
    LPC_SC->PCLKSEL0 |= 1 << 2; // Clock for timer = CCLK, i.e., CPU Clock 
	  LPC_TIM0->MR0 = 1 << 24; // Give a value suitable for the LED blinking 
                             // frequency based on the clock frequency 
    LPC_TIM0->MCR |= 1 << 0; // Interrupt on Match 0 compare 
	  LPC_TIM0->MCR |= 1 << 1; // Reset timer on Match 0 
	  LPC_TIM0->TCR |= 1 << 1; // Manually Reset Timer 0 (forced); 
    LPC_TIM0->TCR &= ~(1 << 1); // Stop resetting the timer 
	  //NVIC_SET_PRIORITY(TIMER0_IRQn, 2);
    NVIC_EnableIRQ(TIMER0_IRQn); // enable timer0 interrupt source
    LPC_TIM0->TCR |= 1 << 0; // Start timer
    
    //Set up timer 1 
    LPC_SC->PCONP |= 1 << 2; // Power up Timer 1
    LPC_SC->PCLKSEL0 |= 1 << 4; // Clock for timer = CCLK, i.e., CPU Clock 
	  LPC_TIM1->MR0 = 1 << 18;  // Give a value suitable for the LED blinking 
                             // frequency based on the clock frequency 
    LPC_TIM1->MCR |= 1 << 0; // Interrupt on Match 1 compare 
	  LPC_TIM1->MCR |= 1 << 1; // Reset timer on Match 0 
	  LPC_TIM1->TCR |= 1 << 1; // Manually Reset Timer 1 (forced); 
    LPC_TIM1->TCR &= ~(1 << 1); // Stop resetting the timer 
	  //NVIC_SET_PRIORITY(TIMER1_IRQn, 2);
    NVIC_EnableIRQ(TIMER1_IRQn); // enable timer1 interrupt source
    LPC_TIM1->TCR |= 1 << 0; // Start timer
    
    //Set up timer 2-Used for
    LPC_SC->PCONP |= 1 << 22; // Power up Timer 2
    LPC_SC->PCLKSEL1 |= 1 << 12; // Clock for timer = CCLK, i.e., CPU Clock 
	  LPC_TIM2->MR0 = 1 << 28; // Speed of timer = 28
                             // frequency based on the clock frequency 
    LPC_TIM2->MCR |= 1 << 0; // Interrupt on Match 1 compare 
	  LPC_TIM2->MCR |= 1 << 1; // Reset timer on Match 0 
	  LPC_TIM2->TCR |= 1 << 1; // Manually Reset Timer 2 (forced); 
    LPC_TIM2->TCR &= ~(1 << 1); // Stop resetting the timer 
		//NVIC_SET_PRIORITY(TIMER2_IRQn, 2);
    NVIC_EnableIRQ(TIMER2_IRQn); // enable timer2 interrupt source
    LPC_TIM2->TCR |= 1 << 0; // Start timer
		
				//**************************************************************************************
//**********DISPLAY INITIAL SCORE AND LIVES*************************************************		
GLCD_SetBackColor(Black);
GLCD_SetTextColor(White);
sprintf((char *)score1, "%d", score);
//sprintf((char *)lives1, "%d", lives);
GLCD_DisplayString(9, 0, __FI, s1);
//GLCD_DisplayString(9, 12, __FI, s2);
GLCD_DisplayString(9, 6, __FI, score1);
//GLCD_DisplayString(9, 18, __FI, lives1);
Draw_Cannon(284,228);

//************Cannons For Lives**************
if (lives==3) {
	Draw_Cannon(284,228);
	Draw_Cannon(250,228);
	Draw_Cannon(216,228);
}
else if (lives==2) {
	Draw_Cannon(284,228);
	Draw_Cannon(250,228);
}
else {
	Draw_Cannon(284,228);
}
		//********************************
//Draw Barriers at beginning of game
Draw_Barrier(Barrier_Left);
Draw_Barrier(Barrier_Left+80);
Draw_Barrier(Barrier_Left+160);
Draw_Barrier(Barrier_Left+240);

				//**************************************************************************************
			
//*************JOYSTICK*********************************************************************
     Play=1;
	    while(Play==1) {//Play controls each life
            
			if (JOYSTICK_GetKeys ()==JOYSTICK_LEFT) {//Checks to see if joystick left is pressed
				moveCannon = 1;//Controls movement in timer
			 }
			  if (JOYSTICK_GetKeys ()==JOYSTICK_RIGHT) {//Checks to see if joystick right is pressed
				moveCannon = 2;//Controls movement in timer
			 }
			 //Cannon Bullet Fire
			 if ((JOYSTICK_GetKeys () == JOYSTICK_CENTER) & (Bullet_yPos <= 0)) {//checks to see if joystick center is pressed, can only fire if bullet is not on screen
				 Draw_Bullet();//Draws bullet
				 playshoot = 1;//Triggers sound effect
				 Fire = 1;//Controls movement up in timer
				 delay(100000);//Delays to ensure that more than one bullet is not fired
			 }
			 //*****************************************
			 
			 
//************CALCS HOW FAR ALIENS MOVE LEFT AND RIGHT***********
        //checks every column of aliens horizontally to determine which column is the left most column of at least 1 alien, so that aliens will move all the way to the left of the screen
		for(Aliensx=0; Aliensx<=5; Aliensx++) {
			AlienRow=AliensAlive[0][Aliensx]+AliensAlive[1][Aliensx]+AliensAlive[2][Aliensx];
			 if (AlienRow > 0) {
				 AlienLeft=Alien_Start_Horiz+Aliensx*40;	
				 break;
			 }
		 }
		//checks every column of aliens horizontally to determine which column is the right most column of at least 1 alien, so that aliens will move all the way to the right of the screen
		for(Aliensx=5; Aliensx>=0; Aliensx--) {
			AlienRow=AliensAlive[0][Aliensx]+AliensAlive[1][Aliensx]+AliensAlive[2][Aliensx];
			 if ((AlienRow) > 0) {
				 AlienRight=320-Alien_Start_Horiz+(5-Aliensx)*40;
				 break;
			 }
		 }
             //Checks if aliens are at the left of the screen
			 if (Alien_Start_Horiz-Alien_xPos == AlienLeft & Alien_yPos>40) {
				 moveLeft=2;//Controls movement in timer (moves aliens down then to the right when they reach the left)
			 }
             //Checks if aliens are at the right of the screen
			 if (Alien_xPos + 240 - Alien_Start_Horiz == AlienRight) {
				 moveLeft=3;//Controls movement in timer (moves aliens down then to the left when they reach the right)
			 }
			 //*****************************************************************
			 
			 
//*******CANNON BULLET***********
			 if (Bullet_yPos>0)//if bullet is on the screen, keep moving it up
				 Fire=1;//Controls movement upward in timer
			 else 
				 Fire=0;//Stops upward movement in timer
			 //*****************		 
			 
//*******ALIEN HIT DETECTION********************
		 //xm,ym is top left edge of leading alien
		 //x0,y0 is (x,y) of top left of bullet
		 //x1,y1 is (x,y) of top right of bullet
			 xm=Alien_xPos;
			 ym=Alien_yPos;
			 x0=Bullet_xPos+4;
			 y0=Bullet_yPos;
			 x1=x0+2;
			 y1=y0;
			 //Checks if the bullet is within one of the three rows, if so checks each column to see if it is at the column an alien is in, if so then it kills the alien
			 for(Aliensx=0; Aliensx<=5; Aliensx++) {
				 if((x0>(xm+40*Aliensx) & x0<(xm+40*Aliensx)+20) | (x1>(xm+40*Aliensx) & x1<(xm+40*Aliensx)+20)) {
					 for(Aliensy=0; Aliensy<=2; Aliensy++) {
						 if(AliensAlive[Aliensy][Aliensx]) {
							 if((y0>(ym+40*Aliensy) & y0<(ym+40*Aliensy)+20) | (y1>(ym+40*Aliensy) & y1<(ym+40*Aliensy)+20)) {
							   AliensAlive[Aliensy][Aliensx]=2;//Inside Alien_Move_left/Right functions so that the drawing of black over the alien is within an interrupt
							   Bullet_yPos=0;//resets bullet position
							   Fire=0;//stops bullet
							   playalienexplode=1;//Plays sound effect	
                             
                               //UPDATE SCORE
                               score+=20;
                               sprintf((char *)score1, "%d", score);
                               GLCD_SetBackColor(Black);
                               GLCD_SetTextColor(White);
                               //Actual displaying is done in the move aliens functions
						 }
					 }
				 }
			 }
		 }//END HIT DETECTION
		 
//************BOTTOM OF ALIEN DETECTION********************
         //When the bottom of the aliens reaches the cannon it is destroyed
         
         //Checks to see what is the lowest row with at least one alien in it
		 for(Aliensy=0; Aliensy<=2; Aliensy++) {
		 AlienRow2=0;
		 for(Aliensx=0; Aliensx<=5; Aliensx++) {
			AlienRow2+=AliensAlive[Aliensy][Aliensx];
		 }
		 //The above for loop checks from top down, so the first row that all aliens are dead in is the lowest (This could be done better because you could have 
         //an empty row above a row with aliens, but that is very unlikely, so this method is good enough)		 
		 if (AlienRow2==0) {
			 Bottom_Aliens=(Aliensy*40);
			 break;
	     }
     }
         //Checks if the Bottom of the aliens have reached the top of the ship
		 if (Alien_yPos+Bottom_Aliens-6>=Cannon_Vert) {
               playcannonexplode=1;//Play Sound Effect
			   Play=0;//End level
			   lives--;//Reduce life
		 }
		 //*****************************************************
		 
//*************ALIEN FIRE*****************************************
             //If Alien bullet is in on the screen, keep moving it down
		 	 if (Alien_Bullet_yPos<=Alien_Bullet_Stop-1 & Remove_Barrier==0) {
				 AlienFire=1;//Controls movement downward in timer
			 }
			 else 
				 AlienFire=0;//Stops movement downward in timer
				//************************************************	

			 
//***********CANNON HIT DETECTION*******************************
		 //variables for bottom left and right edge of alien bullet
		 Alien_Bullet_Left=Alien_Bullet_xPos;
		 Alien_Bullet_Right=Alien_Bullet_Left+2;
         //Checks left and right edges of ship against bullet
		 if ((Alien_Bullet_Left >= Cannon_Pos) & (Alien_Bullet_Right <= Cannon_Pos + 30) & (Alien_Bullet_yPos >= Cannon_Vert) & (Alien_Bullet_yPos < Alien_Bullet_Stop)) {
			 playcannonexplode=1;//play sound effect
			 delay(10000);//Delay to ensure that sound plays/no weird glitching
			 Play=0;//End level
			 lives--;//reduce life
		 }
				//******************************************************
		 
//***********ALIEN SHIP HIT DETECTION***************************
		 //x0,y0 is (x,y) of top left of bullet
		 //x1,y1 is (x,y) of top right of bullet
         //Checks if bullet is within left and right of alien ship
			if (x0 <= Alienship_Pos+26 & x1 >= Alienship_Pos-4 & Bullet_yPos <= shipvert+10 & Fire==1) {
				score+=50;//Increase Score
				sprintf((char *)score1, "%d", score);
				Draw_Ship=0;//Stops ship from moving
                Fire=0;//Stops Bullet
				Bullet_yPos=0;//Resets Bullet Position	
			}


				//******************************************************
			
//***********Barrier Hit Detection****************************
            //If Alien Bullet is within the height of the barriers
			if (Alien_Bullet_yPos+6>=Barrier_yPos & Alien_Bullet_yPos+6<=Barrier_yPos+12) {
                //Checks each barrier to see if the bullet is within the left and right edges
				for (Barrier=0; Barrier<=3; Barrier++) {
					if ((Alien_Bullet_Left >= Barrier_Left+(Barrier*80) & Alien_Bullet_Right <= Barrier_Left+(Barrier*80)+30) & (BarrierHits[Barrier]>1)) {
					  Remove_Barrier=1;//Controls removal of barriers located in timers
					  AlienFire=0;//Stops Alien Bullet
					  Alien_Bullet_yPos=246;//Resets Alien Bullet Position
					  break;
					}
				}
			}
				
			
			
			//***********************************************************
			
//************Checks if all the aliens are dead**************
            Check_Aliens=0;
			for(Aliensx=0; Aliensx<=5; Aliensx++) {
				for(Aliensy=0; Aliensy<=2; Aliensy++) {
					Check_Aliens+=AliensAlive[Aliensy][Aliensx];
				}
			}
			if (Check_Aliens==0) {
                //Resets aliens
			for(Aliensx=0; Aliensx<=5; Aliensx++) {
				for(Aliensy=0; Aliensy<=2; Aliensy++) {
					AliensAlive[Aliensy][Aliensx]=1;
				}
			}
			Play=0;//Ends Play-Resets Aliens
            //****************************************************
		}
				
	 }//END Play

     //Ends game if all lives are lost
 	 if (lives==0) {
	     Restart=0;//Ends Game
         //Displays Final Score
		 GLCD_Clear(Black);
		 GLCD_SetBackColor(Black);
		 GLCD_SetTextColor(White);
		 GLCD_DisplayString(5, 5, __FI, "Game Over");
		 GLCD_DisplayString(6, 5, __FI, score1);
		 GLCD_DisplayString(6, 9, __FI, "Points");
		 score=0;//Resets Score
	 }
		 }//END GAME WHILE
		}//END GAME WHILE
	}

//****************INTERRUPTS***************************
//Tmer 0 interrupt handler-Alien/AlienShip Movement/Barrier Removal
void TIMER0_IRQHandler(void) {
    LPC_TIM0->IR |= 1 << 0; // Clear MR0 interrupt flag
	if (Play==1) {
        
        
 //*************Alien Movement***************
   if (moveLeft==0)
		Move_Aliens_Right();
	 else if (moveLeft==1)
		Move_Aliens_Left();
	 else if (moveLeft==2) {
		Move_Aliens_Down();
		delay(100000);//Ensures aliens move down sucessfully
		Move_Aliens_Right();
	  moveLeft=0;
	 }
	 else if (moveLeft==3) {
	    Move_Aliens_Down();
        delay(100000);//Ensures aliens move down sucessfully
		Move_Aliens_Left();
       moveLeft=1;
	 }
    //**************************************** 
     
 //*****************************Alien Ship Movement**************************  
	 if (Alienship_Pos<=-30)//Resets Alien Ship Position once it gets too far to the left
		 Alienship_Pos=400;
	 if (Draw_Ship==1)
		  Move_AlienShip_Left(); 
	 else if (Draw_Ship==0) {
		  GLCD_SetBackColor(Black);
		  GLCD_SetTextColor(White);
		  GLCD_DisplayString(9, 6, 1, score1);
		 	GLCD_SetTextColor(Black);
			Draw_Rect(Alienship_Pos,shipvert-15,38,31);
			Alienship_Pos=400;
		  Draw_Ship=1;
	 }
     //********************************************************************
     
 //*************************Barriers**********************************************
	 if (Remove_Barrier==1) {
		 BarrierHits[Barrier]-=1;
			if (Barrier==0) {
				Destroy_Barrier(Barrier,BarrierHits[Barrier]);
				Draw_Rect(Alien_Bullet_xPos,Barrier_yPos-6,2,6);
				Remove_Barrier=0;
			}
			else if (Barrier==1) {
				Destroy_Barrier(Barrier,BarrierHits[Barrier]);
				Draw_Rect(Alien_Bullet_xPos,Barrier_yPos-6,2,6);
				Remove_Barrier=0;
			}
			else if (Barrier==2) {
				Destroy_Barrier(Barrier,BarrierHits[Barrier]);
				Draw_Rect(Alien_Bullet_xPos,Barrier_yPos-6,2,6);
				Remove_Barrier=0;
			}
			else {
				Destroy_Barrier(Barrier,BarrierHits[Barrier]);
				Draw_Rect(Alien_Bullet_xPos,Barrier_yPos-6,2,6);
				Remove_Barrier=0;
			}
	 }
 }
}

//Tmer 1 interrupt handler-Cannon/Bullet Movement/SoundFX
void TIMER1_IRQHandler(void) {
	int i;
    LPC_TIM1->IR |= 1 << 0; // Clear MR0 interrupt flag
	if (Play==1) {

 //****************Cannon Movement**************
		if (moveCannon == 1)
			Move_Cannon_Left();
		if (moveCannon== 2)
			Move_Cannon_Right();
		moveCannon = 0;
        //***************************************
        
        
   //***************Bullets*************************
                //Draw Black over alien bullets when they reach the green line
		if (Alien_Bullet_yPos == Alien_Bullet_Stop) {
			GLCD_SetTextColor(Black);
			Draw_Rect(Alien_Bullet_xPos,Alien_Bullet_Stop,2,8);
			Alien_Bullet_yPos++;
		}

			 
        if (Fire==1)
			Move_Bullet_Up();
        if (AlienFire==1)
			Move_Bullet_Down();

	//****************************************************
    
    
//************Sound Effects******************************
        //In SoundFX.c there are big arrays of waveforms for each sound effect
			if (playshoot==1) {
				 for(i=0;i<4080;i++) {
			LPC_DAC->DACR = 16*Shoot[i]-0x8000;
					 delay(1000);
				 }
				 playshoot=0;
			 }
									 
			 if (playalienexplode==1) {
				 for(i=0;i<3377;i++) {
			LPC_DAC->DACR = 16*Alien_Explode[i]-0x8000;
					 delay(1000);
				 }
				 playalienexplode=0;
			 }
//			 
 			if (playcannonexplode==1) {
 				 for(i=0;i<8731;i++) {
 			LPC_DAC->DACR = 16*Cannon_Explode[i]-0x8000;
					 delay(1000);
 				 }
 				 playcannonexplode=0;
 			 }
         //*****************************************************
	}
}

//Tmer 2 interrupt handler-Aliens Firing
void TIMER2_IRQHandler(void) {
	int i;
    LPC_TIM2->IR |= 1 << 0; // Clear MR0 interrupt flag
	
				 if (Play==1) {
					 r=rand() % 6;//creates a random number
                     //bottom alien fires, if dead moves to next alien in column
					 for (Aliensy=2; Aliensy>=0; Aliensy--) {
						 if (AliensAlive[Aliensy][r]==1) {
                            //if alive fire bullet
							 if (Alien_Bullet_yPos>=Alien_Bullet_Stop & Remove_Barrier==0)
						 Draw_Alien_Bullet(Aliensy,r);
							 break;
							 
						 }
					 }	


    }
}



//*********************************************************************
//Delay Function
void delay(unsigned int nCount)	{

  for(; nCount != 0; nCount--);
}


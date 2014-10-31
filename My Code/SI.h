#ifndef _SI_H
#define _SI_H
#include <stdbool.h>
//All global variables/function delcarations that are to by seen outside of SpaceInvaders.c
extern void Draw_Rect( int x0, int y0, int width, int height);
extern void Draw_Cannon(int x0, int y0);
extern void Draw_Alien(int x0, int y0);
extern void Draw_AlienShip(int x0);
extern void Move_Cannon_Left(void);
extern void Move_Cannon_Right(void);
extern void Move_Aliens_Left(void);
extern void Move_Aliens_Right(void);
extern void Draw_Line(void);
extern void Draw_Bullet(void);
extern void Draw_Alien_Bullet(int row,int column);
extern void Move_Bullet_Up(void);
extern void Move_Bullet_Down(void);
extern void Move_Aliens_Down(void);
extern void Move_AlienShip_Left(void);
extern void Draw_Barrier(int x0);
extern void Destroy_Barrier(int barrier,int number);
extern bool Restart;
extern volatile int Alien_xPos;
extern volatile int Alien_yPos;
extern volatile int Cannon_Pos;
extern int Cannon_Vert;
extern volatile int Bullet_yPos;
extern volatile int Bullet_xPos;
extern int Alienship_Pos;
extern volatile int Alien_Bullet_xPos;
extern volatile int Alien_Bullet_yPos;
extern int Alien_Start_Horiz;
extern int Alien_Start_Vert;
extern char AliensAlive[3][6];
extern bool Delete;
extern int Aliensx;
extern int Aliensy;
extern bool AlienFire;
extern char Alien_Bullet_Stop;
extern bool Draw_Black_Bullet;
extern char Alien_Bullet_yPos_Old;
extern char Check_Aliens;
extern unsigned char score1[5];
extern void delay(unsigned int nCount);
extern volatile bool Play;
extern int r;
extern char score;
extern char shipvert;
extern bool Draw_Ship;
extern char Shoot[4080];
extern char Alien_Explode[3377];
extern char Cannon_Explode[8731];
extern char Win[65589];
extern char GameOver[165832];
extern int Barrier_yPos;
extern char Barrier_Left;
#endif

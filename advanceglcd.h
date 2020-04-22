#ifndef _ADVANCEGLCD_H
#define _ADVANCEGLCD_H

#include "stm32f7xx.h"
#include "glcd.h"
#include "stdio.h"

/*-----------------------------------------------------------------------------------------------
                                  **advance glcd function**
------------------------------------------------------------------------------------------------*/
void GLCD_INIT( void );
void glcd_axis(void);
void glcd_plot(float number,char step);
void GLCD_INIT( void );
void line (int x1,int x2,int y);
/*-----------------------------------------------------------------------------------------------
                                  **function code**
------------------------------------------------------------------------------------------------*/

void glcd_axis(void){
	//make x axis----------------------
	for (char x=0;x<=127;x++)
	{
		glcd_puts_point (x,0,1);	
	}
	//make y axis----------------------
	for (char y=0;y<=7;y++)
	{
		glcd_gotoxy (0,y);
		glcd_putchar (0xff,0);
	}
}
//--------------------------------------
void glcd_plot(float number,char step){
	Delay(T);
	char a,s;
	s=step*4;
	
//print point on coordinates---------
	for (a=0;a<=32;a++)
	{
		if(number <=(0.03230*a) && number>(0.03230*(a-1)))
		{
			glcd_puts_point (s,64-(a*2),2);
			glcd_puts_point (s+1,64-(2*a),2);
		}
	}
}
//-------------------------------------
//Function For Initialize GLCD Pins
void GLCD_INIT( void ){
	RCC->AHB1ENR |=1<<3 |1<<4;                                  		//Enable PORTD & PORTE Clock
	GPIOD->MODER |=1<<0 |1<<2|1<<4|1<<6|1<<8|1<<10|1<<12|1<<14; 		//PD0-PD7 Become OUTPUT "DATA PORT"
	GPIOE->MODER |=1<<4|1<<6|1<<8|1<<10|1<<12;                  		//PE2-PE6 Become OUTPUT "CTRL PORT"
	GPIOD->BSRR  |=1<<16|1<<17|1<<18|1<<19|1<<20|1<<21|1<<22|1<<23;	//RESET DATA PORT
	GPIOE->BSRR  |=1<<18|1<<19|1<<20|1<<21|1<<22;										//RESET CTRL PORT

	
}
//----------------------------------------------------------------------------------------------
 void glcd_line(int x1,int x2,int y)
 {
	 for(int k=x1;k<=x2;k++)
 {
			static char arr[]="-";
			glcd_puts(arr,k,y);

 } 
 }
 //---------------------------------------------------------------------------------------------
 
 
 
#endif
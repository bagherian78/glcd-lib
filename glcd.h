#ifndef _GLCD_H
#define _GLCD_H

#include "stm32f7xx.h"
#include "font.h"

#define T  1
//Data Pins
#define D0_PIN_NUMBER  0
#define D1_PIN_NUMBER  1
#define D2_PIN_NUMBER  2
#define D3_PIN_NUMBER  3
#define D4_PIN_NUMBER  4
#define D5_PIN_NUMBER  5
#define D6_PIN_NUMBER  6
#define D7_PIN_NUMBER  7
//Data Port
#define DATA_PORT  GPIOD
//Control Port
#define CTRL_PORT  GPIOE
//Select Column Port
#define SECO_PORT  GPIOE
//Control Pins
#define DI_PIN_NUMBER  2
#define RW_PIN_NUMBER  3
#define E_PIN_NUMBER   4
//Select Column Pins
#define CS1_PIN_NUMBER   5
#define CS2_PIN_NUMBER   6

//GLCD Function
void glcd_on(void );
void glcd_off(void);
void glcd_putchar(unsigned char data,unsigned char j);
void glcd_putImage(unsigned char data,unsigned char j);
void glcd_puts(char *str,char x,char y);
void glcd_puts_point(char x,char y,char w);
void glcd_disp_Image(unsigned char *str,char x1,char x2, char y1,char y2);
void glcd_gotoxy(unsigned char x, unsigned char y);
void glcd_clear_all(void);
void glcd_clrearline(unsigned line);
void Delay(int t);
void send_command(unsigned char data);
void goto_row(unsigned char y);
void goto_column(unsigned char x);
void glcd_clear_here(char x1,char x2, char y1,char y2);
// Functions Code:
void Delay(int t)
{
	t*=10;
	while(t)
	{
		t--;
	}
}
//--------------------------------------
void send_command(unsigned char command){
	Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));      //E_PIN_NUMBER=RESET
	Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (RW_PIN_NUMBER));     //RW_PIN_NUMBER=RESET
	Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (DI_PIN_NUMBER));     //DI_PIN_NUMBER=RESET
	Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR | (1 << CS1_PIN_NUMBER);         //CS1_PIN_NUMBER=SET
	Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR | (1 << CS2_PIN_NUMBER);         //CS2_PIN_NUMBER=SET
	Delay(T);
  DATA_PORT->ODR = 0x00 << (D0_PIN_NUMBER);  //DATA_PORT=RESET
	Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (E_PIN_NUMBER));           //E_PIN_NUMBER=SET
	Delay(T);
	DATA_PORT->ODR = command << D0_PIN_NUMBER;    //DATA_PORT=Command
	Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));      //E_PIN_NUMBER=RESET
}
//--------------------------------------
void glcd_on(void){
	Delay(T);
  send_command(0x3F);     //GLCD on
	send_command(0x40);     //SET Cursor on Y=0
	send_command(0xB8);     //SET Page on X=0,Line=0
	send_command(0xC0);     //Display Start Line=0xC0
}
//--------------------------------------
void glcd_off(void){
	Delay(T);
  send_command(0x3E);
}
//--------------------------------------
void glcd_putchar(unsigned char data,unsigned char j){ //print one char on glcd
	if(j<64){                                      //Left section
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));     //E_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (RW_PIN_NUMBER));     //RW_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (DI_PIN_NUMBER));          //DI_PIN_NUMBER=SET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR | (1 << CS1_PIN_NUMBER);         //CS1_PIN_NUMBER=SET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR &~ (1 << CS2_PIN_NUMBER);    //CS2_PIN_NUMBER=RESET
		Delay(T);
  DATA_PORT->ODR = 0x00 << (D0_PIN_NUMBER);  //DATA_PORT=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (E_PIN_NUMBER));          //E_PIN_NUMBER=SET
		Delay(T);
	DATA_PORT->ODR = data << D0_PIN_NUMBER;       //DATA_PORT=DATA
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));       //E_PIN_NUMBER=RESET
	}
	else{                                          //Right section
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));       //E_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (RW_PIN_NUMBER));     //RW_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (DI_PIN_NUMBER));         //DI_PIN_NUMBER=SET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR &~ (1 << CS1_PIN_NUMBER);    //CS1_PIN_NUMBER=RESET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR | (1 << CS2_PIN_NUMBER);          //CS2_PIN_NUMBER=SET
		Delay(T);
  DATA_PORT->ODR = 0x00 << (D0_PIN_NUMBER);    //DATA_PORT=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (E_PIN_NUMBER));          //E_PIN_NUMBER=SET
		Delay(T);
	DATA_PORT->ODR = data << D0_PIN_NUMBER;       //DATA_PORT=DATA
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));      //E_PIN_NUMBER=RESET
	}
}
//--------------------------------------
void glcd_puts(char *str,char x,char y){         //x:x_axis  y:y_axis    *str:string
	Delay(T);
	int i,j;
	while(*str!=0){
		i=(8*(*str));
		j=i+8;
			for(;i<=j;i++){
				glcd_gotoxy( x , y);
				glcd_putchar(Font[i],x);                 //
				++x;                                     //Next position
				if(x>121){                               //The lack of current data in the last seven lines of the line
					if(i%8==0){                            //If data does not exist in the last seven
						++y;                                 //Next line
						x=0;                                 //Beginning line
					}
				}
			}
		str++;
	}
}
//--------------------------------------
void glcd_putImage(unsigned char data,unsigned char j){
	if(j<64){                                      //Left section
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));      //E_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (RW_PIN_NUMBER));     //RW_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (DI_PIN_NUMBER));          //DI_PIN_NUMBER=SET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR | (1 << CS1_PIN_NUMBER);         //CS1_PIN_NUMBER=SET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR &~ (1 << CS2_PIN_NUMBER);    //CS2_PIN_NUMBER=RESET
		Delay(T);
  DATA_PORT->ODR = 0x00 << (D0_PIN_NUMBER);  //DATA_PORT=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (E_PIN_NUMBER));           //E_PIN_NUMBER=SET
		Delay(T);
	DATA_PORT->ODR = data << D0_PIN_NUMBER;       //DATA_PORT=DATA
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));       //E_PIN_NUMBER=RESET
	}                                              
	else{                                          //Right section
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));       //E_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (RW_PIN_NUMBER));     //RW_PIN_NUMBER=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (DI_PIN_NUMBER));         //DI_PIN_NUMBER=SET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR &~ (1 << CS1_PIN_NUMBER);    //CS1_PIN_NUMBER=RESET
		Delay(T);
	SECO_PORT->ODR = SECO_PORT->ODR | (1 << CS2_PIN_NUMBER);        //CS2_PIN_NUMBER=SET
		Delay(T);
  DATA_PORT->ODR = 0x00 << (D0_PIN_NUMBER);  //DATA_PORT=RESET
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (E_PIN_NUMBER));          //E_PIN_NUMBER=SET
		Delay(T);
	DATA_PORT->ODR = data << D0_PIN_NUMBER;       //DATA_PORT=DATA
		Delay(T);
	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));      //E_PIN_NUMBER=RESET
	}
}
//--------------------------------------
void glcd_disp_Image(unsigned char *str,char x1,char x2, char y1,char y2){  //print Image on glcd x1:First_x_axis  x2:End_x_axis  y1:First_y_axis   y2:End_y_axis
	Delay(T);
	  unsigned char i,j;    
    for(i=y1;i<=y2;i++)
    {
		  					
        for(j=x1;j<=x2;j++)
        {                       
           glcd_gotoxy(j,i);  					
           glcd_putImage(*str,j);
			 		 str++;
        }
					
    }
}
//--------------------------------------
void glcd_puts_point(char x,char y,char w){     //print point on x:x_axis y:y_axis ,w:width 
	                                              //To draw a horizontal line : x = 0 to 127 , y = 0 to 63 , w = 1 to 63
	                                              //To draw a vertical line   : x = 0 to 127 , y = 0 to 63 , w = (0 to 8)<= (y%8) 
	unsigned char point=0x00,dot,i=0;
	dot=y%8;
	y=(y/8);
	if(dot==0)y-=1;
	for(i=0;i<w;i++){
			switch(dot){
				case 0 :
					point|=0x80;
					break;
				case 1 :
					point|=0x01;
					break;
				case 2 :
					point|=0x02;
					break;
				case 3 :
					point|=0x04;
					break;
				case 4 :
					point|=0x08;
					break;
				case 5 :
					point|=0x10;
					break;
				case 6 :
					point|=0x20;
					break;
				case 7 :
					point|=0x40;
					break;
			}
			if(dot==1){
				glcd_gotoxy(x,y);  					
        glcd_putImage(point,x);
				y-=1;
				point=0x00;
			}
			if(dot==0)dot=8;
			dot-=1;
	}
	if(point!=0x00){
		glcd_gotoxy(x,y);  					
		glcd_putImage(point,x);
	}
}
//--------------------------------------
void glcd_gotoxy(unsigned char x, unsigned char y){
	Delay(T); 
		 goto_row( y );                               //Row Y = 0 to 7
		 goto_column( x );                            //Column x = 0 to 127
  }
//--------------------------------------
void goto_row(unsigned char y){ 
   Delay(T);	
   y = (y | 0xB8 ) & 0xBF;                     			   //Set command to Display Control Instruction table
   send_command( y );                
}
//--------------------------------------
void goto_column(unsigned char x){
	Delay(T); 
   if(x<64)                                            //Left section
   {
		  x = (x | 0x40 ) & 0x7F;                          //Set command to Display Control Instruction table
		  CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));        //E_PIN_NUMBER=RESET
		 Delay(T);
    	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (RW_PIN_NUMBER));       //RW_PIN_NUMBER=RESET
		 Delay(T);
	    CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (DI_PIN_NUMBER));       //DI_PIN_NUMBER=RESET  
		 Delay(T);
      SECO_PORT->ODR = SECO_PORT->ODR | 1 << (CS1_PIN_NUMBER);         //CS1_PIN_NUMBER=SET
		 Delay(T);
      SECO_PORT->ODR = SECO_PORT->ODR &~ (1 << (CS2_PIN_NUMBER));      //CS2_PIN_NUMBER=RESET  
		 Delay(T);
			DATA_PORT->ODR = 0x00 << (D0_PIN_NUMBER);    //DATA_PORT=RESET
		 Delay(T);
			CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (E_PIN_NUMBER));             //E_PIN_NUMBER=SET
		 Delay(T);
			DATA_PORT->ODR = x << D0_PIN_NUMBER;            //DATA_PORT=x  
		 Delay(T);		 
    	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));        //E_PIN_NUMBER=RESET		 
   }
   else                                                //Right section
   {
	    x-=64;   		                                     //Put column address on section2
			x = (x | 0x40 ) & 0x7F;                          //Set command to Display Control Instruction table
	  	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));        //E_PIN_NUMBER=RESET
		 Delay(T);
    	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (RW_PIN_NUMBER));        //RW_PIN_NUMBER=RESET
		 Delay(T);
    	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (DI_PIN_NUMBER));       //DI_PIN_NUMBER=RESET 
		 Delay(T);
		  SECO_PORT->ODR = SECO_PORT->ODR &~ (1 << (CS1_PIN_NUMBER));       //CS1_PIN_NUMBER=RESET
		 Delay(T);
		  SECO_PORT->ODR = SECO_PORT->ODR | 1 << (CS2_PIN_NUMBER);           //CS2_PIN_NUMBER=SET 
		 Delay(T);
			DATA_PORT->ODR = 0x00 << (D0_PIN_NUMBER);    //DATA_PORT=RESET
		 Delay(T);
			CTRL_PORT->ODR = CTRL_PORT->ODR | (1 << (E_PIN_NUMBER));              //E_PIN_NUMBER=SET
		 Delay(T);
			DATA_PORT->ODR = x << D0_PIN_NUMBER;            //DATA_PORT=x  
		 Delay(T);
		 	CTRL_PORT->ODR = CTRL_PORT->ODR &~ (1 << (E_PIN_NUMBER));        //E_PIN_NUMBER=RESET	
   }    
} 
//--------------------------------------
void glcd_clrearline(unsigned line){                   //Clear Line , Line = 0 to 7
	  Delay(T);
	  int i;
    glcd_gotoxy(0,line);                               //At start of line of left side
	  SECO_PORT->ODR = SECO_PORT->ODR | 1 << (CS1_PIN_NUMBER);             //CS1_PIN_NUMBER=SET
	Delay(T);
	  SECO_PORT->ODR = SECO_PORT->ODR &~ (1 << (CS1_PIN_NUMBER)); ;        //CS1_PIN_NUMBER=RESET
	
	  for(i=0;i<64;i++){
         glcd_putchar(0,0);                            //Print 0 for Delete Left section
			   glcd_putchar(0,64);                           //Print 0 for Delete Right section
		}
    glcd_gotoxy(64,line);                              //At start of line of right side
    for(i=0;i<64;i++){
         glcd_putchar(0,0);                            //Print 0 for Delete Left section
			   glcd_putchar(0,64);                           //Print 0 for Delete Right section
		}
}

// Draw line between (x1,y1) and (x2,y2) 
void glcd_drawline(unsigned char x1,unsigned char y1,unsigned char x2,unsigned char y2){ 


}

//-------------------------
void glcd_clear_all(){
	int i;
  for(i=0;i<8;i++)
     glcd_clrearline(i);
  send_command(0x40);                                  //SET Cursor on Y=0
	send_command(0xB8);                                  //SET Page on X=0,Line=0
}
//-----------------------
void glcd_clear_here(char x1,char x2, char y1,char y2){//Clear xi to x2 ,y1 to y2 ,x1:First_x_axis  x2:End_x_axis  y1:First_y_axis   y2:End_y_axis ... x = 0 to 127 , y = 0 to 7  
	Delay(T);
	  unsigned char i,j;    
    for(i=y1;i<=y2;i++)
    {
        for(j=x1;j<=x2;j++)
        {                       
           glcd_gotoxy(j,i);  					
           glcd_putImage(0,j);
        }                                              
    }	
}
#endif

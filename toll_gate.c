////////////////////////////////////////////////////////////////////////////////////
//  Project Name : toll_gate
//
//  Target Board : atmega8
//
//  Date Created : 1/22/2017 10:12:09 AM
// 
//  Online Resource  : www.electronicsforstudents.com
///////////////////////////////////////////////////////////////////////////////////

#include<avr/io.h>
#include<util/delay.h>
#include"serial.h"

#include"lcd.h"
#include"rfid.h"

void main_menu()
{
lcd_gotoxy(0,0);
lcd_string("1. REGISTER USER");

lcd_gotoxy(0,1);
lcd_string("2. VERIFY USER");
}

void gate_open()
{
int k;
for(k=0;k<15;k++)
{
     write(portc,0,h);
	 _delay_us(2000);
	  write(portc,0,l);
	 _delay_us(18000);
}
}

void gate_close()
{
int k;
for(k=0;k<15;k++)
{
     write(portc,0,h);
	 _delay_us(1000);
	  write(portc,0,l);
	 _delay_us(19000);
}
}


int main()
{
int i=0;
write(portc,5,h);

lcd_init();
serial_init(9600);
gate_close();
main_menu();
int user[i];

 while (1)
  {
	if(read(portd,3)==0) //first menu
	 {
	 lcd_clear();
	 lcd_gotoxy(0,0);
	 lcd_string("PLACE THE CARD");
	 lcd_gotoxy(0,1);
	 lcd_string("TO REGISTER");
			 
	   user[i]=read_rfid();
	   i=i++;
			   
		lcd_clear();
		lcd_gotoxy(0,0);
		lcd_string("CARD REGISTERED");
			 
		 _delay_ms(1500);
			 
		 main_menu();
		 
	 }
		 
		 
		
     if(read(portc,5)==0) //second menu
	  {
		lcd_clear();
	    lcd_gotoxy(0,0);
	    lcd_string("PLACE THE CARD");
		lcd_gotoxy(0,1);
		lcd_string("TO VERIFY");
			 
		long temp=0,result=0;
		 temp=read_rfid();
		 int j;
			 
		 for(j=0;j<5;j++)
		  { 
		    if(temp==user[j])
			 {
				 result=1;
				 break;
			 }
			  else
			  {
			    result=0;
			  }
		  }
		  
						
             if(result==1)
              { 
                       
                 lcd_clear();
                 lcd_gotoxy(0,0);
                 lcd_string("VALID USER");
				 
                  gate_open();
			     _delay_ms(3000);
                  gate_close();
              }

                else
                {
                 lcd_clear();
                 lcd_gotoxy(0,0);
			     lcd_string("INVALID USER");
			    }
					
			    _delay_ms(1500);
					
			    main_menu();
				
              
		 }

	}

}
/****************************************************************************
 Title	:   Serial library
 Author:    bharath
 company: www.robotics.digitlabz.com
 File:	    lcd
 Software:  AVR-GCC 3.3 
 Target:    any AVR device

 DESCRIPTION
       
*****************************************************************************/


#ifndef SERIAL_H
#define SERIAL_H

#include <avr/interrupt.h>

#define RECEIVE_BUFF_SIZE 128
#define USART_RXC_VECT USART_RXC_vect


volatile char URBuff[RECEIVE_BUFF_SIZE];	//USART Receive Buffer
volatile int8_t UQFront;
volatile int8_t UQEnd;


void serial_init(uint16_t baudrate)
{
uint16_t ubrrvalue = ((F_CPU+(baudrate*8L))/(baudrate*16L)-1);
	//Setup q
	UQFront=UQEnd=-1;

	//Set Baud rate
	UBRRH=(unsigned char)(ubrrvalue>>8);
	UBRRL=(unsigned char)ubrrvalue;
	UCSRC=(1<<URSEL)|(3<<UCSZ0);
	UCSRB=(1<<RXCIE)|(1<<RXEN)|(1<<TXEN);
	sei();

}

unsigned char serial_read()
{
	char data;
	
	//Check if q is empty
	if(UQFront==-1)
		return 0;
	
	data=URBuff[UQFront];
	
	if(UQFront==UQEnd)
	{
	//If single data is left
	//So empty q
	UQFront=UQEnd=-1;
	}
	else
	{
		UQFront++;

		if(UQFront==RECEIVE_BUFF_SIZE)
		UQFront=0;
	}

	return data;
}

void serial_char(char data)
{
	//Wait For Transmitter to become ready
	while(!(UCSRA & (1<<UDRE)));

	//Now write
	UDR=data;
}



void serial_num(long num)
{
	char buffer[7];

 ltoa( num , buffer, 10);
    serial_string(buffer);
}

void serial_string(char *str)
{
	while((*str)!='\0')
	{
		serial_char(*str);
		str++;
	}
}

void UReadBuffer(void *buff,uint16_t len)
{
	uint16_t i;
	for(i=0;i<len;i++)
	{
		((char*)buff)[i]=serial_read();
	}
}

uint8_t serialdata_available()
{
	if(UQFront==-1) return 0;
	if(UQFront<UQEnd)
		return(UQEnd-UQFront+1);
	else if(UQFront>UQEnd)
		return (RECEIVE_BUFF_SIZE-UQFront+UQEnd+1);
	else
		return 1;
}
void UFlushBuffer()
{
	while(serialdata_available()>0)
	{
		serial_read();
	}
}


ISR(USART_RXC_VECT)
{
	//Read the data
	char data=UDR;

	//Now add it to q

	if(((UQEnd==RECEIVE_BUFF_SIZE-1) && UQFront==0) || ((UQEnd+1)==UQFront))
	{
		//Q Full
		UQFront++;
		if(UQFront==RECEIVE_BUFF_SIZE) UQFront=0;
	}
	

	if(UQEnd==RECEIVE_BUFF_SIZE-1)
		UQEnd=0;
	else
		UQEnd++;


	URBuff[UQEnd]=data;

	if(UQFront==-1) UQFront=0;

}


int compare(const char *cmd1,const char *cmd2)
{
    unsigned char len;
	len=strlen(cmd2);
	

if(strncasecmp(cmd1,cmd2,len)==0)
{
   return 1;

}
else
{

    return 0;
}

}


#endif

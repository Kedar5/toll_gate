
#ifndef RFID_H
#define RFID_H


#include"serial.h"
long card_number;
unsigned char card_id[30];
long read_rfid()
{
       while(serialdata_available()<=8);
        _delay_ms(50);
		 serial_read();
			    serial_read();	
	   
	    UReadBuffer(card_id,8);	       
		char *eptr;  
		
			return strtol(card_id,  eptr, 16);
}


#endif
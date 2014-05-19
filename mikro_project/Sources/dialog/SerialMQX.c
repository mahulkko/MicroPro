/*************************************************************************
/	Module		: Zugriffsfkt. Serielle Schnittstelle RS232
/-------------------------------------------------------------------------
/	Description	: Zugriffsfunktionen zum öffnen,lesen,schreiben,schliessen 
/				  eines RS232 Gerätes.
/
/-------------------------------------------------------------------------
/	Department	: HTWG Konstanz, Studiengang Techn. Informatik
/	Project		: Display Treiber EAeDIP320_8.c
/	Version		: 2.0
/	Released	: 07.03.2014
/	Written by	: Andreas Schmidt
//  Edited by   : Jan Radon
//  Edited by   : Manuel Beha 
/************************************************************************/


#include "SerialMQX.h"

/* The delay-value for rs232 was 2 for the coldfire;
/ for twrk60 n512 it should be 0; 07.03.2014 mabeha */
#define rs232delay 0

FILE_PTR SerialOpen(char* device, int baud) {
    
    FILE_PTR devPtr;
    int baudRate;
    devPtr = fopen(device, NULL);
    baudRate = baud; 
    if( devPtr != NULL )
    {   		
    	// Baudrate setzen 
       //	ioctl(devPtr, (unsigned long)IO_IOCTL_SERIAL_SET_BAUD , &baudRate);
    	return devPtr;
    }    
    return devPtr;        
}

int SerialReadDataWaiting(FILE_PTR devPtr) {
    int charAvail=0;    
    _time_delay(rs232delay);   
    ioctl(devPtr, IO_IOCTL_CHAR_AVAIL, &charAvail);        
    return charAvail;    
}

int SerialReadFrom(FILE_PTR devPtr,unsigned char* buffer, int length) {
    _time_delay(rs232delay);   
    return _io_read(devPtr, buffer, length);
}

void SerialWait(int ms) {
    _time_delay(ms);
}

int SerialWriteTo(FILE_PTR devPtr,unsigned char* buffer, int length) { 
	_time_delay(rs232delay+20);   
    return _io_write(devPtr,buffer,length); 
}

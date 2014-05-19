/*************************************************************************
/	Module		: Zugriffsfkt. Serielle Schnittstelle RS232
/-------------------------------------------------------------------------
/	Description	: Zugriffsfunktionen zum öffnen,lesen,schreiben,schliessen 
/				  eines RS232 Gerätes.
/
/-------------------------------------------------------------------------
/	Department	: HTWG Konstanz, Studiengang Techn. Informatik
/	Project		: Display Treiber EAeDIP320_8.c
/	Version		: 1.5
/	Released	: 14.04.2010
/	Written by	: Andreas Schmidt
//  Edited by   : Jan Radon
/************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>


	int SerialClose();    
	FILE_PTR  SerialOpen(char* device, int baud);    
	int SerialReadDataWaiting(FILE_PTR devPtr);    
	int SerialReadFrom(FILE_PTR devPtr,unsigned char* buffer, int length );   
	void SerialWait(int ms);    
	int SerialWriteTo(FILE_PTR devPtr,unsigned char* buffer, int length);


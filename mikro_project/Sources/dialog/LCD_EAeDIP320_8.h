/*************************************************************************
/	Module		: Zugriffsfkt. LCD_EAeDIP320_8
/-------------------------------------------------------------------------
/	Description	: Implementierung der Befehlstabelle des EA eDIP320_8
/				  LCD Displays.
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
#include <stdlib.h>
#ifndef LCD_EAeDIP320_8_H 

#define LCD_EAeDIP320_8_H 




//## package WidgetFactory::LCDDriver 
// Maximum payload length for a packet
#define MAX_PAYLOAD			(128)

// Maximum size for a packet including header, payload and checksum
#define MAX_PACKET_SIZE		(MAX_PAYLOAD + 3)

// Parameter types for the display commands
#define SHORT_PARAM			(0)		// 16 Bit Parameter
#define BYTE_PARAM			(1)		// 8 Bit Parameter
#define STRING_PARAM		(2)		// String parameter
#define NO_PARAM			(-1)	// undefined parameter

#define PARAMETER_START		(5)		// index where the parameter start in the packet

#define DC1 				(0x11)	// Header DC1 for packets
#define DC2 				(0x12)	// Header DC2 for packets


    //## operation calculatePacketChecksum(unsigned char*,int) 
    unsigned char calculatePacketChecksum(unsigned char* byteBuffer, int len);
    
    //## operation driverConnect(char*,int) 
    int driverConnect(FILE_PTR devPtr,char* device, int baud);
    
    //## operation driverDisconnect() 
    int driverDisconnect();
    
    //## operation getBuffer() 
    int getBuffer(FILE_PTR devPtr);
    
    //## operation getBufferInformation(int *,int *) 
    int getBufferInformation(FILE_PTR devPtr,int * bufferBytes, int * freeBytes);
    
    //## operation sendAA(char) 
    int sendAA(FILE_PTR devPtr,char val);
    
    //## operation sendAB(char) 
    int sendAB(FILE_PTR devPtr,char no);
    
    //## operation sendAD(short,short,short,short,char) 
    int sendAD(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char linewidth);
    
    //## operation sendAE(char) 
    int sendAE(FILE_PTR devPtr,char border);
    
    //## operation sendAF(char) 
    int sendAF(FILE_PTR devPtr,char no);
    
    //## operation sendAG(char) 
    int sendAG(FILE_PTR devPtr,char no);
    
    //## operation sendAH(short,short,short,short) 
    int sendAH(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    //## operation sendAI(char) 
    int sendAI(FILE_PTR devPtr,char val);
    int sendTA(FILE_PTR devPtr);
    //## operation sendAK(short,short,short,short,char,char,char*) 
    int sendAK(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char downcode, char upcode, char* text);
    
    //## operation sendAL(char,char) 
    int sendAL(FILE_PTR devPtr,char code, char val);
    
    //## operation sendAN(char) 
    int sendAN(FILE_PTR devPtr,char code);
    
    //## operation sendAO(char) 
    int sendAO(FILE_PTR devPtr,char val);
    
    //## operation sendAP(char,char) 
    int sendAP(FILE_PTR devPtr,char code, char val);
    
    //## operation sendAQ(char) 
    int sendAQ(FILE_PTR devPtr,char val);
    
    //## operation sendAR(char) 
    int sendAR(FILE_PTR devPtr,char no);
    
    //## operation sendAS(char) 
    int sendAS(FILE_PTR devPtr,char val);
    
    //  display light on off.
    int sendYL(FILE_PTR devPtr,char mode);
    
    // Touch areas
    //## operation sendAT(short,short,short,short,char,char,char*) 
    int sendAT(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char downcode, char upcode, char* text);
    
    //## operation sendAV(short,short,char) 
    int sendAV(FILE_PTR devPtr,short x, short y, char val);
    
    //## operation sendAW(char) 
    int sendAW(FILE_PTR devPtr,char val);
    
    //## operation sendAX(char) 
    int sendAX(FILE_PTR devPtr,char code);
    
    //## operation sendAY(char) 
    int sendAY(FILE_PTR devPtr,char val);
    
    //## operation sendAZ(char,char) 
    int sendAZ(FILE_PTR devPtr,char val1, char val2);
    
    //## operation sendBA(char,char) 
    int sendBA(FILE_PTR devPtr,char no, char val);
    
    //## operation sendBD(char,char) 
    int sendBD(FILE_PTR devPtr,char no, char action);
    
    //## operation sendBL(char,short,short,short,short,char,char,char,char) 
    int sendBL(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst);
    
    //## operation sendBO(char,short,short,short,short,char,char,char,char) 
    int sendBO(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst);
    
    // 
    // Bargraph commands
    //## operation sendBR(char,short,short,short,short,char,char,char,char) 
    int sendBR(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst);
    
    //## operation sendBRLOU(char,char,short,short,short,short,char,char,char,char) 
    int sendBRLOU(FILE_PTR devPtr,char barType, char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst);
    
    //## operation sendBS(char) 
    int sendBS(FILE_PTR devPtr,char no);
    
    //## operation sendBU(char,short,short,short,short,char,char,char,char) 
    int sendBU(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst);
    
    //## operation sendBZ(char) 
    int sendBZ(FILE_PTR devPtr,char no);
    
    //## operation sendCommand(char*,char*) 
    int sendCommand(FILE_PTR devPtr,char* cmdStr, char* cmdParam);
    
    //## operation sendDA() 
    int sendDA(FILE_PTR devPtr);
    
    //## operation sendDE() 
    int sendDE(FILE_PTR devPtr);
    
    //## operation sendDI() 
    int sendDI(FILE_PTR devPtr);
    
    // 
    // Display commands
    //## operation sendDL() 
    int sendDL(FILE_PTR devPtr);
    
    //## operation sendDS() 
    int sendDS(FILE_PTR devPtr);
    
    //## operation sendGD(short,short,short,short) 
    int sendGD(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    //## operation sendGM(char) 
    int sendGM(FILE_PTR devPtr,char pattern);
    
    //## operation sendGP(short,short) 
    int sendGP(FILE_PTR devPtr,short x1, short y1);
    
    // 
    // Lines and points
    //## operation sendGR(short,short,short,short) 
    int sendGR(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    //## operation sendGV(char) 
    int sendGV(FILE_PTR devPtr,char mode);
    
    //## operation sendGW(short,short) 
    int sendGW(FILE_PTR devPtr,short x1, short y1);
    
    //## operation sendGZ(char,char) 
    int sendGZ(FILE_PTR devPtr,char sizeX, char sizeY);
    
    //## operation sendQI(short,short,short,short) 
    int sendQI(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    // 
    // Blink areas
    //## operation sendQL(short,short,short,short) 
    int sendQL(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    //## operation sendQM(short,short,short,short,char) 
    int sendQM(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern);
    
    //## operation sendQZ(char) 
    int sendQZ(FILE_PTR devPtr,char time);
    
    //## operation sendRI(short,short,short,short) 
    int sendRI(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    // 
    // Rectangular areas
    //## operation sendRL(short,short,short,short) 
    int sendRL(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    //## operation sendRM(short,short,short,short,char) 
    int sendRM(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern);
    
    //## operation sendRO(short,short,short,short,char) 
    int sendRO(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern);
    
    //## operation sendRR(short,short,short,short,char) 
    int sendRR(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern);
    
    //## operation sendRS(short,short,short,short) 
    int sendRS(FILE_PTR devPtr,short x1, short y1, short x2, short y2);
    
    //## operation sendRT(short,short,short,short,char) 
    int sendRT(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern);
    
    //## operation sendZB(char) 
    int sendZB(FILE_PTR devPtr,char blinkMode);
    
    //## operation sendZC(short,short,char*) 
    int sendZC(FILE_PTR devPtr,short x, short y, char* text);
    
    //## operation sendZF(char) 
    int sendZF(FILE_PTR devPtr,char fontNo);
    
    // 
    // String Operations
    //## operation sendZL(short,short,char*) 
    int sendZL(FILE_PTR devPtr,short x, short y, char* text);
    
    //## operation sendZLCR(char,short,short,char*) 
    int sendZLCR(FILE_PTR devPtr,char dir, short x, short y, char* text);
    
    //## operation sendZM(char) 
    int sendZM(FILE_PTR devPtr,char pattern);
    
    //## operation sendZR(short,short,char*) 
    int sendZR(FILE_PTR devPtr,short x, short y, char* text);
    
    //## operation sendZV(char) 
    int sendZV(FILE_PTR devPtr,char mode);
    
    //## operation sendZW(char) 
    int sendZW(FILE_PTR devPtr,char angel);
    
    //## operation sendZY(char) 
    int sendZY(FILE_PTR devPtr,char dist);
    
    //## operation sendZZ(char,char) 
    int sendZZ(FILE_PTR devPtr,char zoomX, char zoomY);


#endif  
/*********************************************************************
	File Path	: MQX_View/MQX_View_Config/LCD_EAeDIP320_8.h
*********************************************************************/


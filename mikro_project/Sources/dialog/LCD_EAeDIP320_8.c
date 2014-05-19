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
/	Released	: 01.04.2014
/	Written by	: Andreas Schmidt
//  Edited by   : Jan Radon
//  Edited by   : Manuel Beha 
/************************************************************************/


#include "LCD_EAeDIP320_8.h"
#include "SerialMQX.h"
#include <stdlib.h>
#include <string.h>
//----------------------------------------------------------------------------
// LCD_EAeDIP320_8.cpp                                                                  
//----------------------------------------------------------------------------

unsigned char calculatePacketChecksum(unsigned char* byteBuffer, int len) {
    
    int i 	= 0;
    int sum = 0;
        
    for( i = 0; i < len; i++ ){
    	sum += byteBuffer[i];	
    }    
    
    sum = sum % 256;    
    
    return (unsigned char)sum;    
}

/*************************************************************************
/	Function        : getBuffer(FILE_PTR devPtr)
/-------------------------------------------------------------------------
/	Description	:    Liefert den OPCode des gedrückten Buttons .
/
/	Input Parameter : devPtr pointer zur Seriellen Schnittstelle
/                               
/	Output Parameter: op (upcode des gedrückten Buttons.)
/	Written by	: Andreas Schmidt
/	Editted by  : Jan Radon
/   Date        : 25.11.2006
/-------------------------------------------------------------------------
/	Changes		: Liefert nur noch den OP Code zurück.
/	Date		: 07.03.2014
/	Purpose		: OPCode
/	Changed by	: Jan Radon
/	Changed by	: Manuel Beha
/*************************************************************************/
int getBuffer(FILE_PTR devPtr) {

    unsigned char  packetBuffer[4];
    unsigned char  bufferInfo[2];
    unsigned char* buffer = NULL;
    unsigned char  checkSum 		= 0;
    unsigned char  readBuffer 		= 0;
    int    bufferSize    	= 0;
    int    result 			= 0;
    int    getCount			= 0;
    int	   charAvail		= 0;
    int op = 0;
    
    // build header
    packetBuffer[0] = DC2;			// packet header
    packetBuffer[1] = 0x01; 		// temp len, calculated later	
    packetBuffer[2] = 'S';			// first command char
    
    checkSum = calculatePacketChecksum( packetBuffer, 3 );
    packetBuffer[3] = checkSum;
    
    
    // write the packet to serial port
    result = SerialWriteTo(devPtr,packetBuffer, 4);
    	
    if( result != 4 ){
    	printf("[getBuffer]: Error while writing packet\n");
    	return 0;
    }
    	
    /* This worked for the coldfire, but doesn´t work for the
     * twrk60 n512; mabeha 07.03.14
     * for( getCount = 0; getCount < 5; getCount ++ ){		

     	//see if some data is waiting for us
        charAvail = SerialReadDataWaiting(devPtr);					
        
    	if( charAvail ){
    		printf("read ACK. \n");
    		// read the ACK
    		result = SerialReadFrom(devPtr,((unsigned char*)&readBuffer), 1);
    	
    		if( result != 1 ){
    			printf("[getBuffer]: Error while reading ACK\n");
    			return 0;
    		}	
    		
    		break;
    	}
    	
    	printf("[getBuffer]: Waiting for ACK\n");
    	
    	//SerialWait(150);
    	SerialWait(1);
    }*/
    
    // get directly the ACK-byte from the display; this work for twrk60 n512; mabeha 07.03.14
    result = SerialReadFrom(devPtr,((unsigned char*)&readBuffer), 1);
        	
     if( result != 1 ){
    	 printf("[getBuffer]: Error while reading ACK\n");
    	 return 0;
    }	
        
    if( readBuffer == 0x06 ){
    
    	// read the first two bytes from buffer data
    	result = SerialReadFrom(devPtr,bufferInfo, 2);
    	
    	if( result != 2 ){
    		printf("[getBuffer]: Error while reading packet header\n");
    		return 0;
    	}
    	
    	// wrong packet
    	if( bufferInfo[0] != DC1 ){	
    		printf("[getBuffer]: Wrong packet header\n");
    		return 0;
    	}			
    	
    	// nothing in buffer
    	if( bufferInfo[1] == 0 ){
    		buffer = NULL;
    		return 0;
    	}
    				
    	bufferSize = bufferInfo[1] + 3;
    //	printf("BufferSize: %d\n", bufferSize);
    	
    	buffer = (unsigned char*) malloc(sizeof(unsigned char) * bufferSize);
    	    	
    	if( buffer == NULL ){
    		printf("[GetBuffer]: Out of memory\n");
    		return 0;
    	}
    	
    	// read the rest of the packets
    	result = SerialReadFrom(devPtr,&buffer[2], bufferInfo[1] + 1);
    	
    	if( result != bufferInfo[1] + 1 )
    	{
    		printf("[getBuffer]: Error while buffer content\n");
    		
    		_mem_free(buffer);
    		return 0;
    	}
    	    	    	
    	buffer[0] = bufferInfo[0];
    	buffer[1] = bufferInfo[1];
    	
    	op = buffer[5];				//Ersten op-code auslesen (der rest wird verworfen)
    	
    //	printf("op_lcd_EAeDIP320_8= %d\n",op);
    	return op;
    }
    	
    return 0;
}
/**********************************************************************************************
/	Function        : getBufferInformation(FILE_PTR devPtr,int * bufferBytes,int * freeBytes)
/----------------------------------------------------------------------------------------------
/	Description	: 
/
/	Input Parameter : devPtr pointer zur Seriellen Schnittstelle
/                               
/	Output Parameter: -1 bei Fehler
/					  0 bei Erfolg
/	Written by	: Andreas Schmidt
/   Date        : 25.11.2006
/----------------------------------------------------------------------------------------------
/**********************************************************************************************/
int getBufferInformation(FILE_PTR devPtr,int * bufferBytes, int * freeBytes) {

    unsigned char packetBuffer[4];
    unsigned char bufferInfo[5];    
    unsigned char checkSum 		= 0;
    unsigned char readBuffer 	= 0;
    int   result		= 0;
    int errorTryAgain =0;
    *bufferBytes = 0;
    *freeBytes   = 0;
    
    // build header
    packetBuffer[0] = DC2;			// packet header
    packetBuffer[1] = 0x01; 		// temp len, calculated later	
    packetBuffer[2] = 'I';			// first command char

    checkSum = calculatePacketChecksum( packetBuffer, 3 );
    packetBuffer[3] = checkSum;
    
    // write the packet to serial port
    result = SerialWriteTo(devPtr,packetBuffer, 4);
    if( result != 4 ){
    	printf("[getBufferInformation]: Error while writing packet\n");   
    	
    	// try again three times
    	for(errorTryAgain=0;errorTryAgain<3;errorTryAgain++)
    	{
    		result = SerialWriteTo(devPtr,packetBuffer, 4);
    		if(result != 4)
    		{
    			printf("[getBufferInformation]: Error while writing packet try %d\n",errorTryAgain);   
    			continue;
    		}
    		else
    		{
    			break;
    		}
    		return -1;
    	}    	 	    	
    	
    }
    
    // read the ACK
    result = SerialReadFrom(devPtr,((unsigned char*) &readBuffer), 1);
    
    if( result != 1 ){
    	printf("[getBufferInformation]: Error while reading ACK\n res=%d",result);
    	return -1;
    }
    
    if( readBuffer == 0x06 ){
    	
    	// now we should receive the buffer informations
    	result = SerialReadFrom(devPtr, bufferInfo, 5);
    	
    	if( result != 5 ){
    		printf("[getBufferInformation]: Error while reading buffer information\n");
    		return -1;
    	}
    	
    	if( bufferInfo[0] != DC2 ){
    			return -1;
    	}
    	  	    	
    	*bufferBytes = bufferInfo[2];
    	*freeBytes = bufferInfo[3];
    	
    	return 0;
    }
    	
    return -1;	    
}

/**********************************************************************************************
/	Function        : sendCommand(FILE_PTR devPtr,char* cmdStr, char* cmdParam)
/----------------------------------------------------------------------------------------------
/	Description	:  sendet den cmdStr, mit den gegebenen Parametern cmdParam an das Display,
/				   via RS232.
/				   Diese funktion wird von allen Nachfolgenden verwendet um commandos 
/				   an das Display zu senden.
/
/	Input Parameter : devPtr pointer zur Seriellen Schnittstelle, cmdStr, und cmdParam
/                               
/	Output Parameter: -1 bei Fehler
/					  readBuffer bei Erfolg
/	Written by	: Andreas Schmidt
/   Date        : 25.11.2006
/----------------------------------------------------------------------------------------------
/**********************************************************************************************/
int sendCommand(FILE_PTR devPtr,char* cmdStr, char* cmdParam) {
    
    	unsigned char* 	packetBuffer 	= NULL;
    	char*	paramPtr 	 	= NULL;	
    	char*   paramEndPtr		= NULL;
    	
    	char	paramBuf[128];
    	
    	int		paramCharCount	= -1;
    	int		paramType		= -1;
    	int		paramCount		= 0;	
    	int 	payloadLen		= 0;
    	short	shortParam		= 0;
    	unsigned char	byteParam		= 0;
    	char	stringParam		= 0;
    	int		i				= 0;
    	unsigned char	checkSum		= 0;
    	int		totalCount		= 0;
    	char	readBuffer		= 0;
    	int		result			= 0;
    	int     errorTryAgain   = 0;
    	char * buf;
    	
    	packetBuffer = (unsigned char*)malloc(sizeof(unsigned char) * MAX_PACKET_SIZE);
    	  
    	if( packetBuffer == NULL )
    	{
    		printf("[SendCommand]: Out of memory\n");
    		return -1;
    	}
    	
    	// build header
    	packetBuffer[0] = DC1;			// packet header
    	packetBuffer[1] = 0x00; 		// temp len, calculated later
    	packetBuffer[2] = 0x1B;			// ESC charater to indicate command
    	packetBuffer[3] = (unsigned char)cmdStr[0];	// first command char
    	packetBuffer[4] = (unsigned char)cmdStr[1];	// second command char
    	
    	paramPtr    = cmdParam;
    	paramEndPtr = &cmdParam[strlen(cmdParam)];
    	
    	// parse the command parameter
    	while( paramPtr != paramEndPtr )
    	{
    		if( paramType != NO_PARAM )
    		{
    			if( *paramPtr != ',' )
    			{
    				paramBuf[paramCharCount] = *paramPtr;
    				paramCharCount++;
    			}
    			else
    			{
    				paramBuf[paramCharCount] = '\0';
    							
    				switch( paramType )
    				{
    					case SHORT_PARAM:
    						shortParam = (short)(atoi(paramBuf) & 0x0000FFFF);
    						packetBuffer[PARAMETER_START + paramCount++] = (unsigned char)(shortParam & 0x00FF);
    						packetBuffer[PARAMETER_START + paramCount++] = (unsigned char)((shortParam & 0xFF00) >> 8);
    						break;
    					case BYTE_PARAM:
    						byteParam = (unsigned char)(atoi(paramBuf) & 0x000000FF);
    						packetBuffer[PARAMETER_START + paramCount++] = (byteParam);						
    						break;
    					case STRING_PARAM:
    						for( i = 0; i < paramCharCount; i++ )
    						{
    							packetBuffer[PARAMETER_START + paramCount++] = (unsigned char)paramBuf[i];
    						}
    						packetBuffer[PARAMETER_START + paramCount++] = '\0';
    						break;
    				}
    				
    				paramCharCount = -1;
    				paramType = NO_PARAM;
    			}
    		}
    	
    		if( *paramPtr == 'b' && paramCharCount < 0 )
    		{
    			paramType = BYTE_PARAM;
    			paramCharCount = 0;
    		}
    		else if( *paramPtr == 's' && paramCharCount < 0 )
    		{
    			paramType = SHORT_PARAM;
    			paramCharCount = 0;
    		}
    		else if( *paramPtr == 't' && paramCharCount < 0 )
    		{
    			paramType = STRING_PARAM;
    			paramCharCount = 0;
    		}
    		
    		paramPtr++;				
    	}
    	
    	// now set the payload length
    	packetBuffer[1] = (unsigned char)(paramCount + strlen(cmdStr) + 1);
    	
    	checkSum = calculatePacketChecksum(packetBuffer, PARAMETER_START + paramCount);
    	packetBuffer[PARAMETER_START + paramCount] = checkSum;
    	
    	totalCount = PARAMETER_START + paramCount + 1;
    	
    	result = SerialWriteTo(devPtr,packetBuffer, totalCount);
    	
    	if( result != totalCount )
    	{
    		printf("[sendCommand]: Error while writing packet\n"); 			
    		_mem_free(packetBuffer);
    		return -1;
 	   		
    	}

    	// read the ACK
    	result = SerialReadFrom(devPtr,((unsigned char*) &readBuffer), 1);
    	
    	if( result != 1 )
    	{
    		printf("[sendCommand]: Error while reading ACK\n");
    		 
    		//delete packetBuffer; 
    		_mem_free(packetBuffer);
    		return -1;
    	}
    	
    	//delete packetBuffer; 
    	_mem_free(packetBuffer);
    	//seems unnecessary..
    	
    	return readBuffer;	
    //#]
}

/**********************************************************************************************
/	Function    : sendXX(....)
/----------------------------------------------------------------------------------------------
/	Description	: Befehlstabelle EA eDIP320_8
/
/
/
/
/	Written by	: Andreas Schmidt
/   Date        : 25.11.2006
/----------------------------------------------------------------------------------------------
/**********************************************************************************************/
int sendAA(FILE_PTR devPtr,char val) {
    
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", val);    
    result = sendCommand(devPtr, "AA", cmdBuffer);
        
    return result;
}

int sendAB(FILE_PTR devPtr,char no) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", no);    
    result = sendCommand( devPtr,"AB", cmdBuffer);
    
    return result;
}

int sendAD(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char linewidth) {

    char cmdBuffer[26];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,", x1, y1, x2, y2, linewidth);    
    result = sendCommand(devPtr, "AD", cmdBuffer);
    
    return result;
}

int sendAE(FILE_PTR devPtr,char border) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", border);    
    result = sendCommand(devPtr, "AE", cmdBuffer);
    
    return result;
}

int sendAF(FILE_PTR devPtr,char no) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", no);    
    result = sendCommand(devPtr, "AF", cmdBuffer);
    
    return result; 
}

int sendAG(FILE_PTR devPtr,char no) {

    char cmdBuffer[6];
    int result = 0;    
    
    sprintf(cmdBuffer, "b%d,", no);    
    result = sendCommand(devPtr, "AG", cmdBuffer);
    
    return result;    
}

int sendAH(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {

   	char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);    
    result = sendCommand( devPtr, "AH", cmdBuffer);
    
    return result;
}

int sendAI(FILE_PTR devPtr,char val) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", val);    
    result = sendCommand(devPtr, "AI", cmdBuffer);
    
    return result;
}

int sendAK(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char downcode, char upcode, char* text) {
  
    char* cmdBuffer = NULL;
   	int result = 0;
    	
   	cmdBuffer = (char*)_mem_alloc(sizeof(char) * (strlen(text) + 31));       	
   	sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,b%d,t%s,", x1, y1, x2, y2, downcode, upcode, text);    	    	
    result = sendCommand(devPtr, "AK", cmdBuffer);		    	     
    _mem_free(cmdBuffer);
    	
    return result;    
}

int sendAL(FILE_PTR devPtr,char code, char val) {

    char cmdBuffer[6];
    int result = 0;
        
    sprintf(cmdBuffer, "b%d,b%d,", code, val);    
    result = sendCommand(devPtr, "AL", cmdBuffer);
    
    return result; 
}

int sendAN(FILE_PTR devPtr,char code) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", code);    
    result = sendCommand(devPtr, "AN", cmdBuffer);
    
    return result; 
}

int sendAO(FILE_PTR devPtr,char val) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", val);    
    result = sendCommand(devPtr, "AO", cmdBuffer);
    
    return result;
}

int sendAP(FILE_PTR devPtr,char code, char val) {

   	char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,b%d,", code, val);    
    result = sendCommand(devPtr, "AP", cmdBuffer);
    
    return result;
}

int sendAQ(FILE_PTR devPtr,char val) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", val);    
    result = sendCommand( devPtr,"AQ", cmdBuffer);
    
    return result;
}

int sendAR(FILE_PTR devPtr,char no) {

    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", no);    
    result = sendCommand(devPtr, "AR", cmdBuffer);
    
    return result;
}

int sendAS(FILE_PTR devPtr,char val) {
    //#[ operation sendAS(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", val);
    
    result = sendCommand(devPtr, "AS", cmdBuffer);
    
    return result;
    //#]
}

int sendAT(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char downcode, char upcode, char* text) {
    //#[ operation sendAT(short,short,short,short,char,char,char*) 
    char* cmdBuffer = NULL;
    
    int result = 0;
    
    cmdBuffer = (char*) _mem_alloc(sizeof(char) * (strlen(text) + 31)); 
    //cmdBuffer = new char[(strlen(text) + 31)];
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,b%d,t%s,", x1, y1, x2, y2, downcode, upcode, text);
    //printf("cmdBuffer : %s\n",cmdBuffer);
    
    result = sendCommand(devPtr, "AT", cmdBuffer);		
    
    
    _mem_free(cmdBuffer);
    
    return result; 
    //#]
}

int sendAV(FILE_PTR devPtr,short x, short y, char val) {
    //#[ operation sendAV(short,short,char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,b%d,", x, y, val);
    
    result = sendCommand(devPtr, "AV", cmdBuffer);
    
    return result;
    //#]
}

int sendAW(FILE_PTR devPtr,char val) {
    //#[ operation sendAW(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", val);
    
    result = sendCommand(devPtr, "AW", cmdBuffer);
    
    return result;
    //#]
}

int sendAX(FILE_PTR devPtr,char code) {
    //#[ operation sendAX(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", code);
    
    result = sendCommand(devPtr, "AX", cmdBuffer);
    
    return result;
    //#]
}

int sendAY(FILE_PTR devPtr,char val) {
    //#[ operation sendAY(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", val);
    
    result = sendCommand(devPtr, "AY", cmdBuffer);
    
    return result;
    //#]
}

int sendAZ(FILE_PTR devPtr,char val1, char val2) {
    //#[ operation sendAZ(char,char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,b%d,", val1, val2);
    
    result = sendCommand(devPtr, "AZ", cmdBuffer);
    
    return result;
    //#]
}

int sendBA(FILE_PTR devPtr,char no, char val) {
    //#[ operation sendBA(char,char) 
    char cmdBuffer[11];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,b%d,", no, val);
    
    result = sendCommand( devPtr,"BA", cmdBuffer);
    
    return result;
    //#]
}

int sendBD(FILE_PTR devPtr,char no, char action) {
    //#[ operation sendBD(char,char) 
    char cmdBuffer[11];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,b%d,", no, action);
    
    result = sendCommand(devPtr, "BD", cmdBuffer);
    
    return result;
    //#]
}

int sendBL(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst) {
    //#[ operation sendBL(char,short,short,short,short,char,char,char,char) 
    int result = 0;
    
    result = sendBRLOU( devPtr,'L', no, x1, y1, x2, y2, aw, ew, type, mst);
    
    return result;
    //#]
}

int sendBO(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst) {
    //#[ operation sendBO(char,short,short,short,short,char,char,char,char) 
    int result = 0;
    
    result = sendBRLOU(devPtr, 'O', no, x1, y1, x2, y2, aw, ew, type, mst);
    
    return result;
    //#]
}

int sendBR(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst) {
    //#[ operation sendBR(char,short,short,short,short,char,char,char,char) 
    int result = 0;
    
    result = sendBRLOU(devPtr, 'R', no, x1, y1, x2, y2, aw, ew, type, mst);
    
    return result;
    //#]
}

int sendBRLOU(FILE_PTR devPtr,char barType, char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst) {
    //#[ operation sendBRLOU(char,char,short,short,short,short,char,char,char,char) 
        char cmdBuffer[46];
    	char cmdStr[3];
    	int result = 0;
    	
    	sprintf(cmdBuffer, "b%d,s%d,s%d,s%d,s%d,b%d,b%d,b%d,b%d,", no, x1, y1, x2, y2, aw, ew, type, mst);
    	
    	cmdStr[0] = 'B';
    	cmdStr[1] = barType;
    	cmdStr[2] = '\0';
    	
    	result = sendCommand( devPtr,cmdStr, cmdBuffer);
    	
    	return result;
    //#]
}

int sendBS(FILE_PTR devPtr,char no) {
    //#[ operation sendBS(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", no);
    
    result = sendCommand(devPtr, "BS", cmdBuffer);
    
    return result; 
    //#]
}

int sendBU(FILE_PTR devPtr,char no, short x1, short y1, short x2, short y2, char aw, char ew, char type, char mst) {
    //#[ operation sendBU(char,short,short,short,short,char,char,char,char) 
    int result = 0;
    
    result = sendBRLOU(devPtr, 'U', no, x1, y1, x2, y2, aw, ew, type, mst);
    
    return result;
    //#]
}

int sendBZ(FILE_PTR devPtr,char no) {
    //#[ operation sendBZ(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", no);
    
    result = sendCommand(devPtr, "BZ", cmdBuffer);
    
    return result;
    //#]
}

int sendDA(FILE_PTR devPtr) {
    //#[ operation sendDA() 
    	int result = sendCommand(devPtr, "DA", "");
    
    return result;
    //#]
}

int sendDE(FILE_PTR devPtr) {
    //#[ operation sendDE() 
    int result = sendCommand( devPtr,"DE", "");
    
    return result;
    //#]
}

int sendDI(FILE_PTR devPtr) {
    //#[ operation sendDI() 
    int result = sendCommand(devPtr, "DI", "");
    
    return result;
    //#]
}

int sendDL(FILE_PTR devPtr) {
    //#[ operation sendDL() 
    	int result = sendCommand(devPtr, "DL", "");
    
    return result;
    //#]
}

int sendTA(FILE_PTR devPtr) {
    //#[ operation sendDL() 
    int result = sendCommand(devPtr, "TA", "");    
    return result;
    //#]
}

int sendDS(FILE_PTR devPtr) {
    //#[ operation sendDS() 
    int result = sendCommand(devPtr, "DS", "");
    
    return result;
    //#]
}

int sendGD(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {
    //#[ operation sendGD(short,short,short,short) 
    char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);
    
    result = sendCommand(devPtr, "GD", cmdBuffer);
    
    return result;
    //#]
}

int sendGM(FILE_PTR devPtr,char pattern) {
    //#[ operation sendGM(char) 
    	char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", pattern);
    
    result = sendCommand(devPtr, "GM", cmdBuffer);
    
    return result;
    //#]
}

int sendGP(FILE_PTR devPtr,short x1, short y1) {
    //#[ operation sendGP(short,short) 
    char cmdBuffer[11];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,", x1, y1);
    
    result = sendCommand(devPtr, "GP", cmdBuffer);
    
    return result;
    //#]
}

int sendGR(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {
    //#[ operation sendGR(short,short,short,short) 
    char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);
    
    result = sendCommand(devPtr,"GR", cmdBuffer);
    
    return result;
    //#]
}

int sendGV(FILE_PTR devPtr,char mode) {
    //#[ operation sendGV(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", mode);
    
    result = sendCommand(devPtr, "GV", cmdBuffer);
    
    return result;
    //#]
}

int sendGW(FILE_PTR devPtr,short x1, short y1) {
    //#[ operation sendGW(short,short) 
    char cmdBuffer[11];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,", x1, y1);
    
    result = sendCommand(devPtr, "GW", cmdBuffer);
    
    return result;
    //#]
}

int sendGZ(FILE_PTR devPtr,char sizeX, char sizeY) {
    //#[ operation sendGZ(char,char) 
    char cmdBuffer[11];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,b%d,", sizeX, sizeY);
    
    result = sendCommand(devPtr, "GZ", cmdBuffer);
    
    return result;
    //#]
}

int sendQI(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {
    //#[ operation sendQI(short,short,short,short) 
    char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);
    
    result = sendCommand(devPtr, "QI", cmdBuffer);
    
    return result;
    //#]
}

int sendQL(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {
    //#[ operation sendQL(short,short,short,short) 
    	char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);
    
    result = sendCommand(devPtr, "QL", cmdBuffer);
    
    return result; 
    //#]
}

int sendQM(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern) {
    //#[ operation sendQM(short,short,short,short,char) 
    char cmdBuffer[26];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,", x1, y1, x2, y2, pattern);
    
    result = sendCommand(devPtr, "QM", cmdBuffer);
    
    return result; 
    //#]
}

int sendQZ(FILE_PTR devPtr,char time) {
    //#[ operation sendQZ(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", time);
    
    result = sendCommand(devPtr, "QZ", cmdBuffer);
    
    return result;
    //#]
}

int sendRI(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {
    //#[ operation sendRI(short,short,short,short) 
    char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);
    
    result = sendCommand(devPtr, "RI", cmdBuffer);
    
    return result;
    //#]
}

int sendRL(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {
    //#[ operation sendRL(short,short,short,short) 
    char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);
    
    result = sendCommand(devPtr, "RL", cmdBuffer);
    
    return result;
    //#]
}

int sendRM(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern) {
    //#[ operation sendRM(short,short,short,short,char) 
    char cmdBuffer[26];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,", x1, y1, x2, y2, pattern);
    
    result = sendCommand( devPtr,"RM", cmdBuffer);
    
    return result; 
    //#]
}

int sendRO(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern) {
    //#[ operation sendRO(short,short,short,short,char) 
    	char cmdBuffer[26];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,", x1, y1, x2, y2, pattern);
    
    result = sendCommand(devPtr, "RO", cmdBuffer);
    
    return result; 
    //#]
}

int sendRR(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern) {
    //#[ operation sendRR(short,short,short,short,char) 
    char cmdBuffer[26];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,", x1, y1, x2, y2, pattern);
    
    result = sendCommand(devPtr, "RR", cmdBuffer);
    
    return result;
    //#]
}

int sendRS(FILE_PTR devPtr,short x1, short y1, short x2, short y2) {
    //#[ operation sendRS(short,short,short,short) 
    char cmdBuffer[21];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,", x1, y1, x2, y2);
    
    result = sendCommand(devPtr, "RS", cmdBuffer);
    
    return result;
    //#]
}

int sendRT(FILE_PTR devPtr,short x1, short y1, short x2, short y2, char pattern) {
    //#[ operation sendRT(short,short,short,short,char) 
    char cmdBuffer[26];
    int result = 0;
    
    sprintf(cmdBuffer, "s%d,s%d,s%d,s%d,b%d,", x1, y1, x2, y2, pattern);
    
    result = sendCommand(devPtr, "RT", cmdBuffer);
    
    return result;
    //#]
}

int sendZB(FILE_PTR devPtr,char blinkMode) {
    //#[ operation sendZB(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", blinkMode);
    
    result = sendCommand(devPtr,"ZB", cmdBuffer);
    
    return result;
    //#]
}

int sendZC(FILE_PTR devPtr,short x, short y, char* text) {
    //#[ operation sendZC(short,short,char*) 
    return sendZLCR(devPtr,'C', x, y, text);
    //#]
}

int sendZF(FILE_PTR devPtr,char fontNo) {
    //#[ operation sendZF(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", fontNo);
    
    result = sendCommand(devPtr,"ZF", cmdBuffer);
    
    return result;
    //#]
}

int sendZL(FILE_PTR devPtr,short x, short y, char* text) {
    //#[ operation sendZL(short,short,char*) 
    return sendZLCR(devPtr,'L', x, y, text);
    //#]
}



int sendYL(FILE_PTR devPtr,char mode) {
    //#[ operation sendZF(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", mode);
    
    result = sendCommand(devPtr,"YL", cmdBuffer);
    
    return result;
    //#]
}


int sendZLCR(FILE_PTR devPtr,char dir, short x, short y, char* text) {
    //#[ operation sendZLCR(char,short,short,char*) 
    	char* cmdBuffer = NULL;
    	char  cmdStr[3];
    	
    	int result = 0;
    	
    	cmdBuffer = (char*)_mem_alloc(sizeof(char) * (strlen(text) + 15));
    //	cmdBuffer = new char[(strlen(text) + 15)];
    	sprintf(cmdBuffer, "s%d,s%d,t%s,", x, y, text);
    	
    	cmdStr[0] = 'Z';
    	cmdStr[1] = dir;
    	cmdStr[2] = '\0';
    	
    	result = sendCommand(devPtr,cmdStr, cmdBuffer);		
    	
    	_mem_free(cmdBuffer);
    	
    	
    	return result;
    //#]
}

int sendZM(FILE_PTR devPtr,char pattern) {
    //#[ operation sendZM(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", pattern);
    
    result = sendCommand(devPtr,"ZM", cmdBuffer);
    
    return result;
    //#]
}

int sendZR(FILE_PTR devPtr,short x, short y, char* text) {
    //#[ operation sendZR(short,short,char*) 
    return sendZLCR(devPtr,'R', x, y, text);
    //#]
}

int sendZV(FILE_PTR devPtr,char mode) {
    //#[ operation sendZV(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", mode);
    
    result = sendCommand(devPtr,"ZV", cmdBuffer);
    
    return result;
    //#]
}

int sendZW(FILE_PTR devPtr,char angel) {
    //#[ operation sendZW(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", angel);
    
    result = sendCommand(devPtr,"ZW", cmdBuffer);
    
    return result;
    //#]
}

int sendZY(FILE_PTR devPtr,char dist) {
    //#[ operation sendZY(char) 
    char cmdBuffer[6];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,", dist);
    
    printf("sendZY sends ZZ ? oO\n");
    result = sendCommand(devPtr,"ZZ", cmdBuffer);
    
    return result;
    //#]
}

int sendZZ(FILE_PTR devPtr,char zoomX, char zoomY) {
    //#[ operation sendZZ(char,char) 
    char cmdBuffer[13];
    int result = 0;
    
    sprintf(cmdBuffer, "b%d,b%d,", zoomX, zoomY);
    
    result = sendCommand(devPtr,"ZZ", cmdBuffer);
    
    return result;
    //#]
}

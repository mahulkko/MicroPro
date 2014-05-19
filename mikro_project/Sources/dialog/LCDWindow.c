/*************************************************************************
/	Module		: Zugriffsfkt. LCDWindow 
/-------------------------------------------------------------------------
/	Description	: Zugriffsfunktionen auf den EAeDIP320_8 DisplayTreiber 
/				  Version 1.5.
/				  
/				 Funktionsbeschreibung, siehe .h 
/-------------------------------------------------------------------------
/	Department	: HTWG Konstanz, Studiengang Techn. Informatik
/	Project		: Display Treiber EAeDIP320_8.c
/	Version		: 1.0
/	Released	: 14.04.2010
/	Written by	: Jan Radon
/************************************************************************/


#include "LCDWindow.h"
#include "SerialMQX.h"
#include "LCD_EAeDIP320_8.h"



FILE_PTR Init(FILE_PTR devPtr) {
	
	if(devPtr == NULL){
		devPtr = SerialOpen("ttyd:",115200);                
	}                                     	
    displayLight(devPtr,1);
    return devPtr;            
}

void cleanDisplay(FILE_PTR devPtr)
{		
    int i = 1;   
	while(sendTA(devPtr)==21);					//Terminalanzeige ausgeschaltet			   	
    while(sendDL(devPtr)==21);					//löscht Grafikbildschirm (nicht Terminalfenster)             
}

// mode 0 off 1 on
void displayLight(FILE_PTR devPtr,int mode)
{
	while(sendYL(devPtr,mode)==21);	
}

void displayOff(FILE_PTR devPtr)
{
	while(sendDA(devPtr)==21);		
}
void displayOn(FILE_PTR devPtr)
{
	while(sendDE(devPtr)==21);	
}

// lock item
void lockItem(FILE_PTR devPtr, struct item* item)
{	
	while(sendAL(devPtr, item->upCode, 0)==21);
	while(sendAA(devPtr,1)==21);	
}

void clearDisplay(FILE_PTR devPtr)
{
	while(sendRL(devPtr,0,0,319,239));
}

void hideItem(FILE_PTR devPtr, struct item* item)
{		
	if(item->type == LABEL)
	{	// loeschbox um 16px nach oben verschieben
		while(sendRL(devPtr,item->x, item->y - 16, item->width, item->height)==21);		
	}
	else
	{
		while(sendRL(devPtr,item->x, item->y, item->width, item->height)==21);			
	}	
}

void drawFctLabel(FILE_PTR devPtr,struct item* currentLab)
{

	while(sendZF(devPtr,currentLab->font) == 21);
	while(sendZL(devPtr, currentLab->x, currentLab->y, currentLab->text)==21);
//	printf("currentLab->text: %s\n",currentLab->text)	;
}

void drawFctBox(FILE_PTR devPtr,struct item* currentLab)
{
	while(sendGR(devPtr, currentLab->x,  currentLab->y,currentLab->width,currentLab->height)==21);	
}

void drawFctToggleButton(FILE_PTR devPtr,struct item* current)
{	
	while(sendAK(devPtr, current->width,current->height, current->x, current->y,  current->upCode, current->downCode, current->text)==21);    	
	if(current->enabled!=1)
	{				
    	while(sendAL(devPtr,current->upCode, 0)==21);    	    	
	}
	
    while(sendAA(devPtr,1)==21);
    	
}
void drawFctButton(FILE_PTR devPtr,struct item* current)
{	
	while(sendAT(devPtr, current->width,current->height, current->x, current->y, current->upCode, current->downCode, current->text)==21);    		
	if(current->enabled!=1){				
    	while(sendAL(devPtr,current->upCode, 0)==21); 	    	
	}
	while(sendAA(devPtr,1)==21);    	
}


struct item* createButton(int x, int y, int height,int width,char* text,int upCode,int downCode, void* handlefct,int toggle,int enabled)
{
	struct item *btn1;		
	btn1 = (struct item*) _mem_alloc(sizeof(struct item));
    btn1->x = x;
    btn1->y = y;
    btn1->width = btn1->x+ width;
    btn1->height = btn1->y +height;
    btn1->text =  _mem_alloc(strlen(text));
    strcpy(btn1->text,text);
    btn1->upCode = upCode;
    btn1->downCode = downCode;    
    btn1->handle = handlefct;
    btn1->enabled = enabled;
    btn1->type = BUTTON;
    btn1->toggle = toggle;
    if(toggle == 1){
    	btn1->drawfct = *drawFctToggleButton;
    }    	
    else{
    	btn1->drawfct = *drawFctButton;
    }
    	
    return btn1;
}

struct item* createBox(int x,int y,int width,int height)
{
	struct item *box;	
    box= (struct item*) _mem_alloc(sizeof(struct item));
    box->x=x;
    box->y=y;
    box->width = box->x + width; 
    box->height = box->y + height;
    box->text = NULL;    
    box->drawfct = *drawFctBox;
    box->type = BOX;	
	return box;	
}

struct item* createLabel(int x, int y, char* text, int font) {
	
	struct item *lab1;	
    lab1 = (struct item*) _mem_alloc(sizeof(struct item));
    lab1->x=x;
    lab1->y=y;
    lab1->width = lab1->x+ (strlen(text) * (8 + font)); //8pts per pixel using font (this is just a guess) .
    lab1->height = lab1->y+30;
    lab1->text = _mem_alloc(strlen(text));    
    lab1->drawfct = *drawFctLabel;
    lab1->type = LABEL;
    lab1->font = font;
	strcpy(lab1->text,text);    			
	return lab1;
}

void itemChangeText(struct item* label,char* text)
{
	_mem_free(label->text);
	label->text = _mem_alloc(strlen(text));	
	strcpy(label->text,text);	
}

void Close(FILE_PTR devPtr)
{
	fclose(devPtr);	
}

void show(struct item *itemList[],FILE_PTR devPtr,int size)
{
	int i = 0;
	for(i=0;i<size;i++)
	{					
		itemList[i]->drawfct(devPtr,itemList[i]);	
	}
}

int display_read_task(struct item *itemList[], FILE_PTR devPtr,int size )
{
    int validup = -1;
    int validdown = -1;
    int i = 0;
    int op =-1;    															
    int  bufferBytes;
    int  receiveBufferFree;    
    
    while(getBufferInformation(devPtr,&bufferBytes, &receiveBufferFree)==-1);  //Pufferinformationen anfordern

    if(bufferBytes > 0)
    {	    
    	op = getBuffer(devPtr);		//Pufferinhalt anfordern	    	
    	for(i=0;i<size;i++)
    	{    		
    		if(itemList[i]->type != BUTTON) continue;
    				
			validup = itemList[i]->upCode;
			validdown = itemList[i]->downCode;
			if(validup == op ||validdown == op){												
				itemList[i]->handle(devPtr,op);	 // call fctpointer
				return 1;
			}			
		}	    
    } 
    return 0;   
}








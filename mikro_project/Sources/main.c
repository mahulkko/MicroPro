/*************************************************************************
/	Module		: Zugriffsfkt. Testanwendung LCD DISPLAY EAeDIP320_8 
/-------------------------------------------------------------------------
/	Description	: Zugriffsfunktionen auf den EAeDIP320_8 DisplayTreiber 
/				  Version 1.5 und des LCDWindow v. 1.0
/				  
/
/-------------------------------------------------------------------------
/	Department	: HTWG Konstanz, Studiengang Techn. Informatik
/	Project		: TWR K60 N512 Mikroprozessorsysteme-Labor
/	Version		: 2.0
/	Released	: 01.04.2014
/	Written by	: Jan Radon
/   Edited by	: Manuel Beha
/************************************************************************/

#include "main.h"
#include "network.h"
#include "dialog/LCDWindow.h"

// Verfuegbare DISPLAY ITEMS.
#define BTN_IDLE_STATE 0
#define BTN_NEXT_STATE 1
#define LAB_IDLE_STATE1 2
#define LAB_IDLE_STATE2 3
#define BTN_QUIT_STATE 4

#define BTN_COIN_5 5
#define BTN_COIN_10 6
#define BTN_COIN_20 7
#define BTN_COIN_50 8
#define BTN_COIN_100 9

#define LAB_NEXT_STATE1 10
#define LAB_QUIT_STATE1 11
#define LAB_PAY_STATE1 12
#define BTN_CANCEL_PAY 13

// DEFINIERTE STATES.
#define IDLE_STATE 100
#define NEXT_STATE 101
#define COIN_STATE 102
#define UNINITIALIZED 103 //initial

#define rs232TouchDelay 600 // delay in ms.

int prevState; // prev and next state.
int nextState;


TASK_TEMPLATE_STRUCT MQX_template_list[] =
{
/*  Task number, Entry point, Stack, Pri, String, Auto? */
   {MAIN_TASK,   Main_task,   2000,  9,   "main", 		MQX_AUTO_START_TASK},
   {0,           0,           0,     0,   0,      0,                       }
};


struct item* myItems[14]; // all items.
int myItemsSize = 14;
struct item* myIdleState[10]; // in idle state visible items
int idleStateSize; 			  // size of array

struct item* myNextState[5]; // in next state visible items
int nextStateSize;			  // size of array

struct item* myQuitState[1];// in quit state visible items
int quitStateSize;  // size of array


struct item* myCoinState[7];// in coin state visible items
int coinStateSize;  // size of array


// "versteckt" im folgezustand nicht sichtbare Items.
void hideStateItems(FILE_PTR devPtr)
{
	int i;		
	if(nextState == prevState) return;
	
	switch(prevState)
	{
		case IDLE_STATE:			
			//printf("hide IDLE STATE \n");
			for(i=0;i<idleStateSize;i++)
				{
					if(myIdleState[i]->type == BUTTON)
					{
						lockItem(devPtr,myIdleState[i]); // sperren des Items fuer Ereignisse
					}
					hideItem(devPtr,myIdleState[i]);	// display bereich loeschen
				}
			break;
		case NEXT_STATE:
			//printf("hide NEXT STATE \n");
			for(i=0;i<nextStateSize;i++)
			{		
				if(myNextState[i]->type == BUTTON)
				{
					lockItem(devPtr,myNextState[i]);
				}
				hideItem(devPtr,myNextState[i]);
		
			}
		break;
		case COIN_STATE:
			//printf("hide COIN STATE \n");
			for(i=0;i<coinStateSize;i++)
			{
				if(myCoinState[i]->type == BUTTON)
				{
					lockItem(devPtr,myCoinState[i]);
				}
				hideItem(devPtr,myCoinState[i]);		
			}
		break;
		default:
		break;					
	}									
}


// EVENT Handler (funktionspointer) 
void transitionNextState(FILE_PTR devPtr,int op)
{
	// state specific
    int found = 0;
    //printf("go to next state\n");     
    nextState = NEXT_STATE;	
 
 	displayOff(devPtr);
    hideStateItems(devPtr);
   
    cleanDisplay(devPtr); 		
    myNextState[0]->enabled = 1;	
    myNextState[1]->enabled = 0;
    show(myNextState,devPtr,nextStateSize);					   
    prevState = NEXT_STATE;
	displayOn(devPtr);
	
   
   //	printf("exit transition next \n");
   	     				   
}
// EVENT Handler (funktionspointer) 
void transitionIdleState(FILE_PTR devPtr,int op)
{	
	// state specific
	int found = 0;
	//printf("go to idle State\n");
	nextState = IDLE_STATE;	
	displayOff(devPtr);
	hideStateItems(devPtr);
	
//	cleanDisplay(devPtr); 	
	myIdleState[0]->enabled = 0;	
	myIdleState[1]->enabled = 1;
	show(myIdleState,devPtr,idleStateSize);						
	prevState = IDLE_STATE;
	displayOn(devPtr);
	
  
 //  	printf("exit transition idle \n");
}

// EVENT Handler (funktionspointer) 
void transitionCoinState(FILE_PTR devPtr,int op)
{	
	// state specific
	int found = 0;
	char buffer[12];
//	printf("go to coin State\n");
	nextState = COIN_STATE;	
	
	// handle op code.
 	sprintf((char*)&buffer, "opCode: %d,", op);		
	itemChangeText(myItems[LAB_PAY_STATE1],(char*)&buffer);
	
	if(nextState!=prevState)
	{	
		displayOff(devPtr);	
		//printf("texT: %s\n",myItems[LAB_PAY_STATE1]->text);
		hideStateItems(devPtr);
		//	cleanDisplay(devPtr); 	
		show(myCoinState,devPtr,coinStateSize);					
		prevState = COIN_STATE;		
		displayOn(devPtr);
	}
	else
	{
		drawFctLabel(devPtr,myItems[LAB_PAY_STATE1]);	
	}
	 	 				
}

// EVENT Handler (funktionspointer) 

void transitionQuitState(FILE_PTR devPtr,int op)
{	
	// state specific
	int i=0;
	//printf("go to Quit State\n");	
	displayOff(devPtr);
	cleanDisplay(devPtr); 		
	show(myQuitState,devPtr,quitStateSize);							
	displayOn(devPtr);
	
	// clean up	
	for(i=0;i<idleStateSize;i++)
	{
		if(myIdleState[i] != NULL)
		{
			_mem_free(myIdleState[i]->text);
			_mem_free(myIdleState[i]);
			myIdleState[i] = NULL;	
					
		}
		
	}
	for(i=0;i<nextStateSize;i++)
	{
		if(myNextState[i] != NULL)
		{
			_mem_free(myNextState[i]->text);
			_mem_free(myNextState[i]);
			myNextState[i] = NULL;			
		}
		
	}
	for(i=0;i<quitStateSize;i++)
	{
		if(myQuitState[i] != NULL)
		{
			_mem_free(myQuitState[i]->text);
			_mem_free(myQuitState[i]);
			myQuitState[i] = NULL;			
		}
		
	}
	for(i=0;i<coinStateSize;i++)
	{
		if(myCoinState[i] != NULL)
		{
			_mem_free(myCoinState[i]->text);
			_mem_free(myCoinState[i]);
			myCoinState[i] = NULL;			
		}
		
		
	}
	// wait 5 sec and exit
	_time_delay(5000);   
	displayOff(devPtr);
	displayLight(devPtr,0);
	Close(devPtr);
	_mqx_exit(0);
	
}

// Initialisieren aller Items
void InitializeItems()
{
	myItems[BTN_IDLE_STATE] = createButton(0,0,30,100,"Idle State",11,11,*transitionIdleState,0,0);
    myItems[BTN_NEXT_STATE] = createButton(0,35,30,100,"Next State",22,22,*transitionNextState,0,1);  
    myItems[LAB_IDLE_STATE1] = createLabel(120,30,"Idle State",3);
	myItems[LAB_IDLE_STATE2] = createLabel(120,70,".. press a button....",3);				
	myItems[BTN_QUIT_STATE] = createButton(0,70,30,100,"QUIT",33,33,*transitionQuitState,0,1);
	
	myItems[BTN_COIN_5] = createButton(10,190,30,50,"5",44,44,*transitionCoinState,0,1);
	myItems[BTN_COIN_10] = createButton(65,190,30,50,"10",55,55,*transitionCoinState,0,1);
	myItems[BTN_COIN_20] = createButton(120,190,30,50,"20",66,66,*transitionCoinState,0,1);
	myItems[BTN_COIN_50] = createButton(175,190,30,50,"50",77,77,*transitionCoinState,0,1);
	myItems[BTN_COIN_100] = createButton(230,190,30,50,"100",88,88,*transitionCoinState,0,1);	
	
	myItems[LAB_NEXT_STATE1] = createLabel(120,30,"Next State",3);
	myItems[LAB_QUIT_STATE1] = createLabel(120,70,"... BYE BYE ...",3);		
	myItems[LAB_PAY_STATE1] = createLabel(130,70,"... PAY...",3);				
	myItems[BTN_CANCEL_PAY] = createButton(0,95,30,100,"Zurueck",100,100,*transitionIdleState,0,1);
}

// Gueltige Items / Layout im Idle State
void CreateInitialScreen()
{   
	idleStateSize = 10;
	myIdleState[0] = myItems[BTN_IDLE_STATE];
    myIdleState[1] = myItems[BTN_NEXT_STATE];
    myIdleState[2] = myItems[LAB_IDLE_STATE1];
	myIdleState[3] = myItems[LAB_IDLE_STATE2];
	myIdleState[4] = myItems[BTN_QUIT_STATE];
	
	
	myIdleState[5] = myItems[BTN_COIN_5];
	myIdleState[6] = myItems[BTN_COIN_10];
	myIdleState[7] = myItems[BTN_COIN_20];
	myIdleState[8] = myItems[BTN_COIN_50];
	myIdleState[9] = myItems[BTN_COIN_100];
}

// Gueltige Items / Layout im Next State
void CreateNextScreen() {	
	nextStateSize = 5;	
	myNextState[0] = myItems[BTN_IDLE_STATE];
    myNextState[1] = myItems[BTN_NEXT_STATE];
    myNextState[2] = myItems[LAB_NEXT_STATE1];
	myNextState[3] = myItems[LAB_IDLE_STATE2];		
	myNextState[4] = myItems[BTN_QUIT_STATE];
}

// gueltige Items / Layout im Quit State
void CreateQuitScreen()
{
	quitStateSize = 1;
	myQuitState[0] = myItems[LAB_QUIT_STATE1];
}

// gueltige Items / Layout im Coin State
void CreateCoinScreen()
{
	coinStateSize = 7;
	myCoinState[0] = myItems[LAB_PAY_STATE1];
	myCoinState[1] = myItems[BTN_COIN_5];
	myCoinState[2] = myItems[BTN_COIN_10];
	myCoinState[3] = myItems[BTN_COIN_20];
	myCoinState[4] = myItems[BTN_COIN_50];
	myCoinState[5] = myItems[BTN_COIN_100];	
	myCoinState[6] = myItems[BTN_CANCEL_PAY];	
}

void Main_task(uint_32 initial_data)
{
     FILE_PTR devPtr = NULL;
     
   /* initialize the network stack */
	init_network();

	// oeffnen der Verbindung zum Display				
	devPtr = Init(devPtr); 
	
	// Item liste initialisieren
	InitializeItems();
		
	// Screens / Layouts definieren
	CreateInitialScreen();		
	CreateNextScreen();
	CreateQuitScreen();
	CreateCoinScreen();	
	
	// Initiales "sauberes" Display erzeugen
	cleanDisplay(devPtr); 		
	// prev und next state auf initialzustand
	prevState = UNINITIALIZED;
	nextState = UNINITIALIZED;
	
	// wechsel in den IDLE zustand.
	transitionIdleState(devPtr,0);

	// handle display events.
	   	while(TRUE)
	   	{   	   	   	
	   		display_read_task(myItems,devPtr,myItemsSize);   					   		
	   		_time_delay(rs232TouchDelay);    // wartezeit bis erneute eingabe moeglich ist.   		   	   		
	   	}   
	 
}


/* EOF */



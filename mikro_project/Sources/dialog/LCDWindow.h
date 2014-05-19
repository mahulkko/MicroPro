/*************************************************************************
/	Module		: Zugriffsfkt. LCDWindow 
/-------------------------------------------------------------------------
/	Description	: Zugriffsfunktionen auf den EAeDIP320_8 DisplayTreiber 
/				  Version 1.0.
/				  
/
/-------------------------------------------------------------------------
/	Department	: HTWG Konstanz, Studiengang Techn. Informatik
/	Project		: Display Treiber EAeDIP320_8.c
/	Version		: 1.0
/	Released	: 14.04.2010
/	Written by	: Jan Radon
/************************************************************************/

#include <mqx.h>
#include <bsp.h>
#include <rtcs.h>

#ifndef LCDWindow_H 

#define LCDWindow_H 

// Type (item) unterscheidung Button, Label, Box
#define BUTTON 1
#define LABEL 2
#define BOX 3

// Repraesentiert ein Display item,
struct item
{		
	int x; // X position
	int y; // Y position
	int width; // breite
	int height; // hoehe
	int upCode; // up Code (bei buttons)
	int downCode; // downCode (bei buttons)
	char* text;   // Beschriftung
	void (*handle)(FILE_PTR devPtr,int opCode); // Funktionspointer zur Haendler funktion
	void (*drawfct)(FILE_PTR devPtr,struct item* btn); // Zeichenfunktion (wie wird item auf dem LCD dargestellt)	
	int enabled; // Button aktiv = 1, inaktiv = 0
	int type; // BUTTON,LABEL,BOX
	int font; // 0-31 Fonttype siehe Doku
	int toggle; // normaler Button= 0, Radio Button = 1
};

// Initialisiert das Display und oeffnet die Verbindung (oeffnet devPtr)
FILE_PTR Init(FILE_PTR devPtr); 
// schliest die Verbindung zum Display (schliesst devPtr);
void Close(FILE_PTR devPtr);
// löscht das Display
void cleanDisplay(FILE_PTR devPtr);    
// zeit alle itemList[] elemente mittels (drawfct) auf dem display an, 
// groesse des Arrays muss mitgegeben werden.
void show(struct item *itemList[],FILE_PTR devPtr,int size);

// hinterlegte drawfcts
void drawFctLabel(FILE_PTR devPtr,struct item* currentLab);
void drawFctToggleButton(FILE_PTR devPtr,struct item* current);
void drawFctButton(FILE_PTR devPtr,struct item* current);
void drawFctBox(FILE_PTR devPtr,struct item* currentLab);

// hinterlegte item erzeuger methoden
struct item* createButton(int x, int y, int height,int width,char* text,int upCode,int downCode, void* handlefct,int toggle,int enabled);
struct item* createLabel(int x, int y, char* text, int font);
struct item* createBox(int x,int y,int width,int height);

// sperrt einen button (nicht aktiv auf dem display)
void lockItem(FILE_PTR devPtr, struct item* item);
// loescht das item aus dem display anzeigespeicher
void hideItem(FILE_PTR devPtr, struct item* item);
// Schaltet die Display anzeige an/aus
void displayOff(FILE_PTR devPtr);
void displayOn(FILE_PTR devPtr);
// aendert den Text eines items.
void itemChangeText(struct item* label,char* text);
// schaltet die display hintergrundbeleuchtung an / aus.
void displayLight(FILE_PTR devPtr,int mode);


// nicht blockierende methode zum auslesen von op codes aus dem Display speicher.
int display_read_task(struct item *itemList[], FILE_PTR devPtr,int size );

#endif  

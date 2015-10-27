/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2015 Andrei Degtiarev. All right reserved
  
  You can find the latest version of the library at 
  https://github.com/AndreiDegtiarev/AWind

  This library is free software; you can redistribute it and/or
  modify it under the terms of the CC BY-NC-SA 3.0 license.
  Please see the included documents for further information.

  Commercial use of this library requires you to buy a license that
  will allow commercial use. This includes using the library,
  modified or not, as a tool to sell products.

  The license applies to all part of the library including the 
  examples and tools supplied with the library.
*/
#include <UTFT.h>
#include <UTouch.h>


#include "Log.h"
#include "LinkedList.h"
#include "WindowsManager.h"
#include "DefaultDecorators.h"
#include "Window1.h"
#include "Window2.h"
#include "Window3.h"

#include "WindowSelector.h"

// Setup TFT display + touch (see UTFT and UTouch library documentation)
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32,25,26,27,28);
UTouch  myTouch(6,5,32,3,2);
#else
UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
#endif

//manager which is responsible for window updating process
WindowsManager<WindowSelector> windowsManager(&myGLCD,&myTouch);


void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(57600);
	out<<F("Setup")<<endln;

	//initialize display
	myGLCD.InitLCD(); 
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecorators::InitAll();

	//initialize window manager
	windowsManager.Initialize();

	//create tabs
	windowsManager.MainWnd()->Initialize(); 
	Window1 *window1=new Window1(F("Window1"),0,0,0,0);
	Window2 *window2=new Window2(F("Window2"),0,0,0,0);
	Window3 *window3=new Window3(F("Window3"),0,0,0,0);
	windowsManager.MainWnd()->AddTab(F("Red"),window1);
	windowsManager.MainWnd()->AddTab(F("Yellow"),window2);
	windowsManager.MainWnd()->AddTab(F("Green"),window3);

	AHelper::LogFreeRam();
	delay(1000); 
	out<<F("End setup")<<endln;

}

void loop()
{
	//give window manager an opportunity to update display
	windowsManager.loop();
}
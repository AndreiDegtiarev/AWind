/*
  Text example is intended to demonstrate basics features of AWind library. 

  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
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

#include "WindowsManager.h"
#include "TouchManager.h"
#include "Log.h"
#include "TextExampleWindow.h"

//UTFT disaply connection
UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);

//Windows manager: container for GUI elements 
WindowsManager windowsManager(&myGLCD);
TouchManager touchManager(&myTouch,&windowsManager);



void setup()
{
	out.begin(57600);
	out<<(F("Setup"))<<endl;

	myGLCD.InitLCD();
	myGLCD.clrScr();
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	windowsManager.Initialize();
	windowsManager.SetCriticalProcess(&touchManager);

	//my speciality: I connect LED-A display pin to D47 on arduino board
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);


	windowsManager.MainWnd()->AddChild(new TextExampleWindow(windowsManager.GetDC()->DeviceWidth(),windowsManager.GetDC()->DeviceHeight()));


	out<<F("End setup")<<endl;

}

void loop()
{
	windowsManager.loop();
}


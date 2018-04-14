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
#include "DC_UTFT.h"
#include "TouchUTFT.h"

#include "WindowsManager.h"
#include "Log.h"
#include "TextExampleWindow.h"
#include "DefaultDecorators.h"

// Setup TFT display + touch (see UTFT and UTouch library documentation)
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE +tft shield
UTFT    myGLCD(CTE32,25,26,27,28);
URTouch  myTouch(6,5,32,3,2);
//URTouch  myTouch(6, 5, 4, 3, 2);
#else
UTFT    myGLCD(ITDB32S,39,41,43,45);
URTouch  myTouch( 49, 51, 53, 50, 52);
#endif

DC_UTFT dc(&myGLCD);
TouchUTFT touch(&myTouch);


//Windows manager: container for GUI elements 
WindowsManager<TextExampleWindow> windowsManager(&dc,&touch);



void setup()
{
	out.begin(9600);
	out<<(F("Setup"))<<endln;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	//pinMode(47,OUTPUT);
	//digitalWrite(47,HIGH);
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_LOW);

	DC_UTFT::RegisterDefaultFonts();
	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecorators::InitAll();
	//initialize window manager
	windowsManager.Initialize();

	out<<F("End setup")<<endln;

}

void loop()
{

	windowsManager.loop();
}


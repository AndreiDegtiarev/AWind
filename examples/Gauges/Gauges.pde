/*
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

#include "LinkedList.h"
#include "WindowsManager.h"
#include "TouchManager.h"
#include "GaugesWindow.h"

// Setup TFT display + touch (see UTFT and UTouch library documentation)
UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);

//manager which is responsible for window updating process
WindowsManager windowsManager(&myGLCD);
//manager which is responsible for processing of touch events
TouchManager touchManager(&myTouch,&windowsManager);

GaugesWindow *gaugesWnd;

int time_step_mus=100;
const int reserved_buf_size=2000;
int buf_size=500;

void setup()
{
	//setup log (out is wrap about Serial class)
	out.begin(57600);
	out<<F("Setup")<<endl;

	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//initialize touch
	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	//initialize window manager
	windowsManager.Initialize();


	windowsManager.SetCriticalProcess(&touchManager);

	gaugesWnd=new GaugesWindow(0.0,0.0,windowsManager.GetDC()->DeviceWidth(),windowsManager.GetDC()->DeviceHeight());
	windowsManager.MainWnd()->AddChild(gaugesWnd);

	//finalize window initialization: window resizing and etc.
	windowsManager.InitializeWindowSystem();
	delay(1000); 
	out<<F("End setup")<<endl;

}

void loop()
{
	//give window manager an opportunity to update display
	windowsManager.loop();
}
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
#include "VoltmeterSensor.h"
#include "Oscilloscope.h"


UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
extern uint8_t ArialNumFontPlus[];


WindowsManager windowsManager(&myGLCD);
TouchManager touchManager(&myTouch,&windowsManager);
VoltmeterSensor *voltmeter;
Oscilloscope *oscilloscopeWnd;

int time_step_mus=100;
const int reserved_buf_size=2000;
int buf_size=500;

void setup()
{
	out.begin(57600);
	out<<F("Setup");

	myGLCD.InitLCD();
	myGLCD.clrScr();

	windowsManager.Initialize();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myGLCD.setFont(ArialNumFontPlus);
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	voltmeter=new VoltmeterSensor(A0,reserved_buf_size,500);
	voltmeter->SetTimeStep(time_step_mus);

	windowsManager.SetCriticalProcess(&touchManager);

	oscilloscopeWnd=new Oscilloscope(voltmeter,buf_size,0.0,4.0,windowsManager.GetDC()->DeviceWidth(),windowsManager.GetDC()->DeviceHeight());
	windowsManager.MainWnd()->AddChild(oscilloscopeWnd);

	delay(1000); 
	out<<F("End setup");

}

void loop()
{
	voltmeter->MeasureBuffer();
	oscilloscopeWnd->ChartWnd()->SetBuffer(voltmeter->Buffer());
	windowsManager.loop();
}
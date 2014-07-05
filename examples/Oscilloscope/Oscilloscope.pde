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

#include "ChartWindow.h"

#include "VoltmeterSensor.h"


UTFT    myGLCD(ITDB32S,39,41,43,45);
UTouch  myTouch( 49, 51, 53, 50, 52);
extern uint8_t ArialNumFontPlus[];

WindowsManager windowsManager(&myGLCD,loopTouch);
TouchManager touchManager(&myTouch,&windowsManager);
VoltmeterSensor *voltmeter;
ChartWindow *chartWnd;

const float time_length=0.001;
const int buf_size=3000;
const int sample_ratio=(int)(1.0/(time_length/buf_size));

void setup()
{
	Serial.begin(57600);
	Serial.println(F("Setup"));

	myGLCD.InitLCD();
	myGLCD.clrScr();

	myTouch.InitTouch();
	myTouch.setPrecision(PREC_MEDIUM);

	myGLCD.setFont(ArialNumFontPlus);
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	voltmeter=new VoltmeterSensor(A0,buf_size);
	Log::Number("Sample ratio:",sample_ratio,true);
	Log::Number("ms:",(int)(1e6*1.0/sample_ratio),true);

	chartWnd=new ChartWindow(0,0,windowsManager.MainWindow()->Width(),windowsManager.MainWindow()->Height());
	windowsManager.MainWindow()->AddChild(chartWnd);

	delay(1000); 
	Serial.println("End setup");

}
void loopTouch()
{
  touchManager.loop();
}
void loop()
{
	voltmeter->MeasureBuffer(sample_ratio);
	chartWnd->SetBuffer(voltmeter->Buffer());
	loopTouch();
	windowsManager.loop();
}
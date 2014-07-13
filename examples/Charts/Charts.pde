/*
  Charts example is intended to demonstrate basics features of AWind library. 

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
#include "ChartWindow.h"
#include "TimeSerieBuffer.h"
#include "TextBoxString.h"
#include "TextBoxNumber.h"
#include "Log.h"

//UTFT disaply connection
UTFT    myGLCD(ITDB32S,39,41,43,45);

//Windows manager: container for GUI elements 
WindowsManager windowsManager(&myGLCD);

//Container that keeps data for further visualization 
TimeSerieBuffer	*dataBuffer;
ChartWindow *chartWnd;
TextBoxNumber *textNumber;

//Number of chart points
int buf_size=1000;
//Time step in buffer. If this paramter is defined it is no more need to store x-date separatly
float time_step=1.0/buf_size;

void setup()
{
	out.begin(57600);
	out<<(F("Setup"));

	myGLCD.InitLCD();
	myGLCD.clrScr();
	windowsManager.Initialize();

	//my speciality: I connect LED-A display pin to D47 on arduino board
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);

	dataBuffer=new TimeSerieBuffer(time_step,100,1000,1000);

	int x=0;
	int y=0;
	TextBoxFString *textBox=new TextBoxFString(x,y,windowsManager.GetDC()->DeviceWidth()/2,25,F("Scaling factor: "),Color::SkyBlue);
	textBox->SetFont(BigFont);
	x=windowsManager.GetDC()->DeviceWidth()*3.0/4;
	textNumber=new TextBoxNumber(x,y,windowsManager.GetDC()->DeviceWidth()-x,25,0,Color::SkyBlue);
	textNumber->SetBackColor(Color::Red);
	textNumber->SetFont(BigFont);
	textNumber->SetMargins(20,2);
	x=0;
	y+=25;
	chartWnd=new ChartWindow(x,y,windowsManager.GetDC()->DeviceWidth(),windowsManager.GetDC()->DeviceHeight()-25);
	chartWnd->SetBackColor(Color::Black);
	chartWnd->SetBuffer(dataBuffer);

	windowsManager.MainWnd()->AddChild(textBox);
	windowsManager.MainWnd()->AddChild(textNumber);
	windowsManager.MainWnd()->AddChild(chartWnd);


	out<<F("End setup");

}
int index=100;
void loop()
{
	if(index<1)
		index=100;
	dataBuffer->SetFactorY(index);
	for(unsigned int i=0;i<1000;i++)
	{
		dataBuffer->Set(i,sin(2*3.14*(time_step*i)));
	}
	textNumber->SetNumber(index);
	chartWnd->Invalidate();
	index/=2;
	windowsManager.loop();
	delay(1000);
}


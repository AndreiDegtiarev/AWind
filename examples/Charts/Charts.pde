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
#include "DecoratorPrimitives.h"

// Setup TFT display (see UTFT and UTouch library documentation)
UTFT    myGLCD(ITDB32S,39,41,43,45);

//manager which is responsible for window updating process 
WindowsManager<> windowsManager(&myGLCD,NULL);

//Container that keeps data for further visualization 
TimeSerieBuffer	*dataBuffer;
ChartWindow *chartWnd;
TextBoxNumber *textNumber;

//Number of chart points
int buf_size=1000;
//Time step in buffer. If this parameter is defined it is no more need to store x-date separatly
float time_step=1.0/buf_size;

void setup()
{
	out.begin(57600);
	out<<(F("Setup"))<<endl;
	//initialize display
	myGLCD.InitLCD();
	myGLCD.clrScr();
	//my speciality I have connected LED-A display pin to the pin 47 on Arduino board. Comment next two lines if the example from UTFT library runs without any problems 
	pinMode(47,OUTPUT);
	digitalWrite(47,HIGH);
	
	//initialize window manager
	windowsManager.Initialize();

	dataBuffer=new TimeSerieBuffer(time_step,100,1000,1000);
	//Setup main window background
	windowsManager.MainWnd()->AddDecorator(new DecoratorRectFill(Color::LightGray));

	int x=0;
	int y=2;
	int cy=25;
	TextBoxFString *textBox=new TextBoxFString(x,y,windowsManager.GetDC()->DeviceWidth()/2,cy,F("Scaling factor: "));
	//Setup transparent background and green text color
	textBox->AddDecorator(new DecoratorColor(Color::Green));
	textBox->SetFont(BigFont);
	x=windowsManager.GetDC()->DeviceWidth()*3.0/4;
	textNumber=new TextBoxNumber(x,y,windowsManager.GetDC()->DeviceWidth()-x-2,cy,0);
	//Setup background + 3D-look of text box
	textNumber->AddDecorator(new DecoratorRectFill(Color::Red));
	textNumber->AddDecorator(new Decorator3DRect(Color::DarkGray,Color::White));
	textNumber->AddDecorator(new DecoratorColor(Color::SkyBlue));
	textNumber->SetFont(BigFont);
	textNumber->SetMargins(20,2);
	x=1;
	y+=cy+5;
	cy=windowsManager.GetDC()->DeviceHeight()-y-2;
	//We create axis decorator separetly because chart need this decorator as well
	int axis_y_margins=2;
	DecoratorAxis *chartYAxis=new DecoratorAxis(DecoratorAxis::VerticalLeft,SmallFont,cy-axis_y_margins*2,-1,1,5);
	chartYAxis->SetOffset(4,axis_y_margins);
	chartWnd=new ChartWindow(NULL,chartYAxis,x,y,windowsManager.GetDC()->DeviceWidth()-2,cy);
	//Chart decorators
	chartWnd->AddDecorator(new Decorator3DRect(Color::White,Color::Gray));
	chartWnd->AddDecorator(new DecoratorColor(Color::Black));
	chartWnd->AddDecorator(chartYAxis);
	chartWnd->SetBuffer(dataBuffer);
	///Attach crated window to the main window
	windowsManager.MainWnd()->AddChild(textBox);
	windowsManager.MainWnd()->AddChild(textNumber);
	windowsManager.MainWnd()->AddChild(chartWnd);


	out<<F("End setup")<<endl;

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
	//In order to reduce flickering only chart area is updated
	chartWnd->InvalidateOnlyChartArea();
	index/=2;
	//give window manager an opportunity to update display
	windowsManager.loop();
	delay(1000);
}


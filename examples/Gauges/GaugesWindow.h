#pragma once
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
#include "Gauge.h"
#include "ChartWindow.h"
#include "TextBoxString.h"
#include "ButtonWindow.h"
#include "SensorDataBuffer.h"
#include "SensorManager.h"
#include "FakeSensor.h"

///Main window of gauges example. Routs events between child elements
class GaugesWindow : public MainWindow, public ITouchEventReceiver, public ISensorHasDataEventReceiver, public ISensorMeasuredEventReceiver
{
	Gauge *_gaugeBar;
	Gauge *_gaugeRadialPointer;
	ChartWindow *_chartWindow;
	TextBoxFString *_btnFast;
	TextBoxFString *_btnSlow;
	ButtonWindow *_btnTop;
	ButtonWindow *_btnBottom;
	SensorManager *_sensorManager;
	bool _isAuto;
public:
	///Constructor
	/**
	\param width window width
	\param height window height
	*/
	GaugesWindow(int width,int height):MainWindow(width,height)
	{
		_sensorManager=NULL;
		SetBackColor(Color::WhiteSmoke);
		_isAuto=false;
		int x=1;
		int szx=width/4;
		_gaugeBar=new Gauge(Gauge::Bar,x,1,szx,height-2);
		x+=szx+1;
		szx=width/2;
		_gaugeRadialPointer=new Gauge(Gauge::RadialPointer,x,1,szx,height/2-2);
		_chartWindow=new ChartWindow(x,height/2,szx,height/2-2);
		AddChild(_gaugeBar); 
		AddChild(_gaugeRadialPointer);
		AddChild(_chartWindow);
		x+=szx+4;
		szx=width/4-6;
		_btnFast=new TextBoxFString(x,20,szx,30,F("Fast"),Color::Blue);
		initTextBox(_btnFast);
		_btnSlow=new TextBoxFString(x,70,szx,30,F("Slow"),Color::Blue);
		initTextBox(_btnSlow);
		_btnTop=new ButtonWindow(ButtonWindow::TriangleTop,x+15,height/2,40,40);
		_btnBottom=new ButtonWindow(ButtonWindow::TriangleBottom,x+15,height/2+60,40,40);
		initButton(_btnTop);
		initButton(_btnBottom);
	}
	///Initialize text box windows
	void initTextBox(TextBoxFString *textBox)
	{
		textBox->SetBorder(Color::Green);
		textBox->SetFont(BigFont);
		textBox->SetMargins(0,5);
		textBox->RegisterTouchEventReceiver(this);
		textBox->SetBackColor(GetBackColor());
		AddChild(textBox);
	}
	///Initialize button windows
	void initButton(ButtonWindow *button)
	{
		button->SetBorder(Color::Blue);
		button->RegisterTouchEventReceiver(this);
		button->SetBackColor(GetBackColor());
		AddChild(button);
	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if(window==_btnFast)
		{
			out<<F("touch in gauges auto")<<endl;
			if(_sensorManager!=NULL)
			{
				_sensorManager->SetPause(_sensorManager->GetPause()*1.1);
			}
		}
		else if(window==_btnSlow)
		{
			out<<F("touch in gauges manual")<<endl;
			if(_sensorManager!=NULL)
			{
				_sensorManager->SetPause(_sensorManager->GetPause()*0.9);
			}
		}
		else if(window==_btnTop)
		{
			out<<F("touch in gauges top")<<endl;
			if(_sensorManager!=NULL)
				((FakeSensor *)_sensorManager->Sensor())->Increase();
		}
		else if(window==_btnBottom)
		{
			out<<F("touch in gauges bottom")<<endl;
			if(_sensorManager!=NULL)
				((FakeSensor *)_sensorManager->Sensor())->Decrease();
		}
	}
	///Events routing for gui interaction (see RegisterReceiver and public ISensorHasDataEventReceiver declaration)
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		float value=sensorManager->GetData();
		_gaugeBar->SetValue(value);
		_gaugeRadialPointer->SetValue(value);
		_sensorManager=sensorManager;
		_chartWindow->SetBuffer(sensorManager->SecBuffer());
		_chartWindow->Invalidate();
	}
	void NotifySensorMeasured(SensorManager *sensorManager)
	{
		_chartWindow->Invalidate();
	}
};
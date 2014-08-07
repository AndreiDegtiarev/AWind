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
#include "GaugeBar.h"
#include "GaugeRadialPointer.h"
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
	TextBoxNumber  *_txtNumber;
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
		AddDecorator(new DecoratorRectFill(Color::LightGray,false));
		_isAuto=false;
		int x=1;
		int szx=width/4;
		int szy=height-2;
		DecoratorList *gaugeDecorators=new DecoratorList(GetDecorators());
		gaugeDecorators->Add(new Decorator3DRect(Color::White,Color::Gray));
		gaugeDecorators->Add(new DecoratorColor(Color::Green));
		DecoratorList *gaugeRadialPointerDecorators=new DecoratorList(*gaugeDecorators);
		int gauge_axis_y_margins=3;
		DecoratorAxis *gaugeAxis=new DecoratorAxis(DecoratorAxis::VerticalRight,SmallFont,szy-gauge_axis_y_margins*2,0,100,5);
		gaugeDecorators->Add(gaugeAxis);
		DC dc;
		int offsetX=gaugeAxis->EstimateRight(&dc);
		gaugeAxis->SetOffset(szx-offsetX,gauge_axis_y_margins);
		_gaugeBar=new GaugeBar(gaugeAxis,x,1,szx,szy);
		_gaugeBar->SetDecorators(*gaugeDecorators);
		_gaugeBar->SetFillColor(Color::LightGray);
		AddChild(_gaugeBar); 
		x+=szx+3;
		szx=width/2;
		_gaugeRadialPointer=new GaugeRadialPointer(x,1,szx,height/2-2);
		_gaugeRadialPointer->SetDecorators(*gaugeRadialPointerDecorators);
		_gaugeRadialPointer->SetFillColor(Color::LightGray);
		szy=height/2-4;
		int axis_y_margins=2;
		DecoratorAxis *chartYAxis=new DecoratorAxis(DecoratorAxis::VerticalLeft,SmallFont,szy-axis_y_margins*2,0,100,5);
		chartYAxis->SetOffset(4,axis_y_margins);
		_chartWindow=new ChartWindow(NULL,chartYAxis,x,height/2+2,szx,szy);
		_chartWindow->AddDecorator(new DecoratorRectFill(Color::LightGray));
		_chartWindow->AddDecorator(new Decorator3DRect(Color::White,Color::Gray));
		_chartWindow->AddDecorator(new DecoratorColor(Color::Black));
		_chartWindow->AddDecorator(chartYAxis);
		AddChild(_gaugeRadialPointer);
		AddChild(_chartWindow);
		x+=szx+2;
		szx=width/4-6;
		int y=4;
		szy=30;
		_txtNumber=new TextBoxNumber(x,y,szx,szy,0);
		_txtNumber->AddDecorator(new DecoratorRectFill(Color::Black,false));
		_txtNumber->AddDecorator(new Decorator3DRect(Color::Gray,Color::White));
		_txtNumber->AddDecorator(new DecoratorColor(Color::White));
		initTextBox(_txtNumber,&_txtNumber->GetDecorators());
		y+=szy+13;
		DecoratorList *txtDecorators=new DecoratorList(GetDecorators());
		txtDecorators->Add(new Decorator3DRect(Color::White,Color::Gray));
		txtDecorators->Add(new DecoratorColor(Color::Black));
		_btnFast=new TextBoxFString(x,y,szx,szy,F("Fast"));
		initTextBox(_btnFast,txtDecorators);
		y+=szy+10;
		_btnSlow=new TextBoxFString(x,y,szx,szy,F("Slow"));
		initTextBox(_btnSlow,txtDecorators);
		y+=szy+10;
		szx=szy=40;
		_btnTop=new ButtonWindow(ButtonWindow::TriangleTop,x+15,y,szx,szy);
		y+=szy+20;
		_btnBottom=new ButtonWindow(ButtonWindow::TriangleBottom,x+15,y,szx,szy);
		initButton(_btnTop,txtDecorators);
		initButton(_btnBottom,txtDecorators);
	}
	///Initialize text box windows
	void initTextBox(TextBox *textBox,DecoratorList *decorators)
	{
		textBox->SetDecorators(*decorators);
		textBox->SetFont(BigFont);
		textBox->SetMargins(0,5);
		textBox->RegisterTouchEventReceiver(this);
		//textBox->SetBackColor(GetBackColor());
		AddChild(textBox);
	}
	///Initialize button windows
	void initButton(ButtonWindow *button,DecoratorList *decorators)
	{
		button->SetDecorators(*decorators);
		button->RegisterTouchEventReceiver(this);
		AddChild(button);
	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if(window==_btnFast)
		{
			out<<F("touch in gauges fast")<<endl;
			if(_sensorManager!=NULL)
			{
				_sensorManager->SetPause(_sensorManager->GetPause()*0.9);
			}
		}
		else if(window==_btnSlow)
		{
			out<<F("touch in gauges slow")<<endl;
			if(_sensorManager!=NULL)
			{
				_sensorManager->SetPause(_sensorManager->GetPause()*1.1);
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
	///Event sensor has new data. If data is the same as measured previosly. This event is not generated (see RegisterReceiver and public ISensorHasDataEventReceiver declaration)
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		float value=sensorManager->GetData();
		_gaugeBar->SetValue(value);
		_gaugeRadialPointer->SetValue(value);
		_txtNumber->SetNumber(value);
		_sensorManager=sensorManager;
		_chartWindow->SetBuffer(sensorManager->SecBuffer());
		//_chartWindow->InvalidateOnlyChartArea();
	}
	///Event is generated after each measurement
	void NotifySensorMeasured(SensorManager *sensorManager)
	{
		_chartWindow->InvalidateOnlyChartArea();
	}
};
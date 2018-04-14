#pragma once
/*
This file is part of AWind library

Copyright (c) 2014-2018 Andrei Degtiarev

Licensed under the Apache License, Version 2.0 (the "License"); you
may not use this file except in compliance with the License.  You may
obtain a copy of the License at

http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or
implied.  See the License for the specific language governing
permissions and limitations under the License.
*/
#include "GaugeBar.h"
#include "GaugeRadialPointer.h"
#include "ChartWindow.h"
#include "Button.h"
#include "ButtonWindow.h"
#include "SensorDataBuffer.h"
#include "SensorManager.h"
#include "FakeSensor.h"

#include "WindowsManager.h"
#include "MeasurementNode.h"
#include "DefaultDecorators.h"

///Main window of gauges example. Routs events between child elements
class GaugesWindow : public MainWindow, public ITouchEventReceiver, public ISensorHasDataEventReceiver, public ISensorMeasuredEventReceiver
{
	Gauge *_gaugeBar;
	GaugeRadialPointer *_gaugeRadialPointer;
	ChartWindow *_chartWindow;
	Button *_btnFast;
	Button *_btnSlow;
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
		int gauge_axis_y_margins=3;
		DecoratorAxis *gaugeAxis=new DecoratorAxis(DecoratorAxis::VerticalRight, Environment::Get()->FindFont(F("Small")),szy-gauge_axis_y_margins*2,0,100,5);
		//DC dc;
		int offsetX=gaugeAxis->EstimateRight(globalLcd);
		gaugeAxis->SetOffset(szx-offsetX,gauge_axis_y_margins);
		_gaugeBar=new GaugeBar(gaugeAxis,x,1,szx,szy);
		_gaugeBar->SetFillColor(Color::LightGray);
		AddChild(_gaugeBar); 
		x+=szx+3;
		szx=width/2;
		_gaugeRadialPointer=new GaugeRadialPointer(x,1,szx,height/2-2);
		_gaugeRadialPointer->SetFillColor(Color::LightGray);
		_gaugeRadialPointer->SetPointerColor(Color::Green);
		szy=height/2-4;
		int axis_y_margins=2;
		DecoratorAxis *chartYAxis=new DecoratorAxis(DecoratorAxis::VerticalLeft, Environment::Get()->FindFont(F("Small")),szy-axis_y_margins*2,0,100,5);
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
		szy= height / 7;
		int dy = szy / 3;
		_txtNumber=new TextBoxNumber(x,y,szx,szy,0);
		initTextBox(_txtNumber);
		y+=szy + dy;
		_btnFast=new Button(x,y,szx,szy,F("Fast"));
		initTextBox(_btnFast);
		y+=szy + dy;
		_btnSlow=new Button(x,y,szx,szy,F("Slow"));
		initTextBox(_btnSlow);
		y+=szy + dy;
		szx=szy=(height - y - dy*2)/2;
		_btnTop=new ButtonWindow(ButtonWindow::TriangleTop,x+15,y,szx,szy);
		y+=szy+ dy;
		_btnBottom=new ButtonWindow(ButtonWindow::TriangleBottom,x+15,y,szx,szy);
		initButton(_btnTop);
		initButton(_btnBottom);
	}
	///Initialize text box windows
	void initTextBox(TextBox *textBox)
	{
		textBox->SetMargins(0,5);
		textBox->RegisterTouchEventReceiver(this);
		//textBox->SetBackColor(GetBackColor());
		AddChild(textBox);
	}
	///Initialize button windows
	void initButton(ButtonWindow *button)
	{
		button->RegisterTouchEventReceiver(this);
		AddChild(button);
	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if(window==_btnFast)
		{
			out<<F("touch in gauges fast")<<endln;
			if(_sensorManager!=NULL)
			{
				_sensorManager->SetPause(_sensorManager->GetPause()*0.9);
			}
		}
		else if(window==_btnSlow)
		{
			out<<F("touch in gauges slow")<<endln;
			if(_sensorManager!=NULL)
			{
				_sensorManager->SetPause(_sensorManager->GetPause()*1.1);
			}
		}
		else if(window==_btnTop)
		{
			out<<F("touch in gauges top")<<endln;
			if(_sensorManager!=NULL)
				((FakeSensor *)_sensorManager->Sensor())->Increase();
		}
		else if(window==_btnBottom)
		{
			out<<F("touch in gauges bottom")<<endln;
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
		if (value > 75)
			_gaugeRadialPointer->SetPointerColor(Color::Red);
		else
			_gaugeRadialPointer->SetPointerColor(Color::Green);
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
//list where all sensors are collected
LinkedList<SensorManager> sensors;
//manager which controls the measurement process
MeasurementNode measurementNode(sensors, NULL);
void setupExample(WindowsManager<GaugesWindow> &windowsManager)
{
	//Initialize apperance. Create your own DefaultDecorators class if you would like different application look
	DefaultDecorators::InitAll();

	//initialize window manager
	windowsManager.Initialize();

	SensorManager *manager = new SensorManager(new FakeSensor(), 15, 40, 1000 * 1);
	sensors.Add(manager);
	manager->RegisterHasDataEventReceiver(windowsManager.MainWnd());
	manager->RegisterMeasuredEventReceiver(windowsManager.MainWnd());
	//in order to avoid pause in the touch interactions, windows manager is defined as critical process
	measurementNode.SetCriticalProcess(&windowsManager);
}
void loopExample(WindowsManager<GaugesWindow> &windowsManager)
{
	//measure (if necessary -see delay parameter in sensor manager)
	if (measurementNode.Measure())
	{
		//following if is only for debugging purposes
		if (measurementNode.IsChanged())
		{
			measurementNode.LogResults();
		}

	}
	//give window manager an opportunity to update display
	windowsManager.loop();
}
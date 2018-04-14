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
#pragma once
#include <Window.h>
#include "TimeSerieBuffer.h"
#include <ChartWindow.h>
///Tab with chart for temperature logging 
class TabProcess : public Window, public ISensorHasDataEventReceiver,  public ISensorMeasuredEventReceiver, public ITouchEventReceiver
{
	RadioButton    *_radioTemperature;
	RadioButton    *_radioPressure;

	ChartWindow *_chartWnd;
	DecoratorAxis *_chartYAxis;
	PumpController *_pumpController;
	//bool _isBufferInitialized;

public:
	TabProcess(PumpController *pumpController, const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		SetDecorators(Environment::Get()->FindDecorators(F("Window")));


		_radioTemperature = new RadioButton(0, 0, 0, 0, F("Temper."));
		initRadio(_radioTemperature);
		_radioPressure = new RadioButton(0, 0, 0, 0, F("Vacuum"));
		initRadio(_radioPressure);

		_pumpController = pumpController;
		_pumpController->RegisterHasDataEventReceiver(this);
		//_isBufferInitialized = false;

	}
	void initRadio(RadioButton *radio)
	{
		radio->SetMargins(20, 5);
		radio->RegisterTouchEventReceiver(this);
		AddChild(radio);
	}
	void Initialize()
	{
		int cy = Height()-40;
		int axis_y_margins = 5;

		_radioTemperature->Move(5,2,70,30);
		_radioPressure->Move(160, 2, 70, 30);

		_chartYAxis = new DecoratorAxis(DecoratorAxis::VerticalLeft, Environment::Get()->FindFont(F("Small")), cy - axis_y_margins * 2, 10, 50, 5);
		_chartYAxis->SetOffset(4, axis_y_margins);
		_chartWnd = new ChartWindow(NULL, _chartYAxis, 2, Height() - cy, Width() - 4, cy - axis_y_margins * 2);
		//Chart decorators
		_chartWnd->AddDecorator(new Decorator3DRect(Color::White, Color::Gray));
		_chartWnd->AddDecorator(new DecoratorColor(Color::Black));
		_chartWnd->AddDecorator(_chartYAxis);
		AddChild(_chartWnd);

		_pumpController->Hardware().TemperatureSensorManager()->RegisterMeasuredEventReceiver(this);
		_pumpController->Hardware().PressureSensorManager()->RegisterMeasuredEventReceiver(this);

		NotifyTouch(_radioTemperature);


	}
	///If sensor data was changed this notification is call
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		/*switch (_pumpController->GetSensorType(sensorManager))
		{
		case PumpController::Temperature:
			if (!_isBufferInitialized)
			{
				//_chartWnd->SetBuffer(sensorManager->SecBuffer());
				//sensorManager->RegisterMeasuredEventReceiver(this);
			}
			//_txtTemperature->SetNumber(sensorManager->GetData());
			break;
		}*/
	}
	///Event is generated after each measurement
	void NotifySensorMeasured(SensorManager *sensorManager)
	{
		_chartWnd->InvalidateOnlyChartArea();
	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if (window == _radioTemperature && !_radioTemperature->IsChecked())
		{
			_radioTemperature->SetChecked(true);
			_radioPressure->SetChecked(false);
			_chartYAxis->SetMinMax(10,50);
			_chartWnd->SetBuffer(_pumpController->Hardware().TemperatureSensorManager()->SecBuffer());
			this->Invalidate();
		}
		else if(window == _radioPressure && !_radioPressure->IsChecked())
		{
			_radioTemperature->SetChecked(false);
			_radioPressure->SetChecked(true);
			_chartYAxis->SetMinMax(0, 0.8);
			_chartWnd->SetBuffer(_pumpController->Hardware().PressureSensorManager()->SecBuffer());
			this->Invalidate();
		}
	}
};
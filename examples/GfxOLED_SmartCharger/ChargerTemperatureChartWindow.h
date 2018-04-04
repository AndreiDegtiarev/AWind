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
#pragma once

#include "DecoratorPrimitives.h"
#include "ChartWindow.h"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

class ChargerTemperatureChartWindow : public Window, public ISensorMeasuredEventReceiver
{
	ChartWindow *_chartWnd;
	DecoratorAxis *_chartYAxis;
	SensorManager *_temperature;

public:
	ChargerTemperatureChartWindow(int width, int height, SensorManager *temperature) :Window(F("Temperature chart"), 0, 0, 0, 0),
		_temperature(temperature)
	{
		auto labelTitle = new TextBoxFString(0, 2, width, 20, F("Temperature"), F("Label"));
		labelTitle->SetFont(SmallFont);
		labelTitle->SetHorizontalAlignment(DC::HorizontalAlignment::Center);
		AddChild(labelTitle);
		_chartYAxis = new DecoratorAxis(DecoratorAxis::VerticalLeft, SmallFont, height - 15, 20, 50, 3);
		_chartYAxis->SetOffset(0, 0);
		_chartWnd = new ChartWindow(NULL, _chartYAxis, 0, 10, width - 5, height - 15);
		_chartWnd->AddDecorator(new DecoratorColor(Color::Black));
		_chartWnd->AddDecorator(_chartYAxis);
		_chartWnd->SetBuffer(_temperature->MinBuffer());
		AddChild(_chartWnd);
		_temperature->RegisterMeasuredEventReceiver(this);
	}
	///Notification is call for each sensor measurements
	void NotifySensorMeasured(SensorManager *sensorManager)
	{
		_chartWnd->Invalidate();
	}
};
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

#include "TextBoxString.h"
#include "TextBoxNumber.h"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

class ChargerOverviewWindow : public Window, public  ISensorHasDataEventReceiver
{
	SensorManager *_currentmeter;
	SensorManager *_voltmeter;
	SensorManager *_temperature;
	TextBoxNumber *_txtTemperature;
	TextBoxNumber *_txtCurrent;
	TextBoxNumber *_txtVoltage;
	TextBoxNumber *_txtHours;
public:
	ChargerOverviewWindow(int width, SensorManager *currentmeter, SensorManager *voltmeter, SensorManager *temperature) :Window(F("Overview"), 0, 0, 0, 0),
	    _currentmeter(currentmeter),
	    _voltmeter(voltmeter),
		_temperature(temperature)
	{
		int x = 20;
		int y = 2;
		auto labelTitle = new TextBoxFString(x, y, width / 2, 20, F("Smart charger"), F("Label"));
		labelTitle->SetFont(F("Small"));
		AddChild(labelTitle);
		x = 0;
		y = 15;
		_txtTemperature = initNumberFields(x, y, width, 0, F("T"));
		x += width / 2 - 10;
		_txtCurrent = initNumberFields(x, y, width, 25, F("I"));
		x = 0;
		y += 25;
		_txtHours = initNumberFields(x, y, width, 0, F("H"));
		x += width / 2 - 10;
		_txtVoltage = initNumberFields(x, y, width, 25, F("V"));
		_txtVoltage->SetPrecission(2);

		currentmeter->RegisterHasDataEventReceiver(this);
		voltmeter->RegisterHasDataEventReceiver(this);
		temperature->RegisterHasDataEventReceiver(this);
	}
	TextBoxNumber *initNumberFields(int x, int y, int width, int dx,const __FlashStringHelper *name)
	{
		auto label = new TextBoxFString(x, y, width / 2, 20, name, F("Label"));
		label->SetMargins(2, 2);
		label->SetFont(F("Big"));
		x += 18;
		auto text = new TextBoxNumber(x, y, 30 + dx, 20, 0);
		text->SetFont(F("Big"));
		text->SetMargins(0, 3);
		text->SetHorizontalAlignment(DC::HorizontalAlignment::Center);
		text->SetNumber(0);

		AddChild(label);
		AddChild(text);
		return text;
	}
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		float value = sensorManager->GetData();
		if (sensorManager == _currentmeter)
		{
			_txtCurrent->SetNumber(value * 1000);
			_txtHours->SetNumber(millis() / 1000 / 60 / 60);
		}
		else if (sensorManager == _voltmeter)
			_txtVoltage->SetNumber(value);
		else if(sensorManager == _temperature)
			_txtTemperature->SetNumber(value);
	}
};
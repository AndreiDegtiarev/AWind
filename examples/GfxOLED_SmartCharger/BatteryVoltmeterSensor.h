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
#include "ChargerController.h"
class BatteryVoltmeterSensor : public VoltmeterSensor
{	
	ChargerController *_controller;
public:
	BatteryVoltmeterSensor(ChargerController *controller,int port, float voltageReference,float offset):VoltmeterSensor(port, voltageReference,offset), _controller(controller)
	{

	}
	///Returns internal sensors name. Usefull for debugging
	virtual const __FlashStringHelper* Name()
	{
		return F("Battery voltage");
	}
	virtual bool Measure(float &data)
	{
		_controller->SetMosfettActive(false);
		delay(3000);
		out << "V:";
		bool retCode = false;
		double middelValue = 0;
		for (int i = 0; i < 10; i++)
		{
			retCode = VoltmeterSensor::Measure(data);
			middelValue = max(middelValue, data);
			out << data << " ";
		}
		data = middelValue;
		out << endln;
		_controller->SetMosfettActive(true);
		delay(5000);
		return retCode;
	}
};
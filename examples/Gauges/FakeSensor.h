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
#include "ISensor.h"
///Fake sensor. It is intended only to generate some data. Sensor value can be modified via Increase, Decrease member functions
class FakeSensor : public ISensor
{
	float _value;
public:
	FakeSensor()
	{
		_value=50;
	}
	///Returns sensor name. Only for debug purpose
	const __FlashStringHelper* Name()
	{
		return F("Sensor");
	}
	///Return how many decimal places make sence for the sensor
	int Precission()
	{
		return 0;
	}
	///Return lowest possible measurement limit. If value outside of this limit, measurements threatet as erroneous
	float LowMeasurementLimit()
	{
		return -100;
	}
	///Return highest possible measurement limit. If value outside of this limit, measurements threatet as erroneous
	float HighMeasurementLimit()
	{
		return 200;
	}
	///Perfrom measurements 
	/**
	\param data reference to the variable that becoms result measurement
	\return true if success
	*/
	bool Measure(float &data)
	{
		data=_value;
		return true;
	}
	///Increases internal value 
	void Increase()
	{
		_value++;
	}
	///Decreases internal value 
	void Decrease()
	{
		_value--;
	}
};
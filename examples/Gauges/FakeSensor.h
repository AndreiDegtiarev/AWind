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
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

#include "TimerSensorManager.h"

class ChargerController : public ISensorMeasuredEventReceiver
{
	SensorManager  *_currentMeter;
	SensorManager  *_voltageProbeTwo;
	SensorManager  *_temperature;
	int _mosfetPin;
	float _targetCurrent;
	float _cutoffVoltage;
	float _cutoffTemperature;
	int   _cutoffHours;
	bool  _isError;
	int   _outputMosfetValue;
public:
	ChargerController(SensorManager *currentMeter,
		              SensorManager *voltageProbeTwoPort, 
		              SensorManager *temperature, 
			          int mosfetPin, int mosfetStartValue,		
		              float targetCurrent,
				      float cutoffVoltage, float cutoffTemperature,
				      int cutoffHours)
	{
		_currentMeter = currentMeter;
		_voltageProbeTwo = voltageProbeTwoPort;
		_temperature = temperature;
		_mosfetPin = mosfetPin;
		_outputMosfetValue = mosfetStartValue;
		_targetCurrent = targetCurrent;
		_cutoffVoltage = cutoffVoltage;
		_cutoffTemperature = cutoffTemperature;
		_cutoffHours = cutoffHours;
		_isError = false;
		currentMeter->RegisterMeasuredEventReceiver(this);
		voltageProbeTwoPort->RegisterMeasuredEventReceiver(this);
	}

	void SetMosfettActive(bool isOn)
	{
		WriteMosfet(isOn && !_isError?_outputMosfetValue:0);
	}
	void WriteMosfet(int value)
	{
#ifdef ESP32
		ledcWrite(_mosfetPin, value);
#else
		analogWrite(_mosfetPin, value);
#endif
	}
	///Notification is call for each sensor measurements
	void NotifySensorMeasured(SensorManager *sensorManager)
	{
		if (sensorManager == _currentMeter)
		{
			if (_isError)
				WriteMosfet(0);
			else
			{
				//_outputMosfetValue = 0;
				WriteMosfet(_outputMosfetValue);
				out << F("Out mosfet value:") << _outputMosfetValue << endln;
			}

			float voltageProbeTwo = _voltageProbeTwo->GetData();
			float batteryVoltage = voltageProbeTwo;
			float current = _currentMeter->GetData();
			float currentError = _targetCurrent - current*1000;
			//out << F("Current error:") << currentError << endln;
			float hours = millis() / 1000 / 60 / 60;
			if (_temperature->GetData() > _cutoffTemperature)
			{
				out <<F("Max Temperature Exceeded:")<< _temperature->GetData()<< endln;
				_isError = true;
			}
			else if (batteryVoltage > _cutoffVoltage)
			{
				out <<F("Max Voltage Exceeded:")<< batteryVoltage  <<endln;
				currentError *= -1;
				//_outputMosfetValue = 100.0;
			}
			else if (hours > _cutoffHours)
			{
				out <<F("Max Charge Time Exceeded:") << hours<< endln;
				_isError = true;
			}
			if (abs(currentError) > 10)     //if output error is large enough, adjust output
			{
				_outputMosfetValue = _outputMosfetValue + currentError / 50;

				if (_outputMosfetValue < 1)    //output can never go below 0
					_outputMosfetValue = 0;
				else if (_outputMosfetValue > 254)     //output can never go above 255
					_outputMosfetValue = 255;
			}
		}

	}
};
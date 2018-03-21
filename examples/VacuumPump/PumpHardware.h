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

#include "DS18B20Sensor.h"
#include "MPXPressureSensor.h"

enum PumpStatus
{
	Off,
	On
};
class PumpHardware
{

private:
	static const uint32_t _pressurePin = A0;
	static const uint32_t _temperaturePin = 17;
	static const uint32_t _relayPin = 13;

	static const uint32_t _stepperEnablePin = 10;
	static const uint32_t _stepperDirPin = 12;
	static const uint32_t _stepperStepPin = 11;
	static const uint32_t _stepperM0Pin = 13;
	static const uint32_t _stepperM1Pin = 14;
	static const uint32_t _stepperM2Pin = 8;
	static const uint32_t _stepperMotorSteps = 200;

	SensorManager *_temperatureSensorManager;
	SensorManager *_pressureSensorManager;
	PumpStatus _status;
#ifndef DEMO_SENSORS
	DRV8825 _stepper;
#endif
public:
	PumpHardware()
#ifndef DEMO_SENSORS
		: _stepper(_stepperMotorSteps, _stepperDirPin, _stepperStepPin, _stepperM0Pin, _stepperM1Pin, _stepperM2Pin)
#endif
	{
		pinMode(_stepperEnablePin, OUTPUT);
		pinMode(_relayPin, OUTPUT);
	}
	void Initialize(LinkedList<SensorManager> &sensors, ISensorHasDataEventReceiver *receiver)
	{
		_temperatureSensorManager = new SensorManager(new DS18B20Sensor(_temperaturePin, 1), 10, 37, 1000 * 2,false);
		_temperatureSensorManager->initSecondsBuffer(1000);
		_pressureSensorManager = new SensorManager(new MPXPressureSensor(_pressurePin,0.25, 0.537634409), -1, 1, 1000 * 2, false);
		_pressureSensorManager->initSecondsBuffer(1000);

		sensors.Add(_temperatureSensorManager);
		sensors.Add(_pressureSensorManager);

		_temperatureSensorManager->RegisterHasDataEventReceiver(receiver);
		_pressureSensorManager->RegisterHasDataEventReceiver(receiver);
	}
protected:
	void rotateStepper(double angle)
	{
		digitalWrite(_stepperEnablePin, HIGH);
#ifndef DEMO_SENSORS
		_stepper.rotate(angle);
#endif
		digitalWrite(_stepperEnablePin, LOW);
	}
public:
	PumpStatus Status()
	{
		return _status;
	}
	void InitVacuumValve()
	{
#ifndef DEMO_SENSORS
		_stepper.setMicrostep(8);
		_stepper.setRPM(5);
#endif
		//connectVesselToVaccum();
		DisconnectVesselFromVacuum();
		delay(1 * 1000);
	}
	void OnPump()
	{
		_status = On;
		digitalWrite(_relayPin, LOW);
	}
	void OffPump()
	{
		_status = Off;
		digitalWrite(_relayPin, HIGH);
	}

	void DisconnectVesselFromVacuum()
	{
		rotateStepper(95);
	}
	void ConnectVesselToVaccum()
	{
		rotateStepper(-95);
	}
	SensorManager *TemperatureSensorManager()
	{
		return _temperatureSensorManager;
	}
	SensorManager *PressureSensorManager()
	{
		return _pressureSensorManager;
	}

};
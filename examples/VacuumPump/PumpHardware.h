/*
AWind.h - Arduino window library support for Color TFT LCD Boards
Copyright (C)2016 Andrei Degtiarev. All right reserved

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
		_temperatureSensorManager = new SensorManager(new DS18B20Sensor(_temperaturePin, 1), 10, 60, 1000 * 2,false);
		_temperatureSensorManager->initSecondsBuffer(1000);
		_pressureSensorManager = new SensorManager(new MPXPressureSensor(_pressurePin), 0.01, 10, 1000 * 2, false);
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
	void StartPump()
	{
		_status = On;
		digitalWrite(_relayPin, LOW);
	}
	void StopPump()
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
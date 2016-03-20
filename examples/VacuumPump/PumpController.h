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

///Implements pump control logic
class PumpController
{
	static const uint32_t _pumpRelayPin=13;
	static const uint32_t _stepperEnablePin = 10;
	static const uint32_t _stepperDirPin=12;
	static const uint32_t _stepperStepPin=11;
	static const uint32_t _stepperM0Pin=13;
	static const uint32_t _stepperM1Pin=14;
	static const uint32_t _stepperM2Pin=8;
	static const uint32_t _stepperMotorSteps=200;
#ifndef DEMO_SENSORS
	DRV8825 _stepper;
#endif
public:
	PumpController()
#ifndef DEMO_SENSORS
		: _stepper(_stepperMotorSteps, _stepperDirPin, _stepperStepPin, _stepperM0Pin, _stepperM1Pin, _stepperM2Pin)
#endif
	{
		pinMode(_stepperEnablePin, OUTPUT);
		pinMode(_pumpRelayPin, OUTPUT);
		stopPump();
	}
	void initVacuumValve()
	{
#ifndef DEMO_SENSORS
		_stepper.setMicrostep(8);
		_stepper.setRPM(5);
#endif
		//connectVesselToVaccum();
		disconnectVesselFromVaccum();
		delay(1 * 1000);
	}
	void disconnectVesselFromVaccum()
	{
		rotateStepper(95);
	}
	void connectVesselToVaccum()
	{
		rotateStepper(-95);
	}
	void startVacuum()
	{
		startPump();
		pause();
		connectVesselToVaccum();
	}
	void stopVacuum()
	{
		stopPump();
		pause();
		disconnectVesselFromVaccum();
	}
	void startPump()
	{
		digitalWrite(_pumpRelayPin, LOW);
	}
	void stopPump()
	{
		digitalWrite(_pumpRelayPin, HIGH);
	}
	void rotateStepper(double angle)
	{
		digitalWrite(_stepperEnablePin, HIGH);
#ifndef DEMO_SENSORS
        _stepper.rotate(angle);
#endif
		digitalWrite(_stepperEnablePin, LOW);
	}
	void pause()
	{
		delay(1000);
	}
};
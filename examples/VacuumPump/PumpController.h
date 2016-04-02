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

#include "TimerSensorManager.h"
#include "PumpHardware.h"


///Implements pump control logic
class PumpController : public ISensorHasDataEventReceiver
{
public:
	struct PumpControllerSettings
	{
		uint32_t PauseTime_ms;
		uint32_t ActiveTime_ms;
		float   MinPressure_bar;
		float   MaxPressure_bar;
	};
	enum PumpProgramm
	{
		ManualControl,
		TimerControll,
		PressureControll,
	};
	enum SensorType
	{
		Temperature,
		Pressure,
		ActiveTimer,
		PauseTimer,
	};
private:
	PumpControllerSettings _settings;
	TimerSensorManager  *_pauseTimerManager;
	TimerSensorManager  *_activeTimerManager;
	LinkedList<ISensorHasDataEventReceiver> _hasDataEventsReceiver;
	PumpProgramm _programm;
	PumpHardware _hardware;
	bool _isOverheatetd;

public:
	PumpController()
	{
		_programm = ManualControl;
		_isOverheatetd = false;
	}
	void Initialize(LinkedList<SensorManager> &sensors)
	{
		_pauseTimerManager = new TimerSensorManager();
		_activeTimerManager = new TimerSensorManager();

		sensors.Add(_pauseTimerManager);
		sensors.Add(_activeTimerManager);

		_pauseTimerManager->RegisterHasDataEventReceiver(this);
		_activeTimerManager->RegisterHasDataEventReceiver(this);
		_hardware.Initialize(sensors, this);
		StopPump();
	}
	void RegisterHasDataEventReceiver(ISensorHasDataEventReceiver *receiver)
	{
		_hasDataEventsReceiver.Add(receiver);
	}
	PumpControllerSettings & Settings()
	{
		return _settings;
	}
	PumpProgramm Programm()
	{
		return _programm;
	}
	void SetProgramm(PumpProgramm programm)
	{
		_programm = programm;
	}
	PumpStatus Status()
	{
		return _hardware.Status();
	}
	void StartPump()
	{
		if(!_isOverheatetd)
			_hardware.OnPump();
	}
	void StopPump()
	{
		_hardware.OffPump();
	}
	void startVacuum()
	{
		StartPump();
		pause();
		_hardware.ConnectVesselToVaccum();
	}
	void stopVacuum()
	{
		StopPump();
		pause();
		_hardware.DisconnectVesselFromVacuum();
	}
	void pause()
	{
		delay(1000);
	}
	///If sensor data was changed this notification is call
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		if (sensorManager == _hardware.TemperatureSensorManager())
		{
			if(sensorManager->Status() == MeasurementStatus::ApplicationAlarm)
			{
				StopPump();
				_isOverheatetd = true;
			}
			else if(sensorManager->GetData()<sensorManager->HightApplicationLimit() * 0.97)
			{
				_isOverheatetd = false;
			}
		}
		if (sensorManager == _activeTimerManager)
		{
			//out << F("PumpController::NotifySensorHasData _activeTimerManager")<<endln;
			stopVacuum();
			_activeTimerManager->Reset();
			_pauseTimerManager->Enable();
		}
		else if(sensorManager == _pauseTimerManager)
		{
			//out << F("PumpController::NotifySensorHasData _pauseTimerManager") << endln;
			_pauseTimerManager->Reset();
			_activeTimerManager->Enable();
			startVacuum();
		}
		for (int i = 0;i < _hasDataEventsReceiver.Count();i++)
			_hasDataEventsReceiver[i]->NotifySensorHasData(sensorManager);

	}
public:
	int32_t ElapsedActiveTime()
	{
		return _activeTimerManager->Elapsed();
	}
	int32_t ElapsedPauseTime()
	{
		return _pauseTimerManager->Elapsed();
	}
	SensorType GetSensorType(SensorManager *sensorManager)
	{
		if (sensorManager == _hardware.TemperatureSensorManager())
			return SensorType::Temperature;
		else if (sensorManager == _hardware.PressureSensorManager())
			return SensorType::Pressure;
		else if (sensorManager == _activeTimerManager)
			return SensorType::ActiveTimer;
		else if (sensorManager == _pauseTimerManager)
			return SensorType::PauseTimer;
	}
	void StartProgramm()
	{
		switch (Programm())
		{
		case ManualControl:
			StartPump();
			break;
		case TimerControll:
			_hardware.InitVacuumValve();
			_activeTimerManager->SetInterval(Settings().ActiveTime_ms);
			_pauseTimerManager->SetInterval(Settings().PauseTime_ms);
			startVacuum();
			_activeTimerManager->Enable();
			break;
		default:
			break;
		}
	}
	void StopProgramm()
	{
		switch (Programm())
		{
		case ManualControl:
			StopPump();
			break;
		case TimerControll:
			_activeTimerManager->Reset();
			_pauseTimerManager->Reset();
			stopVacuum();
			break;
		default:
			break;
		}
	}
};
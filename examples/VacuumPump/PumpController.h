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
		float   MaxTemperature;
	};
	enum PumpProgramm
	{
		ManualControl,
		TimerControll,
		PressureControll,
		Inactive,
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
		_programm = Inactive;
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
		InvalidateSettings();
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
	PumpHardware & Hardware()
	{
		return _hardware;
	}
	PumpProgramm Programm()
	{
		return _programm;
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
				_programm = Inactive;
				StopPump();
				_isOverheatetd = true;
			}
			else if(sensorManager->GetData()<sensorManager->HightApplicationLimit() * 0.97)
			{
				_isOverheatetd = false;
			}
		}
		if (Programm() == PressureControll)
		{
			if (sensorManager == _hardware.PressureSensorManager())
			{
				if(sensorManager->GetData()>Settings().MaxPressure_bar && Status() == PumpStatus::On)
					stopVacuum();
				else if (sensorManager->GetData()<Settings().MinPressure_bar && Status() == PumpStatus::Off)
					startVacuum();
			}
		}
		else if(Programm() == TimerControll)
		{
			if (sensorManager == _activeTimerManager)
			{
				//out << F("PumpController::NotifySensorHasData _activeTimerManager")<<endln;
				stopVacuum();
				_activeTimerManager->Reset();
				_pauseTimerManager->Enable();
			}
			else if (sensorManager == _pauseTimerManager)
			{
				//out << F("PumpController::NotifySensorHasData _pauseTimerManager") << endln;
				_pauseTimerManager->Reset();
				_activeTimerManager->Enable();
				startVacuum();
			}
		}
		for (int i = 0;i < _hasDataEventsReceiver.Count();i++)
			_hasDataEventsReceiver[i]->NotifySensorHasData(sensorManager);

	}
public:
	void InvalidateSettings()
	{
		_hardware.TemperatureSensorManager()->SetHightApplicationLimit(_settings.MaxTemperature);
	}
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
	void StartProgramm(PumpProgramm programm)
	{
		_programm = programm;
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
		case PressureControll:
			_hardware.InitVacuumValve();
			startVacuum();
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
		case PressureControll:
			stopVacuum();
			break;
		default:
			break;
		}
		_programm = Inactive;
	}
};
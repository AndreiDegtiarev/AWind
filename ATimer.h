#pragma once
/*
AWind.h - Arduino window library support for Color TFT LCD Boards
Copyright (C)2016 Andrei Degtiarev. All right reserved


You can always find the latest version of the library at
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
#include <ITimerEventReceiver.h>

/**Timer class. Implement timer events in window classes. 
If a  Window need to receive timer events it need to be derived from ITimerEventReceiver and NotifyTimer meber function has to be implemented
See VacuumPump example for details
*/
class ATimer
{
	ITimerEventReceiver *_timerEventReceiver;   //!< call back event receiver
	uint32_t _msStart;                          //!< time when timer was started
	uint32_t _msInterval;                       //!< timer interval
	bool _isAutoreset;                          //!< defines whether timer event will begenerated only once or continiously
	bool _isEnabled;                            //!< defines whether timer is active
public:
	ATimer()
	{
		_isAutoreset = false;
		_isEnabled = false;
		_timerEventReceiver = NULL;
	}
	///Registers event receiver.
	void RegisterTimerEventReceiver(ITimerEventReceiver *timerEventReceiver)
	{
		_timerEventReceiver = timerEventReceiver;
	}
	///Set whether timer is in autoreset modus
	void SetAutoReset(bool isAutoreset)
	{
		_isAutoreset = isAutoreset;
	}
	///Set timer interval in milliseconds
	void SetInterval(uint32_t milliseconds)
	{
		_msInterval = milliseconds;
	}
	///Returns true if timer is active
	bool IsEnabled()
	{
		return _isEnabled;
	}
	///Starts timer
	void Enable()
	{
		_isEnabled = true;
		_msStart = millis();
	}
	///Disabels timer
	void Reset()
	{
		_isEnabled = false;
	}
	///Returns actual elapsed time
	uint32_t Elapsed()
	{
		return millis() - _msStart;
	}
	//Is called by AWind framework 
	void loop()
	{
		if (_isEnabled && Elapsed() >= _msInterval && _timerEventReceiver != NULL)
		{
			_timerEventReceiver->NotifyTimer(this);
			if (_isAutoreset)
				Enable();
			else
				Reset();
		}

	}
};
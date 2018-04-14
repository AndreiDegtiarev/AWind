/*
GUI library for Arduino TFT and OLED displays

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

#include "TextBoxString.h"
#include "DecoratorPrimitives.h"

///Main window that controls child window appearance (day/night/alarm) for windows like sensor window. See sensors monitor example. To save board memory this class shares decoration code for child windows
class ViewModusWindow : public MainWindow,ITouchEventReceiver
{
public:
	enum Modus
	{
		Day,
		Night
	};
private:
	DecoratorList _dayLightDecorator;
	DecoratorList _dayLightSensorWndDecorator;
	DecoratorList _nightDecorator;
	DecoratorList _alarmDecorator;
	DecoratorList _chartDecorator;
	TextBoxFString *_text;
	Modus _modus;
public:
	///Constructor
	/**
	\param width screen width
	\param height screen height
	*/	
	ViewModusWindow(int width,int height):MainWindow(width,height)
	{
		AddDecorator(new DecoratorRectFill(Color::Black));
		_dayLightDecorator.Add(new DecoratorRectFill(Color::CadetBlue));
		_dayLightDecorator.Add(new DecoratorColor(Color::White));
		_dayLightSensorWndDecorator.Add(new DecoratorRectFill(Color::CadetBlue));
		_dayLightSensorWndDecorator.Add(new Decorator3DRect(Color::White,Color::Gray));
		_dayLightSensorWndDecorator.Add(new DecoratorColor(Color::White));
		_nightDecorator.Add(new DecoratorRectFill(Color::Black));
		_nightDecorator.Add(new DecoratorColor(Color::White));
		_alarmDecorator.Add(new DecoratorRectFill(Color::Red,false));
		_alarmDecorator.Add(new DecoratorColor(Color::White));
		_chartDecorator.Add(new DecoratorRectFill(Color::Black,false));

		//SetBackColor(Color::Black);
		_text=new TextBoxFString(width-100,height-45,95,35,F("Night"));
		_text->SetFont(F("Big"));
		_text->SetMargins(12,10);
		_modus=Day;
		AddChild(_text);
		this->RegisterTouchEventReceiver(this);
	}
	void Initialize()
	{
		updateBackColor();
	}
	DecoratorList *ChartDecorators()
	{
		return &_chartDecorator;
	}
	DecoratorList *AlarmDecorators()
	{
		return &_alarmDecorator;
	}
	DecoratorList *NormalDecorators()
	{
		return _modus==Day?&_dayLightDecorator:&_nightDecorator;
	}
	DecoratorList *NormalSensorWndDecorators()
	{
		return _modus==Day?&_dayLightSensorWndDecorator:&_nightDecorator;
	}
	void SetModus(Modus modus)
	{
		if(modus!=_modus)
			NotifyTouch(NULL);
	}
	///Switcher for day and night modus
	void NotifyTouch(Window *wnd)
	{
		if(_modus==Day)
		{
			_modus=Night;
			_text->SetText(F("Day"));
		}
		else
		{
			_modus=Day;	
			_text->SetText(F("Night"));
		}
		updateBackColor();
		Invalidate();
	}
protected:
	void updateBackColor()
	{
		for(int i=0;i<Children().Count();i++)
		{
			Children()[i]->SetDecorators(NormalDecorators());
			_text->SetDecorators(NormalSensorWndDecorators());
		}
	}
};

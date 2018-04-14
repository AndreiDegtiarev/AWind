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

#include "SensorManager.h"
#include "TextBoxNumber.h"
#include "TextBoxString.h"
#include "ChartWindow.h"
#include "ViewModusWindow.h"
#include "ISensorHasDataEventReceiver.h"


///Window that visualizes data from sensor in from from text or chart with different time scala (See example Sensors monitor)
class SensorWindow : public Window, ISensorHasDataEventReceiver
{
public:
	static const int Margin=10;
	static const int BigWindowHeight=70+Margin*2;
	static const int SmallWindowHeight=BigWindowHeight/2-Margin/4;
	static const int BigWindowWidth=135+Margin*2;
	static const int SmallWindowWidth=BigWindowWidth/2-Margin/4;

	enum WindowSize
	{
		Big=0,
		Small=1,
		User
	};

protected:
	enum VisMode
	{
		Text,
		ChartSec,
		ChartMin,
		ChartHowr
	};
	SensorManager *_sensorManager;
	TextBoxNumber *_textValue;
	TextBoxFString *_textName;
	TextBoxFString *_textChartAxis;
	ChartWindow *_chartWnd;
	VisMode _mode;

public:
	///Constructor
	/**
	\param name window title
	\param sensorManager provide interface to sensor object
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param size window size (big or small)
	*/	
	SensorWindow(const __FlashStringHelper * name,SensorManager *sensorManager,int left,int top,WindowSize size=Big):Window(name,left,top,size == Big?BigWindowWidth:SmallWindowWidth,size == Big?BigWindowHeight:SmallWindowHeight)
	{
		Initialize(name,sensorManager,size,Width());
	}
	SensorWindow(const __FlashStringHelper * name,SensorManager *sensorManager,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		Initialize(name,sensorManager,User,BigWindowWidth);
	}
protected:
	void Initialize(const __FlashStringHelper * name,SensorManager *sensorManager,WindowSize size,int textBoxWidth)
	{
		_sensorManager = sensorManager;
		_sensorManager->RegisterHasDataEventReceiver(this);
		_mode = Text;
		int offset = size != Small?Margin:Margin/2;
		int first_font_height=size != Small?70:30;


		_textValue = new TextBoxNumber(offset,offset,textBoxWidth-offset,first_font_height-offset,_sensorManager->Sensor()->Precission());
		_textValue->SetFont(Environment::Get()->FindFont(size != Small?F("BigPlus") : F("Big")));
		_textValue->SetStatus(false);

		_textName = new TextBoxFString(offset,first_font_height,textBoxWidth,1,name);
		_textName->SetFont(Environment::Get()->FindFont(size != Small? F("Big") :F("Small")));

		int chart_height=Height()-15;
		_textChartAxis=new TextBoxFString(Width()/2,chart_height,textBoxWidth,1,F(""));
		_textChartAxis->SetFont(Environment::Get()->FindFont(size != Small ? F("Big") : F("Small")));
		int chart_offset_x=0;
		_chartWnd = new ChartWindow(NULL,NULL,chart_offset_x,0,Width()-chart_offset_x,chart_height);
		_chartWnd->SetVisible(false);

		this->AddChild(_textValue);
		this->AddChild(_textName);
		this->AddChild(_chartWnd);
		this->AddChild(_textChartAxis);
	}
public:
	bool IsAwaitTouch()
	{
		return true;
	}
	///Sets appearance settings
	void SetDecorators(DecoratorList *decorators)
	{
		Window::SetDecorators(((ViewModusWindow *)Parent())->NormalSensorWndDecorators());
		_textValue->SetDecorators(decorators);
		_textName->SetDecorators(decorators);
		_textChartAxis->SetDecorators(decorators);
		_chartWnd->SetDecorators(((ViewModusWindow *)Parent())->ChartDecorators());
	}
	///Process touch event
	virtual bool OnTouch(int x, int y)
	{
		if(_mode == ChartHowr)
		{
			_textValue->SetVisible(true);
			_textName->SetVisible(true);
			_chartWnd->SetVisible(false);
			_textChartAxis->SetVisible(false);
			_mode=Text;
		}
		else if(_mode == Text)
		{
			_textValue->SetVisible(false);
			_textName->SetVisible(false);
			_chartWnd->SetVisible(true);
			_textChartAxis->SetVisible(true);
			_chartWnd->SetBuffer(_sensorManager->SecBuffer());
			_textChartAxis->SetText(F("sec"));
			_mode=ChartSec;
		}
		else if(_mode == ChartSec)
		{
			_chartWnd->SetBuffer(_sensorManager->MinBuffer());
			_textChartAxis->SetText(F("min"));
			_mode=ChartMin;
		}
		else if(_mode == ChartMin)
		{
			_chartWnd->SetBuffer(_sensorManager->HowrsBuffer());
			_textChartAxis->SetText(F("howr"));
			_mode=ChartHowr;
		}
		_textChartAxis->SetVisible(!_textValue->IsVisible());
		Invalidate();
		return true;
	}
	///If sensor data was changed this notification is call
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		_textValue->SetStatus(_sensorManager->Status()!=Error);
		float value=_sensorManager->GetData();
		if(_sensorManager->Status()!=Error)
		{
			ViewModusWindow *viewModus=(ViewModusWindow *)Parent();
			DecoratorList *prevDec=GetDecorators();
			if(_sensorManager->Status() == ApplicationAlarm)
			{
				SetDecorators(viewModus->AlarmDecorators());
				Window::SetDecorators(viewModus->AlarmDecorators());
			}
			else
				SetDecorators(viewModus->NormalDecorators());
			//_textValue->SetDecorators(GetDecorators());
			if(prevDec != GetDecorators() && _mode == Text)
				Invalidate();
		}
		if(_sensorManager->Status()!=Error && _textValue->GetNumber()!=value)
		{
			_textValue->SetNumber(value);
			_mode == Text?_textValue->Invalidate():_chartWnd->Invalidate();
		}
		else if(_sensorManager->Status()!=Error && _mode == ChartSec)
			_chartWnd->Invalidate();
	}
};
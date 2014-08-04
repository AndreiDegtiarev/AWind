/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  

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
#pragma once

#include "SensorManager.h"
#include "TextBoxNumber.h"
#include "TextBoxString.h"
#include "ChartWindow.h"
#include "ChartWindow.h"
#include "ISensorHasDataEventReceiver.h"


extern uint8_t ArialNumFontPlus[];
extern uint8_t BigFont[];
extern uint8_t SmallFont[];


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
		Big,
		Small
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
	Color _normalBkColor;

public:
	SensorWindow(const __FlashStringHelper * name,SensorManager *sensorManager,int left,int top,WindowSize size=Big):Window(name,left,top,size == Big?BigWindowWidth:SmallWindowWidth,size == Big?BigWindowHeight:SmallWindowHeight)
	{
		_sensorManager = sensorManager;
		_mode = Text;
		int offset = size == Big?Margin:Margin/2;
		int first_font_height=size == Big?70:30;

		_textValue = new TextBoxNumber(offset,offset,Width()-offset,first_font_height-offset,_sensorManager->Sensor()->Precission(),Color::White);
		_textValue->SetFont(size == Big?ArialNumFontPlus:BigFont);
		_textValue->SetStatus(false);
		_textValue->SetColor(Color::White);

		_textName = new TextBoxFString(offset,first_font_height,Width(),1,name,Color::White);
		_textName->SetFont(size == Big?BigFont:SmallFont);

		int chart_height=Height()-15;
		_textChartAxis=new TextBoxFString(Width()/2,chart_height,Width(),1,F(""),Color::White);
		_textChartAxis->SetFont(size == Big?BigFont:SmallFont);
		_chartWnd = new ChartWindow(0,0,Width(),chart_height);
		_chartWnd->SetVisible(false);
		_normalBkColor=Color::Black;

		this->AddChild(_textValue);
		this->AddChild(_textName);
		this->AddChild(_chartWnd);
		this->AddChild(_textChartAxis);
		sensorManager->RegisterHasDataEventReceiver(this);
	}
	bool IsAwaitTouch()
	{
		return true;
	}
	void SetBackColor(Color color)
	{
		Window::SetBackColor(color);
		_normalBkColor=color;
		_textValue->SetBackColor(GetBackColor());
	}

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
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		_textValue->SetStatus(_sensorManager->Status()!=Error);
		float value=_sensorManager->GetData();
		if(_sensorManager->Status()!=Error)
		{
			ARGB oldColor=GetBackColor().GetValue();
			if(_sensorManager->Status() == ApplicationAlarm)
				Window::SetBackColor(Color::Red);
			else
				Window::SetBackColor(_normalBkColor);
			_textValue->SetBackColor(GetBackColor());
			if(oldColor != GetBackColor().GetValue() && _mode == Text)
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
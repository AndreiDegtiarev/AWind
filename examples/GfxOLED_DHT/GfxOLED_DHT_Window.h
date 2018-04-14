/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
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

#include "TextBoxString.h"
#include "TextBoxNumber.h"
#include "DecoratorPrimitives.h"
#include "ChartWindow.h"

extern uint8_t SmallFont[];
extern uint8_t BigFont[];

class GfxOLED_DHT_Window : public MainWindow, public  ISensorHasDataEventReceiver
{
	TextBoxNumber *_texTempr;
	TextBoxNumber *_texHumid;
	SensorManager *_inTemprManager;

public:
	GfxOLED_DHT_Window(int width,int height):MainWindow(width,height)
	{
		int x = 20;
		int y = 2;
		auto labelTitle = new TextBoxFString(x, y, width / 2, 20, F("Demo DHT sensor"), F("Label"));
		labelTitle->SetFont(F("Small"));
		AddChild(labelTitle);
		//x = 2;
		y = 15;
		_texTempr = initNumberFields(y, width, F("Temp:"));
		//auto labelT=new TextBoxFString(x,y,width/2,20,F("Temp:"), F("Label"));
		//labelT->SetMargins(5, 2);
		//labelT->SetFont(BigOledFont);
		//x=3*width/4-12;
		//_texTemr =new TextBoxNumber(x,y,width/4+8,20,0);
		//_texTemr->SetFont(BigOledFont);
		//_texTemr->SetMargins(10,3);
		//_texTemr->SetNumber(25);

		//AddChild(labelT);
		//AddChild(_texTemr);

		//x = 2;
		y += 25;
		_texHumid = initNumberFields(y, width, F("Humid:"));
		//auto labelH = new TextBoxFString(x, y, width / 2, 20, F("Humid:"), F("Label"));
		//labelH->SetMargins(5, 2);
		//labelH->SetFont(BigOledFont);
		//x = 3 * width / 4 - 12;
		//_texHumid = new TextBoxNumber(x, y, width / 4 + 8, 20, 0);
		//_texHumid->SetFont(BigOledFont);
		//_texHumid->SetMargins(10, 3);
		//_texHumid->SetNumber(65);

		//AddChild(labelH);
		//AddChild(_texHumid);
	}
	TextBoxNumber *initNumberFields(int y,int width, const __FlashStringHelper *name)
	{
		int x = 2;
		auto label = new TextBoxFString(x, y, width / 2, 20, name, F("Label"));
		label->SetMargins(5, 2);
		label->SetFont(F("Big"));
		x = 3 * width / 4 - 12;
		auto text = new TextBoxNumber(x, y, width / 4 + 8, 20, 0);
		text->SetFont(F("Big"));
		text->SetMargins(10, 3);
		text->SetNumber(0);

		AddChild(label);
		AddChild(text);
		return text;
	}
	void Initialize(SensorManager *inTemprManager)
	{
		_inTemprManager = inTemprManager;
	}
	///Event sensor has new data. If data is the same as measured previosly. This event is not generated (see RegisterReceiver and public ISensorHasDataEventReceiver declaration)
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		float value = sensorManager->GetData();
		if (sensorManager == _inTemprManager)
		{
			_texTempr->SetNumber(value);
		}
		else
			_texHumid->SetNumber(value);
	}
};
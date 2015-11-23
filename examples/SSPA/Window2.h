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
#include <Window.h>
#include "LM35Sensor.h"
#include "ISensor.h"
#include "LinkedList.h"
#include "TextBoxNumber.h"
#include "SensorManager.h"

#include "TextBoxString.h"



//pin on Arduino where temperature sensor is connected (there are three DC/DC Modules with integrated LM35)
int temperature_port=0;

///Window with detail temperatur informations
class Window2 : public Window, public ISensorHasDataEventReceiver 
{
	LinkedList<SensorManager> _sensorManagers;
public:
	Window2(const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		AddDecorator(new DecoratorRectFill(Color::DeepSkyBlue,false));
		AddDecorator(new DecoratorColor(Color::Black));

		int x=0;
		int y=0;
		TextBoxFString *label=new TextBoxFString(x+10,y+10,270/2,25,F("DC1: "));
		TextBoxFString *label1=new TextBoxFString(x+10,y+35,270/2,25,F("DC2: "));
		TextBoxFString *label2=new TextBoxFString(x+10,y+60,270/2,25,F("DC3: "));
		label->SetFont(BigFont);
		label1->SetFont(BigFont);
		label2->SetFont(BigFont);


		TextBoxNumber * txtNumber1=new TextBoxNumber(x+80,y+10,60,16,0);
		TextBoxNumber * txtNumber2=new TextBoxNumber(x+80,y+35,60,16,0);
		TextBoxNumber * txtNumber3=new TextBoxNumber(x+80,y+60,60,16,0);

		// txtNumber1->SetDecorators->(Environment::Get()->FindDecorators(F("Label"))); //error: expected unqualified-id before '(' token  txtNumber1->SetDecorators->(Environment::Get()->FindDecorators(F("Label")));
		txtNumber1->AddDecorator(new DecoratorRectFill(Color::LightGreen));
		txtNumber1->AddDecorator(new Decorator3DRect(Color::DarkGray,Color::White));
		txtNumber1->AddDecorator(new DecoratorColor(Color::Black));    
		txtNumber2->SetDecorators(txtNumber1->GetDecorators());
		txtNumber3->SetDecorators(txtNumber1->GetDecorators());

		//!!!Important!!! Connection between sensors and window children work only if txtNumber-s are added first as children of the window.
		txtNumber1->SetNumber(0); //this should show the temp from modul1 -> LM35 Sensor
		txtNumber2->SetNumber(0); //this should show the temp from modul2 -> LM35 Sensor
		txtNumber3->SetNumber(0); //this should show the temp from modul3 -> LM35 Sensor

		AddChild(txtNumber1);  //child window with index 0
		AddChild(txtNumber2);  //child window with index 1 
		AddChild(txtNumber3);  //child window with index 2 

		AddChild(label);
		AddChild(label1);
		AddChild(label2);
	}
	void AttachSensor(SensorManager *sensorManager)
	{
		sensorManager->RegisterHasDataEventReceiver(this);
		_sensorManagers.Add(sensorManager);
	}
	void NotifySensorHasData(SensorManager *sensorManager)
	{
		if(sensorManager->Status()!=Error)
		{
			int index=0; 
			for(;index<_sensorManagers.Count();index++)
			{
				if(_sensorManagers[index]==sensorManager)
				{
					break;
				}
			}
			if(index<_sensorManagers.Count())
			{
				((TextBoxNumber *)_children[index])->SetNumber(sensorManager->GetData());
			}
		}
	}
};

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
#include "Button.h"
#include "ButtonWindow.h"
#include "LinkedList.h"

///Window with all known results and life power output information
class Window1 : public Window, public ITouchEventReceiver
{
	Button * _btn1;
	int _outPin;
    int relay_state;
	DecoratorList _onlineDecorator;
	DecoratorList _standbyDecorator;
public:
	Window1(const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		AddDecorator(new DecoratorRectFill(Color::DeepSkyBlue,false));
		AddDecorator(new DecoratorColor(Color::Black));

		//Buttons should set in this project a pinoutput for a relais
		_onlineDecorator.Add(new DecoratorRectFill(Color::LightGreen));  
		_onlineDecorator.Add(new Decorator3DRect(Color::DarkGray,Color::White));
		_onlineDecorator.Add(new DecoratorColor(Color::Black)); 

		_standbyDecorator.Add(new DecoratorRectFill(Color::Red));
        _standbyDecorator.Add(new Decorator3DRect(Color::White,Color::DarkGray));
        _standbyDecorator.Add(new DecoratorColor(Color::Black)); 


		_btn1=new Button(0,0,0,0,F("STANDBY"));
		InitButton(_btn1,10,180,120);
        _outPin = 8;
        pinMode(_outPin,OUTPUT);
        relay_state = LOW;
	}

	///Create child button
	void InitButton(Button *_btn,int left,int top,int width)
	{
        _btn1->SetDecorators(&_standbyDecorator);
		_btn->Move(left,top,width,25);
		_btn->SetMargins(5,5);
		_btn->RegisterTouchEventReceiver(this);
		AddChild(_btn);  

	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		if(window == _btn1)
		{
			if(relay_state == LOW) { 
				relay_state = HIGH;
				_btn1->SetText(F("ONLINE"));
				_btn1->SetDecorators(&_onlineDecorator);
				// out<<F("high")<<endln; //Just for test
			}
			else {
				relay_state = LOW;
				_btn1->SetText(F("STANDBY"));
				_btn1->SetDecorators(&_standbyDecorator);
				// out<<F("low")<<endln; //Just for test
			}
			analogWrite(_outPin, relay_state);
			//  out<<relay_state<<endln; //Just for test
		}
	}

};

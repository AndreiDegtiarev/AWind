#pragma once
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
///This class initializes sandart apperance for window, buttons, edit box and other controls in AWind library. If you need your another look just deefine your own resources file
class DefaultDecorators
{
public:
	static void InitAll()
	{
		InitMainWindow();
		InitButton();
		InitDialog();
		InitEditBox();
		InitLabel();
		InitGauge();
	}
	static void InitMainWindow()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::LightGray,false));
		list->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("Window"),list);
	}
	static void InitButton()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::LightGray,false));
		list->Add(new Decorator3DRect(Color::White,Color::Gray));
		list->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("Button"),list);
	}
	static void InitDialog()
	{
		Environment::Get()->RegisterDecoratorsGroup(F("Dialog"),Environment::Get()->FindDecorators(F("Button")));
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::LightBlue,false));
		list->Add(new DecoratorColor(Color::White));
		Environment::Get()->RegisterDecoratorsGroup(F("DialogCaption"),list);
	}
	static void InitLabel()
	{
		Environment::Get()->RegisterDecoratorsGroup(F("Label"),Environment::Get()->FindDecorators(F("Window")));
	}
	static void InitEditBox()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::Black));
		list->Add(new Decorator3DRect(Color::Gray,Color::White));
		list->Add(new DecoratorColor(Color::White));
		Environment::Get()->RegisterDecoratorsGroup(F("EditTextBox"),list);

		list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::LightGray));
		list->Add(new Decorator3DRect(Color::Gray,Color::White));
		list->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("EditTextBoxReadOnly"),list);

	}
	static void InitGauge()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new Decorator3DRect(Color::White,Color::Gray));
		list->Add(new DecoratorColor(Color::Green));
		Environment::Get()->RegisterDecoratorsGroup(F("Gauge"),list);
	}
};
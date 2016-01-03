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
#include "DecoratorPrimitives.h"
///This class initializes default apperance for window, buttons, edit box and other controls in AWind library. If you need your another look just deefine your own resources file
class DefaultDecorators
{
public:
	enum InitMode
	{
		all,
		none
	};
	DefaultDecorators(InitMode mode=DefaultDecorators::none)
	{
		if(mode == DefaultDecorators::all)
			DefaultDecorators::InitAll();
	}
	static void InitAll()
	{
		InitMainWindow();
		InitButton();
		InitDialog();
		InitEditBox();
		InitLabel();
		InitGauge();
		InitTabControl();
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
		//list->Add(new DecoratorRectFill(Color::LightGray,false));
		list->Add(new DecoratorRectGradientFill(Color::Gray,Color::WhiteSmoke));
		list->Add(new Decorator3DRect(Color::White,Color::Black));
		list->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("Button"),list);
	}
	static void InitDialog()
	{
		DecoratorList *listDialog=new DecoratorList();
		listDialog->Add(new DecoratorRectFill(Color::LightGray,false));
		listDialog->Add(new Decorator3DRect(Color::White,Color::Gray));
		listDialog->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("Dialog"),listDialog);
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
	static void InitTabControl()
	{
		DecoratorList *list = new DecoratorList();
		list->Add(new DecoratorRectFill(Color::LightGray, false));
		list->Add(new DecoratorColor(Color::Gray));
		list->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Left));
		list->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Top));
		list->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Right));
		list->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("ActiveTabButton"), list);

		DecoratorList *list_ctrl = new DecoratorList();
		list_ctrl->Add(new DecoratorRectFill(Color::LightGray, false));
		list_ctrl->Add(new DecoratorColor(Color::Gray));
		list_ctrl->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Bottom));
		Environment::Get()->RegisterDecoratorsGroup(F("BackTabButtons"), list_ctrl);

		DecoratorList *list_tab = new DecoratorList();
		list_tab->Add(new DecoratorRectFill(Color::LightGray, false));
		list_tab->Add(new Decorator3DRect(Color::Gray, Color::Gray));
		//list_tab->Add(new DecoratorColor(Color::Gray));
		//list_tab->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Left));
		Environment::Get()->RegisterDecoratorsGroup(F("TabControl"), list_tab);
	}
};
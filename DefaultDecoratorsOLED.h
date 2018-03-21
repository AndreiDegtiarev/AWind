#pragma once
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
#include "DecoratorPrimitives.h"
///This class initializes default apperance for window, buttons, edit box and other controls in AWind library. If you need your another look just deefine your own resources file
class DefaultDecoratorsOLED
{
public:
	enum InitMode
	{
		all,
		none
	};
	DefaultDecoratorsOLED(InitMode mode= DefaultDecoratorsOLED::none)
	{
		if(mode == DefaultDecoratorsOLED::all)
			DefaultDecoratorsOLED::InitAll();
	}
	static void InitAll()
	{
		InitMainWindow();
		InitButton();
		//RadioButton();
		//InitDialog();
		InitEditBox();
		InitLabel();
		InitGauge();
		//InitTabControl();
	}
	static void InitMainWindow()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::White,false));
		list->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("Window"),list);
	}
	static void InitButton()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::White));
		list->Add(new DecoratorColor(Color::Black));
		Environment::Get()->RegisterDecoratorsGroup(F("Button"),list);
	}
	//static void RadioButton()
	//{
	//	DecoratorList *list = new DecoratorList();
	//	list->Add(new DecoratorRectFill(Color::LightGray, false));
	//	list->Add(new Decorator3DSquare(Color::Gray, Color::White, Color::LightGray, false));
	//	list->Add(new DecoratorColor(Color::Black));
	//	Environment::Get()->RegisterDecoratorsGroup(F("RadioButtonInactive"), list);
	//	DecoratorList *list_1 = new DecoratorList();
	//	list_1->Add(new DecoratorRectFill(Color::LightGray, false));
	//	list_1->Add(new Decorator3DSquare(Color::Gray, Color::White, Color::LightGray, true));
	//	list_1->Add(new DecoratorColor(Color::Black));
	//	Environment::Get()->RegisterDecoratorsGroup(F("RadioButtonActive"), list_1);
	//}
	//static void InitDialog()
	//{
	//	DecoratorList *listDialog=new DecoratorList();
	//	listDialog->Add(new DecoratorRectFill(Color::LightGray,false));
	//	listDialog->Add(new Decorator3DRect(Color::White,Color::Gray));
	//	listDialog->Add(new DecoratorColor(Color::Black));
	//	Environment::Get()->RegisterDecoratorsGroup(F("Dialog"),listDialog);
	//	DecoratorList *list=new DecoratorList();
	//	list->Add(new DecoratorRectFill(Color::LightBlue,false));
	//	list->Add(new DecoratorColor(Color::White));
	//	Environment::Get()->RegisterDecoratorsGroup(F("DialogCaption"),list);
	//}
	static void InitLabel()
	{
		Environment::Get()->RegisterDecoratorsGroup(F("Label"),Environment::Get()->FindDecorators(F("Window")));
	}
	static void InitEditBox()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::Black));
		//list->Add(new Decorator3DRect(Color::Gray,Color::White));
		list->Add(new DecoratorColor(Color::White));
		Environment::Get()->RegisterDecoratorsGroup(F("EditTextBox"),list);
		Environment::Get()->RegisterDecoratorsGroup(F("EditTextBoxReadOnly"), Environment::Get()->FindDecorators(F("EditTextBox")));

		//list=new DecoratorList();
		//list->Add(new DecoratorRectFill(Color::LightGray));
		//list->Add(new Decorator3DRect(Color::Gray,Color::White));
		//list->Add(new DecoratorColor(Color::Black));
		//Environment::Get()->RegisterDecoratorsGroup(F("EditTextBoxReadOnly"),list);

	}
	static void InitGauge()
	{
		DecoratorList *list=new DecoratorList();
		list->Add(new DecoratorRectFill(Color::Black));
		list->Add(new DecoratorColor(Color::White));
		Environment::Get()->RegisterDecoratorsGroup(F("Gauge"),list);
	}
	//static void InitTabControl()
	//{
	//	DecoratorList *list = new DecoratorList();
	//	list->Add(new DecoratorRectFill(Color::LightGray, false));
	//	list->Add(new DecoratorColor(Color::Gray));
	//	list->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Left));
	//	list->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Top));
	//	list->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Right));
	//	list->Add(new DecoratorColor(Color::Black));
	//	Environment::Get()->RegisterDecoratorsGroup(F("ActiveTabButton"), list);

	//	DecoratorList *list_ctrl = new DecoratorList();
	//	list_ctrl->Add(new DecoratorRectFill(Color::LightGray, false));
	//	list_ctrl->Add(new DecoratorColor(Color::Gray));
	//	list_ctrl->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Bottom));
	//	Environment::Get()->RegisterDecoratorsGroup(F("BackTabButtons"), list_ctrl);

	//	DecoratorList *list_tab = new DecoratorList();
	//	list_tab->Add(new DecoratorRectFill(Color::LightGray, false));
	//	list_tab->Add(new Decorator3DRect(Color::Gray, Color::Gray));
	//	//list_tab->Add(new DecoratorColor(Color::Gray));
	//	//list_tab->Add(new DecoratorBoundaryLine(DecoratorBoundaryLine::Left));
	//	Environment::Get()->RegisterDecoratorsGroup(F("TabControl"), list_tab);
	//}
};
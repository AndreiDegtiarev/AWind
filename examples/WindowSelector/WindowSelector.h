/*
This file is part of AWind library

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
#include "MainWindow.h"
#include "Button.h"

///Window selector main window. It works as kind of tab control and can be used more or less without modifications in the target application 
class WindowSelector : public MainWindow, public ITouchEventReceiver
{
	LinkedList<Button> _listButtons; //list of buttons on the left scrren side
	LinkedList<Window> _listWindow;          //list of depended windows (
public:
	WindowSelector(int wnd_width,int wnd_height):MainWindow(wnd_width,wnd_height)
	{
	}
	///Adds pair: button + corresponding window
	void AddTab(const __FlashStringHelper *buttonName,Window *window)
	{
		int wnd_width=Width();
		int wnd_height=Height();
		int szx=110;
		int szy=40;
		int x=0;
		int y=0;
		Button * button=new Button(x+10,(szy+10)*(_listButtons.Count())+20,szx,szy,buttonName);
		button->SetMargins(5,15);
		button->RegisterTouchEventReceiver(this);
		AddChild(button);
		AddChild(window);
		window->Move(szx+25,0,wnd_width-szx-25,wnd_height);
		_listButtons.Add(button);
        _listWindow.Add(window);
		if(_listWindow.Count()>1)
			window->SetVisible(false);
		else
			_listButtons[0]->SetDecorators(_listWindow[0]->GetDecorators());

	}
	void Initialize()
	{
		AddDecorator(new DecoratorRectFill(Color::LightGray,false));
		AddDecorator(new DecoratorColor(Color::Black));

	}
	///Events routing for gui interaction (see RegisterTouchEventReceiver and public ITouchEventReceiver declaration)
	void NotifyTouch(Window *window)
	{
		int sel_index=-1;
		for(int i=0;i<_listButtons.Count();i++)
		{
			if(window == _listButtons[i])
			{
				sel_index=i;
				break;
			}
		}
		if(sel_index >=0)
		{
			DecoratorList *buttonDeco=Environment::Get()->FindDecorators(F("Button"));
			for(int i=0;i<_listButtons.Count();i++)
			{
				_listWindow[i]->SetVisible(i==sel_index);
				if(i==sel_index)
				{
					_listWindow[i]->Invalidate();
					_listButtons[i]->SetDecorators(_listWindow[i]->GetDecorators());
				}
				else
				{
					if(_listButtons[i]->GetDecorators()!=buttonDeco)
					{
						_listButtons[i]->SetDecorators(buttonDeco);
						_listButtons[i]->Invalidate();
					}
				}
			}
		}
		//out<<F("Window selected: ")<<sel_index<<endl;
	}
};
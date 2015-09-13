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
			for(int i=0;i<_listButtons.Count();i++)
			{
				_listWindow[i]->SetVisible(i==sel_index);
				if(i==sel_index)
					_listWindow[i]->Invalidate();
			}
		}
		//out<<F("Window selected: ")<<sel_index<<endl;
	}
};
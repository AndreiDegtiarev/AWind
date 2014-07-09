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

#include "Color.h"

#include "LinkedList.h"
#include "Log.h"
#include "DC.h"
#include "IEvent.h"

class Window
{
protected:
	const __FlashStringHelper *_type;
	int _left;
	int _top;
	int _width;
	int _height;
	bool _isVisible;
	Color _borderColor;
	Color _backColor;
	LinkedList<Window> _children;
	Window *_parent;
	const __FlashStringHelper *_name;
	bool _isDirty;

	IEvent<Window> *_touchEvent;

public:
	Window(const __FlashStringHelper * name,int left,int top,int width,int height):
																	_left(left),
																	_top(top),
																	_width(width),
																	_height(height),
																	_isVisible(true),
																	_borderColor(Color(VGA_TRANSPARENT)),
																	_backColor(Color(VGA_TRANSPARENT)),
																	_name(name)

	{
		_parent = NULL;
		_isDirty=true;
		_touchEvent=NULL;
		_type=F("Window");
	}
	bool IsOfType(const __FlashStringHelper * type);
	void SetOnTouch(IEvent<Window> *event)
	{
		_touchEvent=event;
	}
	virtual bool IsAwaitTouch()
	{
		return _touchEvent!=NULL;
	}
	virtual void OnTouching(DC *dc)
	{
		dc->SetColor(Color::Red);
		dc->DrawRoundRect(0,0,Width(),Height());
		dc->DrawRoundRect(1,1,Width()-1,Height()-1);
	}
	virtual bool OnTouch(int x,int y)
	{
		out<<F("OnTouch")<<endl;
		if(_touchEvent!=NULL)
		{
			out<<F("TouchEvent generated")<<endl;
			_touchEvent->Notify(this);
			return true;
		}
		return false;
	}

	void PrepareDC(DC *dc)
	{
		//Serial.println(F("PrepareDC"));
		dc->Reset();
		Window *crWnd=this;
		while(crWnd!=NULL)
		{
			//Serial.println(crWnd->Name());
			dc->Offset(crWnd->Left(),crWnd->Top());
			crWnd=crWnd->Parent();
		}
		//Serial.println(F("End PrepareDC"));
	}
	const __FlashStringHelper * GetType()
	{
		return _type;
	}
	/*Window *HitTest(int x,int y)
	{
		if(IsVisible()
			&&x>=Left() && x<=Left()+Width()
			&&y>=Top() && y<=Top()+Height())
		{
			for(int i=0;i<Children().Count();i++)
			{
				Window * retWnd=Children()[i]->HitTest(x-Left(),y-Top());
				if(retWnd!=NULL)
					return retWnd;
			}
			return this;
		}
		return NULL;
	}*/
	void Invalidate()
	{
		//out<<F("Invalidate: ")<<GetType()<<endl;
		_isDirty=true;
	}
	bool IsDirty()
	{
		return _isDirty;
	}
	const __FlashStringHelper *Name()
	{
		return _name;
	}
	int Left()
	{
		return _left;
	}
	int Top()
	{
		return _top;
	}
	int Width()
	{
		return _width;
	}
	int Height()
	{
		return _height;
	}
	void AddChild(Window * window)
	{
		window->SetParent(this);
		_children.Add(window);
	}
	void SetParent(Window *window)
	{
		_parent = window;
	}
	Window * Parent()
	{
		return _parent;
	}
	void SetBackColor(Color color)
	{
		_backColor=color;
	}
	void SetBorder(Color color)
	{
		_borderColor=color;
	}

	void SetVisible(bool isVisible)
	{
		_isVisible=isVisible;

	}
	bool IsVisible()
	{
		return _isVisible;
	}
	LinkedList<Window> Children()
	{
		return _children;
	}
	virtual void OnDraw(DC *dc)
	{
		//Serial.print(F("Begin OnDraw "));
		//Serial.println(Name());
		_isDirty=false;
		if(_backColor.GetValue() != VGA_TRANSPARENT)
		{
			dc->SetBackColor(_backColor);
			dc->FillRoundRect (0, 0, _width, _height);
		}
		if(_borderColor.GetValue() != VGA_TRANSPARENT)
		{
			dc->SetColor(_borderColor);
			dc->DrawRoundRect (0, 0, _width, _height);
		}
	}
};
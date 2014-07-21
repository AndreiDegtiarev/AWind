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
#include "ITouchEventReceiver.h"

class MainWindow;

class Window
{
protected:
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

	ITouchEventReceiver *_touchEventReceiver;

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
		_touchEventReceiver=NULL;
	}
	void RegisterTouchEventReceiver(ITouchEventReceiver *touchEventReceiver)
	{
		_touchEventReceiver=touchEventReceiver;
	}
	virtual bool IsAwaitTouch()
	{
		return _touchEventReceiver!=NULL;
	}
	virtual void OnTouching(DC *dc)
	{
		dc->SetColor(Color::Red);
		dc->DrawRoundRect(0,0,Width(),Height());
		dc->DrawRoundRect(1,1,Width()-1,Height()-1);
	}
	virtual bool OnTouch(int x,int y)
	{
		//out<<F("OnTouch")<<endl;
		if(_touchEventReceiver!=NULL)
		{
			//out<<F("TouchEvent generated")<<endl;
			_touchEventReceiver->NotifyTouch(this);
			return true;
		}
		return false;
	}
	MainWindow *MainWnd()
	{
		Window *parent=this;
		while(parent->Parent()!=NULL)
		{
			parent=parent->Parent();
		}
		return (MainWindow *)(parent);
	}
	virtual void Initialize()
	{

	}
	virtual void Move(int left,int top,int width,int height)
	{
		_left = left;
		_top = top;
		_width = width;
		_height = height;
	}
	void PrepareDC(DC *dc)
	{
		dc->Reset();
		Window *crWnd=this;
		while(crWnd!=NULL)
		{
			dc->Offset(crWnd->Left(),crWnd->Top());
			crWnd=crWnd->Parent();
		}
	}
	void Invalidate()
	{
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
	virtual void SetBackColor(Color color)
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
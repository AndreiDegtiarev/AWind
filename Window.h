/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2015 Andrei Degtiarev. All right reserved
  

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

#include "Log.h"
#include "DC.h"
#include "ITouchEventReceiver.h"
#include "Decorator.h"
#include "IDialogClosedEventReceiver.h"
#include "Environment.h"

class Dialog;

///Base class for all window objects. Provides basic window functionality
class Window
{
protected:
	int _left;				//!< window left coordinate relative to the parent window
	int _top;				//!< window top coordinate relative to the parent window
	int _width;				//!< window width
	int _height;            //!< window height
	bool _isVisible;        //!< if this variable is false this window and all child windows are not visualized 
	LinkedList<Window> _children;   //!< list of children windows. All children window are positioned relative to parent window
	Window *_parent;                //!< pointer to parent window
//#ifdef DEBUG_AWIND
	//const __FlashStringHelper *_name;  //!< internal window name that helps by debugging. For some reason preprocessor definiton #ifdef DEBUG_AWIND does not work here. So if you define debug you dhould uncomment this line
//#endif
	bool _isDirty;                     //!< if true than window manager will redraw this window. 
	DecoratorList *_decorators;//!< contains list of drawig commands. If they are shared between more than one window -> SRAM usage optimisation
	ITouchEventReceiver *_touchEventReceiver;   //!< call back event receiver for touch actions

public:
	///Constructor
	/**
	\param name internal window name that help by debugging
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	Window(const __FlashStringHelper * name,int left,int top,int width,int height):
																	_left(left),
																	_top(top),
																	_width(width),
																	_height(height),
																	_isVisible(true)
#ifdef DEBUG_AWIND
																	,_name(name)
#endif
	{
		_parent = NULL;
		_isDirty=true;
		_decorators=NULL;
		_touchEventReceiver=NULL;
	}
	///Sets window decorators list.
	virtual void SetDecorators(DecoratorList *decorators)
	{
		_decorators=decorators;
	}
	///Returns window decorators list.
	DecoratorList * GetDecorators()
	{
		return _decorators;
	}
	///Adds decorator to the decaorator list.
	void AddDecorator(Decorator *decorator)
	{
		if(_decorators == NULL)
			_decorators=new DecoratorList();
		_decorators->Add(decorator);
	}
	Dialog *FindDialog(const __FlashStringHelper *id);
	IDialogClosedEventReceiver::DialogResults DoDialog(Dialog *dlg);


	///Registers receiver for touch event
    /** Receiver can be a general class that is derived from ITouchEventReceiver. In this calss NotifyTouch virtual function has to be ovveriden */
	/**
	\param TouchEventReceiver pointer to touch event receiver
	*/
	void RegisterTouchEventReceiver(ITouchEventReceiver *touchEventReceiver)
	{
		_touchEventReceiver=touchEventReceiver;
	}
	///Returns true if window await touch action (like button) or false if touch manager should ignore this window during processing of touch events
	virtual bool IsAwaitTouch()
	{
		return _touchEventReceiver!=NULL;
	}
	///Touch manager calls this function in the loop as long as touch action proceeds
	virtual void OnTouching(DC *dc)
	{
		//out<<"OnTouching"<<endl;
		PrepareDC(dc);
		dc->SetColor(Color::Red);
		dc->DrawRoundRect(0,0,Width(),Height());
		dc->DrawRoundRect(1,1,Width()-1,Height()-1);
	}
	///Touch manager calls this function right after touch is released
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
	///Returns pointer to root window. MainWindow does not have any parents
	Window *RootWindow()
	{
		Window *parent=this;
		while(parent->Parent()!=NULL)
		{
			parent=parent->Parent();
		}
		return parent;
	}
	///Moves and resizes window relativly to the parent window 
	/**
	\param left left coordinate relative to parent window
	\param top top coordinate relative to parent window
	\param width window width
	\param height window height
	*/
	virtual void Move(int left,int top,int width,int height)
	{
		_left = left;
		_top = top;
		_width = width;
		_height = height;
	}
	///If function is called than the window manager updates the window
	void Invalidate()
	{
		_isDirty=true;
	}
	///Returns true if window has to be updated
	bool IsDirty()
	{
		return _isDirty;
	}
#ifdef DEBUG_AWIND
	///Returns internal window name
	const __FlashStringHelper *Name()
	{
		return _name;
	}
#endif
	///Returns window left coordinate relative to the parent window
	int Left()
	{
		return _left;
	}
	///Returns window top coordinate relative to the parent window
	int Top()
	{
		return _top;
	}
	///Returns window width
	int Width()
	{
		return _width;
	}
	///Returns window height
	int Height()
	{
		return _height;
	}
	///Adds window child window. 
	void AddChild(Window * window)
	{
		window->SetParent(this);
		_children.Add(window);
	}
	///Returns Parent window
	Window * Parent()
	{
		return _parent;
	}
	///Sets window visibility status
	void SetVisible(bool isVisible)
	{
		_isVisible=isVisible;

	}
	///Returns true if window visible and false is hidden
	bool IsVisible()
	{
		return _isVisible;
	}
	///Returns list of children window
	LinkedList<Window> Children()
	{
		return _children;
	}
	///Performs full window redraw
	/**
	\param dc device context
	*/
	void Redraw(DC *dc)
	{
		PrepareDC(dc);
		_isDirty=false;
		if(_decorators!=NULL)
		{
			for(int i=0;i<_decorators->Count();i++)
			{
				(*_decorators)[i]->Draw(dc,0,0,_width,_height);
			}
		}
		OnDraw(dc);
	}
protected:
	///Setups window coordinate system. This function called by window manager right before window has to be redrawn and it is intended for internal use
	/**
	\param dc device context
	*/
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
	///If derived class needs to draw something in window client area, this function has to be ovverriden 
	/**
	\param dc device context
	*/
	virtual void OnDraw(DC *dc)
	{
	}
private:
	///Adds window child window. 
	void SetParent(Window *window)
	{
		_parent = window;
	}
};
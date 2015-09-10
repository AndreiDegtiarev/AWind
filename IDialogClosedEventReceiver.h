#pragma once
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
class Window;
///Interface that provides dialog closed notifications (user closes dialog window, by pressing OK or cancel button). If you want receive notification this notification you need derive you dialog class from interface class and implement NotifyDialogClosed member function
class IDialogClosedEventReceiver
{
public:
	enum DialogResults
	{
		OK,
		Cancel
	};
	///Has to be implemented in target class
    /**
	\param window pointer to dialog
	\param results defines how dialog was closed with OK or Cancel
    */
	virtual void NotifyDialogClosed(Window *window,DialogResults results)=0;
};

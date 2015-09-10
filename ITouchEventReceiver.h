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
///Interface that provides screen touch notifications. If you want receive this notification in the target or parent window you need derive you window class from interface class and implement NotifyTouch member function
class ITouchEventReceiver
{
public:
	///Has to be implemented in target class
    /**
	\param window pointer to window (like TextBox) wich content was changed 
    */
	virtual void NotifyTouch(Window *window)=0;
};
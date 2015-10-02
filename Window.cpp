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
#include "Arduino.h"

#include "Window.h"
#include "MainWindow.h"

Dialog *Window::FindDialog(const __FlashStringHelper *id)
{
	return ((MainWindow *)RootWindow())->FindDialog(id);
}
void Window::RegisterDialog(const __FlashStringHelper *id,Dialog *dlg)
{
	((MainWindow *)RootWindow())->RegisterDialog(id,dlg);
}
IDialogClosedEventReceiver::DialogResults Window::DoDialog(Dialog *dlg)
{
	IDialogClosedEventReceiver::DialogResults res=((MainWindow *)RootWindow())->ProcessDoDialog(dlg);
	return res;
}
#ifdef _VARIANT_ARDUINO_DUE_X_   //DUE
char *dtostrf(double val, signed char width, unsigned char prec, char *sout) {
      char fmt[20];
      sprintf(fmt, "%%%d.%df", width, prec);
      sprintf(sout, fmt, val);
      return sout;
   }
#endif


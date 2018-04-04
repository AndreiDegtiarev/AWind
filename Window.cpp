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
#include "Arduino.h"
#include "ADefinitions.h"
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


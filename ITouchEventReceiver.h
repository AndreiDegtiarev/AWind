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
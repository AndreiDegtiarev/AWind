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
///Interface that provides content change notifications (like text is changed in text box) from child window to parent window. If you want receive notification in the parent window you need derive you window class from interface class and implement NotifyContentChanged member function
class IContentChangedEventReceiver
{
public:
	///Has to be implemented in target class
    /**
	\param window pointer to window (like TextBox) wich content was changed 
    */
	virtual void NotifyContentChanged(Window *window)=0;
};
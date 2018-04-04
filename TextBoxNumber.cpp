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
#include "TextBoxNumber.h"
#include "KeyboardWindow.h"

bool TextBoxNumber::OnTouch(int x,int y)
{
	//out<<F("TextBoxNumber::OnTouch")<<endln;
	//AHelper::LogFreeRam();
	bool retCode=TextBox::OnTouch(x,y);
	if(!_isReadOnly)
	{
		KeyboardWindow * kbrWnd=(KeyboardWindow *)FindDialog(F("Keyboard"));
		if(kbrWnd == NULL)
		{
			//out<<F("TextBoxNumber::OnTouch1")<<endln;
			kbrWnd=new KeyboardWindow(3,90);
			RegisterDialog(F("Keyboard"),kbrWnd); 
			//AHelper::LogFreeRam();
			//out<<F("TextBoxNumber::OnTouch2")<<endln;
		}
		if(kbrWnd!=NULL)
		{
			kbrWnd->Initialize(GetNumber(),Precission());
			if(DoDialog(kbrWnd) == IDialogClosedEventReceiver::OK)
				SetNumber(kbrWnd->GetNumber());
		}
	}
	return true;
}

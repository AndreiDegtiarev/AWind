/*
  AWind.h - Arduino window library support for Color TFT LCD Boards
  Copyright (C)2014 Andrei Degtiarev. All right reserved
  
  You can find the latest version of the library at 
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

#include "TextBoxString.h"
#include "TextBoxNumber.h"

class TextExampleWindow : public MainWindow
{
	TextBoxNumber *_textNumber;
public:
	TextExampleWindow(int width,int height):MainWindow(width,height) //(F("TextExampleWindow"),0,0,width,height)
	{
		int x=0;
		int y=40;
		TextBoxFString *label=new TextBoxFString(x,y,width/2,25,F("This is label: "),Color::SkyBlue);
		label->SetFont(BigFont);
		x=width*3.0/4;
		_textNumber=new TextBoxNumber(x,y,width-x,25,0,Color::SkyBlue);
		_textNumber->SetBackColor(Color::Black);
		_textNumber->SetFont(BigFont);
		_textNumber->SetMargins(20,2);
		_textNumber->SetNumber(4);
		_textNumber->SetIsReadOnly(false);

		AddChild(label);
		AddChild(_textNumber);
	}
	void Create()
	{
	}
	void Notify(Window * wnd)
	{
		if(wnd == _textNumber)
		{
			out<<F("Value changed")<<((TextBoxNumber *)_textNumber)->GetNumber()<<endl;
		}
	}
};
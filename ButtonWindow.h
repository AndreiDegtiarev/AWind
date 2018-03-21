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

#include "Window.h"
///Implements pictogram buttons
class ButtonWindow : public Window
{
public:
	enum Pictogram
	{
		TriangleLeft,
		TriangleRight,
		TriangleTop,
		TriangleBottom,
	};
private:
	Pictogram _pictogram;
public:
	///Constructor
	/**
	\param pictogram defines button appearance
	\param left left coordinate relative to parent indow
	\param top top coordinate relative to parent indow
	\param width window width
	\param height window height
	*/
	ButtonWindow(Pictogram pictogram,int left,int top,int width,int height):Window(F("Button"),left,top,width,height)
	{
		_pictogram=pictogram;
		SetDecorators(Environment::Get()->FindDecorators(F("Button")));

	}
	///Implements drawing code
	/**
	\param dc Device context
	*/
	void OnDraw(DC *dc)
	{
		//dc->SetColor(_borderColor);
		int marg=7;
		int x1=0,x2=0,x3=0;
		int y1=0,y2=0,y3=0;
		int x_left=marg;
		int x_mid=Width()/2;
		int x_right=Width()-marg;
		int y_mid=Height()/2;
		int y_top=marg;
		int y_bottom=Height()-marg;
		switch(_pictogram)
		{
		case TriangleLeft:
			x1=x_left;
			y1=y_mid;
			x2=x_right;
			y2=y_top;
			x3=x_right;
			y3=y_bottom;
			break;
		case TriangleRight:
			x1=x_left;
			y1=y_top;
			x2=x_right;
			y2=y_mid;
			x3=x_left;
			y3=y_bottom;
			break;
		case TriangleTop:
			x1=x_mid;
			y1=y_top;
			x2=x_right;
			y2=y_bottom;
			x3=x_left;
			y3=y_bottom;
			break;
		case TriangleBottom:
			x1=x_left;
			y1=y_top;
			x2=x_right;
			y2=y_top;
			x3=x_mid;
			y3=y_bottom;
			break;
		}
		dc->MoveTo(x1,y1);
		dc->LineTo(x2,y2);
		dc->LineTo(x3,y3);
		dc->LineTo(x1,y1);
	}
};

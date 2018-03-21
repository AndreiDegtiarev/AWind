/*
This file is part of AWind library

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
#pragma once
#include <Window.h>
///Example window with yellow background
class Window2 : public Window
{
public:
	Window2(const __FlashStringHelper * name,int left,int top,int width,int height):Window(name,left,top,width,height)
	{
		AddDecorator(new DecoratorRectFill(Color::Yellow,false));
		AddDecorator(new DecoratorColor(Color::Black));
	}

};
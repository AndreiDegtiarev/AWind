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
#pragma once

#include <Touch.h>
#include <URTouch.h>

class TouchUTFT : public Touch
{
	URTouch  *_touch;
public:
	TouchUTFT(URTouch *touch):Touch(), _touch(touch)
	{

	}
	bool dataAvailable()
	{
		return _touch->dataAvailable();
	}
	int16_t getX()
	{
		return _touch->getX();
	};
	int16_t getY()
	{
		return _touch->getY();
	}
	void read()
	{
		_touch->read();
	}
};
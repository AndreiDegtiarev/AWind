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
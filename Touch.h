#pragma once

class Touch
{
public:
	virtual bool dataAvailable() = 0;

	virtual int16_t getX() = 0;

	virtual int16_t getY() = 0;

	virtual void read() = 0;

};
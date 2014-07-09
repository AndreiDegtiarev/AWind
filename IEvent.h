#pragma once

template<class T>class IEvent
{
public:
	virtual void Notify(T *context)=0;
};
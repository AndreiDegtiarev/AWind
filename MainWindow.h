#pragma once
#include "Window.h"
#include "IEvent.h"
#include "KeyboardWindow.h"
class TextBoxNumber;
class MainWindow : public Window, IEvent<Window>
{
	Window *_modalWindow;
	KeyboardWindow _keyboardWindow;
public:
	MainWindow():Window(F("Main"),0,0,0,0),_keyboardWindow(3,90)

	{
		_keyboardWindow.SetVisible(false);
		AddChild(&_keyboardWindow);
		_keyboardWindow.SetEndEditEvent(this);
		_modalWindow=NULL;
	}
	void Notify(Window *window)
	{
		if(window == &_keyboardWindow) //End edit
		{
			SetModalWindow(NULL);
			Invalidate();
		}
	}
	void StartKeyboard(TextBoxNumber *target)
	{
		SetModalWindow(MainWnd()->Keyboard());
		_keyboardWindow.BeginEdit(target);
	}
	Window *ModalWnd()
	{
		return _modalWindow;
	}
	void SetModalWindow(Window * modalWindow)
	{
		_modalWindow=modalWindow;
	}
	void Move(int left,int top,int width,int height)
	{
		Window::Move(left,top,width,height);
	}
	KeyboardWindow * Keyboard()
	{
		return &_keyboardWindow;
	}
};
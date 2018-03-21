#pragma once
#include "DC.h"
#include "UTFT.h"

class DC_UTFT : public DC
{
private:
	UTFT * _lcd;  //!< Pointer to the UTFT class
public:
	///Constructor for global context, that created only once in WindowsManager
	DC_UTFT(UTFT *lcd):DC()
	{
		_lcd = lcd;
	}

	///Returns screen orientation vertical or horisontal
	ScreenOrientationType ScreenOrientation()
	{
		return _lcd->orient == LANDSCAPE ? Landscape : Portrate;
	}
	///Returns screen width
	int DeviceWidth()
	{
		return _lcd->getDisplayXSize() - 1;
	}
	///Returns screen height
	int DeviceHeight()
	{
		return _lcd->getDisplayYSize() - 1;
	}
	///Fills rectangle. Input coordinates have to be defined in the window coordinate system
	void FillRect(int left, int top, int right, int bottom)
	{
		_lcd->fillRect(ToDC_X(left), ToDC_Y(top), ToDC_X(right), ToDC_Y(bottom));
	}
	void setColor(byte r, byte g, byte b)
	{
		_lcd->setColor(r, g, b);
	}
	void drawHLine(int x, int y, int l)
	{
		_lcd->drawHLine(x, y, l);
	}
	///Draws rectangle. Input coordinates have to be defined in the window coordinate system
	void Rectangle(int left, int top, int right, int bottom)
	{
		_lcd->drawRect(ToDC_X(left), ToDC_Y(top), ToDC_X(right), ToDC_Y(bottom));
	}
	///Fills rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void FillRoundRect(int left, int top, int right, int bottom)
	{
		_lcd->fillRoundRect(ToDC_X(left), ToDC_Y(top), ToDC_X(right), ToDC_Y(bottom));
	}
	///Draws rounded rectangle. Input coordinates have to be defined in the window coordinate system
	void DrawRoundRect(int left, int top, int right, int bottom)
	{
		_lcd->drawRoundRect(ToDC_X(left), ToDC_Y(top), ToDC_X(right), ToDC_Y(bottom));
	}
	///Draws circle. Input coordinates have to be defined in the window coordinate system
	void FillCircle(int x0, int y0, int radius)
	{
		_lcd->fillCircle(ToDC_X(x0), ToDC_Y(y0), radius);
	}
	///Draws PROGMEM string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const __FlashStringHelper * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		int stl, i;

		stl = strlen_P((const char PROGMEM *)text);

		if (aligment == HorizontalAlignment::Center)
			x = x + (width - stl * _lcd->cfont.x_size) / 2;
		else if (aligment == HorizontalAlignment::Right)
			x = x + width - stl * _lcd->cfont.x_size;

		x = ToDC_X(x);
		y = ToDC_Y(y);
		if (_lcd->orient == PORTRAIT)
		{
			if (x == RIGHT)
				x = (_lcd->disp_x_size + 1) - (stl*_lcd->cfont.x_size);
			if (x == CENTER)
				x = ((_lcd->disp_x_size + 1) - (stl*_lcd->cfont.x_size)) / 2;
		}
		else
		{
			if (x == RIGHT)
				x = (_lcd->disp_y_size + 1) - (stl*_lcd->cfont.x_size);
			if (x == CENTER)
				x = ((_lcd->disp_y_size + 1) - (stl*_lcd->cfont.x_size)) / 2;
		}
		for (i = 0; i<stl; i++)
		{
			unsigned char c = pgm_read_byte(&((const char PROGMEM *)text)[i]);
			DrawSymbol(c, x + (i*(_lcd->cfont.x_size)), y);
		}

	}
	///Returns symbol width for the current font 
	int FontWidth()
	{
		return _lcd->cfont.x_size;
	}
	///Returns symbol jeight for the current font 
	int FontHeight()
	{
		return _lcd->cfont.y_size;
	}
	///Draws symbol. Input coordinates have to be defined in the screen system
	void DrawSymbol(const char c, int dc_x, int dc_y)
	{
		_lcd->printChar(c, dc_x, dc_y);
	}

	///Draw caret. Input coordinates have to be defined in the window coordinate system
	void DrawCaret(int pos, int x, int y)
	{
		x = x + (pos*_lcd->cfont.x_size);
		DrawChar('_', x, y + 2);
	}
	///Draws string. Input coordinates have to be defined in the window coordinate system
	void DrawText(const char * text, int x, int y, HorizontalAlignment aligment = HorizontalAlignment::Left, int width = 0)
	{
		if (aligment == HorizontalAlignment::Center)
			x = x + (width - strlen(text)*_lcd->cfont.x_size) / 2;
		else if (aligment == HorizontalAlignment::Right)
			x = x + width - strlen(text)*_lcd->cfont.x_size;
		x = ToDC_X(x);
		y = ToDC_Y(y);
		//_lcd->print(text,x,y);
		//out<<text<<" "<<strlen(text)<<endln;
		for (int i = 0; i<strlen(text); i++)
		{
			char c = text[i];
			//if(c==' ')
			//	break;
			DrawSymbol(c, x + (i*(_lcd->cfont.x_size)), y);
		}
	}
	void drawPixel(int x, int y)
	{
		_lcd->drawPixel(x, y);
	}
	void SetDeviceColor(Color color)
	{
		_lcd->setColor(color.GetR(), color.GetG(), color.GetB());
	}
	void SetBackColor(Color color)
	{
		_lcd->setBackColor(VGA_TRANSPARENT);
		SetDeviceColor(color);
	}
	void SetFont(uint8_t *font)
	{
		_lcd->setFont(font);
	}
	void drawLine(int x1, int y1, int x2, int y2)
	{
		_lcd->drawLine(x1, y1, x2, y2);
	}
};
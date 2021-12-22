#include <iostream>

#include <Windows.h>
#include <conio.h>



typedef struct POINT_t
{
	int x, y;
	friend std::ostream& operator <<(std::ostream& cout, POINT_t p);
}Point;

std::ostream& operator <<(std::ostream& cout, POINT_t p)
{
	return cout << "[" << p.x << "," << p.y << "]" << std::endl;
}

typedef struct COLOR_t
{
	int r, g, b;
	bool operator ==(const COLOR_t& o) const
	{
		return r == o.r && g == o.g && b == o.b;
	}
	bool operator !=(const COLOR_t& o) const
	{
		return !(*this == o);
	}
	
	friend std::ostream& operator <<(std::ostream&, COLOR_t);
}Color;

std::ostream& operator <<(std::ostream& cout, COLOR_t c)
{
	return cout << "[" << c.r << "," << c.g << "," << c.b << "]" << std::endl;
}

typedef struct PIXEL_t
{
	Point m_Point = { 0,0 };
	Color m_Color = { 0,0,0 };


	PIXEL_t(Point p) : m_Point(p)
	{
		COLORREF c = GetPixel(GetDC(NULL), p.x, p.y);
		m_Color = { GetRValue(c),GetGValue(c) ,GetBValue(c) };
	}

	PIXEL_t(Color c) : m_Color(c)
	{
		m_Point.x = 0;
		m_Point.y = 0;
	}

	PIXEL_t(int x, int y)
	{
		m_Point.x = x;
		m_Point.y = y;
		const COLORREF c = GetPixel(GetDC(NULL), x, y);
		m_Color.r = GetRValue(c);
		m_Color.g = GetGValue(c);
		m_Color.b = GetBValue(c);
	}

	void UpdateColor()
	{
		const COLORREF c = GetPixel(GetDC(NULL), m_Point.x, m_Point.y);
		m_Color.r = GetRValue(c);
		m_Color.g = GetGValue(c);
		m_Color.b = GetBValue(c);
	}


	friend std::ostream& operator <<(std::ostream& cout, PIXEL_t p);
	
}Pixel;

std::ostream& operator <<(std::ostream& cout, PIXEL_t p)
{
	return cout << p.m_Point << " - " << p.m_Color << std::endl;
}

static void MouseClick(int dir) {
	INPUT Inputs[2] = { 0 };

	switch (dir)
	{
	case 1:
		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dwFlags = MOUSEEVENTF_RIGHTDOWN;
		Inputs[1].type = INPUT_MOUSE;
		Inputs[1].mi.dwFlags = MOUSEEVENTF_RIGHTUP;
		break;
	case -1:
		Inputs[0].type = INPUT_MOUSE;
		Inputs[0].mi.dwFlags = MOUSEEVENTF_LEFTDOWN;
		Inputs[1].type = INPUT_MOUSE;
		Inputs[1].mi.dwFlags = MOUSEEVENTF_LEFTUP;
		break;
	}
	SendInput(1, &Inputs[0], sizeof(INPUT));
	Sleep(100);
	SendInput(1, &Inputs[1], sizeof(INPUT));
}

static void MouseLeftClickAt(const Point& p, int posDelay = 0) {
	SetCursorPos(p.x, p.y);
	MouseClick(-1);
	Sleep(posDelay);
}


constexpr static Color BLACK = { 0,0,0 };

int main()
{

	Pixel* tiles_buffer = (Pixel*)calloc(16, sizeof(Pixel));
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			constexpr int sx = 750;
			constexpr int sy = 350;
			constexpr int delta = 150;
			const int index = i + j * 4;
			if(tiles_buffer != nullptr)
				tiles_buffer[index] = Point({ sx + delta * i, sy + delta * j });
		}
	}

	
	std::cout << "Pressione qualquer tecla para iniciar" << std::endl;
	std::cout << "Pressione SHIFT para pausar" << std::endl;
	int _ = _getch();
	while(1)
	{
		
		for(int i = 0; i < 16; i++)
		{
			tiles_buffer[i].UpdateColor();
			if (tiles_buffer[i].m_Color == BLACK)
				MouseLeftClickAt(tiles_buffer[i].m_Point,2);
		}

		if (GetAsyncKeyState(VK_SHIFT))
			int _ = _getch();
	}

}


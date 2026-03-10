#pragma once
#include "Digit.h";

class Texture;

class Clock
{

	//A second allows to give position and 24 hours mode as well, followed by the hours,
	//	minutes, digit mode(light or dark) and color of the digits.
	//	• The digits for hours would be based on 24 hour mode!
	//	 Both constructors dynamically allocate the Digit instances with the correct data(correct
	//		number value based on hour and minutes, correct color and individual position, …)
	//	 Both also load a texture for “:”, using the given font, size 246.26 and chosen digit color.
	//	• If no color is passed as a parameter, ask one of the Digits for their color(hint :
	//		add accessor in Digit
public:
	Clock(const Vector2f& pos, bool is24);
	Clock(Vector2f pos, bool is24, int hours, int minutes, Digit::Mode mode, const Color4f &color);
	~Clock();

	void Draw() const;
	int GetHour() const;
	int GetMinutes() const;
	void AddMinute();
	void AddHour();

private:
	static const int	m_NumDigits{ 4 };
	Digit*				m_pDigits[m_NumDigits]{};
	Texture*			m_pSeparationTexture;
	Vector2f			m_SeparationPos{};
	Vector2f			m_Pos{};
	bool				m_Is24{};
	bool				m_IsAm{ true };
	Vector2f			m_Dim{};
};


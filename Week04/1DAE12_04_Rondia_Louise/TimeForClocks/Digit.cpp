#include "pch.h"
#include "Digit.h"
#include <iostream>
#include "Texture.h"
#include "utils.h"

Texture* Digit::m_pSpriteSheet{};
int		 Digit::m_NumInstances{};

Digit::Digit(int num, Mode mode, const Vector2f& pos) :
	Digit{ num, mode, pos, Color4f{0.f, 1.f, 0.f, 1.f} }
{
}

Digit::Digit(int num, Mode mode, const Vector2f& pos, const Color4f& color) :
	m_Mode(mode), m_Pos(pos), m_Color(color)
{
	if (num < 0 || num > 9) {
		std::cerr << "Error: wrong argument to Digit constructor: " << num << std::endl;
		return;
	}
	m_Num = num;

	if (!m_NumInstances) {
		m_pSpriteSheet = new Texture{ "digits.png" };
	}
	m_NumInstances++;
}

Digit::~Digit() {
	m_NumInstances--;
	if (!m_NumInstances) {
		delete m_pSpriteSheet;
	}
}

void Digit::Increase() {
	SetValue(m_Num + 1);
	if (m_Num > 9)
		SetValue(0);
}

void Digit::Decrease() {
	SetValue(m_Num - 1);
	if (m_Num < 0)
		SetValue(9);
}

void Digit::Reset() {
	SetValue(0);
}

void Digit::SetValue(int num) {
	m_Num = num;
}

void Digit::Draw() const {
	const Rectf rect{ GetFrameRect(m_Num) };
	utils::SetColor(m_Color);
	utils::FillRect(Rectf{m_Pos.x, m_Pos.y, rect.width, rect.height });
	m_pSpriteSheet->Draw(m_Pos, rect);
}

void Digit::DrawMeridium(const Vector2f& pos, bool isAM) const {
	Rectf rect{ GetFrameRect(10) };

	rect.height /= 2.f;
	if (isAM) {
		rect.bottom += rect.height;
	}

	utils::SetColor(m_Color);
	utils::FillRect(Rectf{ rect.width + m_Pos.x, m_Pos.y, rect.width, rect.height });
	m_pSpriteSheet->Draw(m_Pos + pos, rect);
}

Rectf Digit::GetFrameRect(int num) const {
	const float width{ m_pSpriteSheet->GetWidth() / 11 },
		height{ m_pSpriteSheet->GetHeight() / 2 };
	return Rectf{ width * num, height * (m_Mode == Mode::dark ? 0 : 1), width, height };
}

Color4f Digit::GetColor() const {
	return m_Color;
}

Color4f Digit::GetBgColor() const {
	if (m_Mode == Mode::bright)
		return Color4f{ 1.f, 1.f, 1.f, 1.f };
	return Color4f{ 0.f, 0.f, 0.f, 1.f };
}

float Digit::GetWidth() const {
	return m_pSpriteSheet->GetWidth() / 11;
}

float Digit::GetHeight() const {
	return m_pSpriteSheet->GetHeight() / 2;
}

int Digit::GetValue() const {
	return m_Num;
}
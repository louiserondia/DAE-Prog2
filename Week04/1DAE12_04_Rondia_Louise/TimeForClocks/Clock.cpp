#include "pch.h"
#include "Clock.h"
#include "Texture.h"
#include "utils.h"

Texture* m_pSeparationTexture;

Clock::Clock(const Vector2f& pos, bool is24) :
	Clock(pos, is24, 0, 0, Digit::Mode::dark, m_pDigits[0]->GetColor())
{
}

Clock::Clock(Vector2f pos, bool is24, int hours, int minutes, Digit::Mode mode, const Color4f& color) :
	m_Pos(pos), m_Is24(is24)
{
	m_pSeparationTexture = new Texture{ ":", "Digit.otf", 246, color };
	if (!is24) {
		if (hours > 12) {
			hours -= 12;
			m_IsAm = false;
		}
	}

	m_pDigits[0] = new Digit{ hours / 10, mode, pos, color };
	const Vector2f gap{ m_pDigits[0]->GetWidth(), 0.f };
	Vector2f offset{ gap };

	m_pDigits[1] = new Digit{ hours % 10, mode, pos + offset, color };
	m_SeparationPos = offset + gap;
	offset += gap + Vector2f{ m_pSeparationTexture->GetWidth(), 0.f };
	m_pDigits[2] = new Digit{ minutes / 10, mode, pos + offset, color };
	offset += gap;
	m_pDigits[3] = new Digit{ minutes % 10, mode, pos + offset, color };
	offset += gap;

	if (!m_Is24) {
		offset += gap;
	}

	m_Dim = Vector2f{ offset.x, m_pDigits[0]->GetHeight() };
}

Clock::~Clock() {
	delete m_pSeparationTexture;

	for (int idx{}; idx < m_NumDigits; ++idx) {
		delete m_pDigits[idx];
	}
}

void Clock::Draw() const {
	const Vector2f offset{ m_pDigits[0]->GetWidth(), 0.f };

	utils::SetColor(m_pDigits[0]->GetBgColor());
	utils::FillRect(Rectf{ m_Pos.x, m_Pos.y, m_Dim.x, m_Dim.y });

	for (int idx{}; idx < m_NumDigits; ++idx) {
		m_pDigits[idx]->Draw();
	}

	m_pSeparationTexture->Draw(m_Pos + m_SeparationPos);
	if (!m_Is24) {
		m_pDigits[m_NumDigits - 1]->DrawMeridium(offset, m_IsAm);
	}
}

int Clock::GetHour() const {
	int hour{ m_pDigits[0]->GetValue() * 10 + m_pDigits[1]->GetValue() };
	if (m_Is24 && m_IsAm) {
		hour += 12;
	}
	return hour;
}

int Clock::GetMinutes() const {
	return m_pDigits[2]->GetValue() * 10 + m_pDigits[3]->GetValue();
}

void Clock::AddHour() {
	m_pDigits[1]->Increase();
	if (!m_Is24 && (m_pDigits[1]->GetValue() == 2 && m_pDigits[0]->GetValue() == 1) ||
		m_Is24 && (m_pDigits[1]->GetValue() == 4 && m_pDigits[0]->GetValue() == 2)) {
		m_pDigits[1]->Reset();
		m_pDigits[0]->Reset();
		m_IsAm = !m_IsAm;
	}
	else if (m_pDigits[1]->GetValue() == 0) {
		m_pDigits[1]->Reset();
		m_pDigits[0]->Increase();
	}
}

void Clock::AddMinute() {
	m_pDigits[3]->Increase();
	if (!m_pDigits[3]->GetValue()) {
		m_pDigits[2]->Increase();
		if (m_pDigits[2]->GetValue() == 6) {
			m_pDigits[2]->Reset();
			AddHour();
		}
	}
}
#include "pch.h"
#include "RoundButton.h"
#include "utils.h"


RoundButton::RoundButton(float x, float y, const std::string& path) :
	m_Pos(Vector2f{ x, y }),
	m_Texture(new Texture{ path })
{
}

RoundButton::RoundButton(const Vector2f& pos, const std::string& path) :
	m_Pos(pos),
	m_Texture(new Texture{ path })
{
}

RoundButton::~RoundButton() {
	delete m_Texture;
}

bool RoundButton::IsPressed() const {
	return m_IsDown;
}

bool RoundButton::CheckHit(float x, float y) {
	return CheckHit(Vector2f{ x, y });
}

bool RoundButton::CheckHit(const Vector2f& pos) {
	const Rectf bounds{ GetBounds() };
	const Circlef circle{ m_Pos.x + bounds.width / 2, m_Pos.y + bounds.height / 2, bounds.height / 2 };

	return utils::IsPointInCircle(pos, circle);
}

void RoundButton::Draw() const {
	m_Texture->Draw(m_Pos, GetBounds());
	m_Texture->Draw(m_Pos, Rectf{ m_Texture->GetWidth() / 3 * 2, 0.f,  m_Texture->GetWidth() / 3,  m_Texture->GetHeight() });
}

Rectf RoundButton::GetBounds() const {
	const int offset{ m_IsDown ? 1 : 0 };

	return Rectf{ (m_Texture->GetWidth() / 3) * offset, 0.f, m_Texture->GetWidth() / 3, m_Texture->GetHeight() };
}

void RoundButton::Press() {
	m_IsDown = true;
}

void RoundButton::Release() {
	m_IsDown = false;
}

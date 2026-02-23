#include "pch.h"
#include "NorseWarrior.h"
#include "utils.h"

Rectf NorseWarrior::GetBounds() const {
	return Rectf{ m_Pos.x, m_Pos.y, m_pWarriorTexture->GetWidth(), m_pWarriorTexture->GetHeight() };
}

Rectf NorseWarrior::GetHitbox() const {
	if (m_HealthPercent <= 0) return Rectf{};
	return Rectf{ m_Pos.x + m_pWarriorTexture->GetWidth() / 4, m_Pos.y + m_pWarriorTexture->GetHeight() / 4, m_pWarriorTexture->GetWidth() / 2, m_pWarriorTexture->GetHeight() / 2 };
}

void NorseWarrior::DrawLabel() const {
	if (m_HealthPercent <= 0)
		return;

	m_pLabelTexture->Draw(
		m_Pos + Vector2f{
			m_pWarriorTexture->GetWidth() / 2 - m_pLabelTexture->GetWidth() / 2,
			m_pWarriorTexture->GetHeight() 
	});
}
void NorseWarrior::DrawWarrior() const {
	if (m_HealthPercent <= 0)
		return;

	Rectf healthBar{
		m_Pos.x + 20.f,
		m_Pos.y + m_pWarriorTexture->GetHeight(),
		m_pWarriorTexture->GetWidth() - 40.f,
		5.f
	};
	m_pWarriorTexture->Draw(m_Pos);
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f });
	utils::FillRect(healthBar);
	healthBar.width = healthBar.width * (m_HealthPercent / 100.f);
	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	utils::FillRect(healthBar);
}

void NorseWarrior::Select() {
	m_IsSelected = true;
}

void NorseWarrior::Unselect() {
	m_IsSelected = false;
}

void NorseWarrior::MoveWith(float dX, float dY, const Rectf& winBounds) {
	if (m_Pos.x + dX >= winBounds.left && m_Pos.x + m_pWarriorTexture->GetWidth() + dX <= winBounds.width)
		m_Pos.x += dX;
	if (m_Pos.y + dY >= winBounds.bottom && m_Pos.y + m_pWarriorTexture->GetHeight() + dY <= winBounds.height)
		m_Pos.y += dY;
}

void NorseWarrior::MoveWith(float dX, float dY, const Vector2f& winDimensions) {
	MoveWith(dX, dY, Rectf(0.f, 0.f, winDimensions.x, winDimensions.y));
}

void NorseWarrior::MoveWith(const Vector2f& delta, const Vector2f& winDimensions) {
	MoveWith(delta.x, delta.y, Rectf(0.f, 0.f, winDimensions.x, winDimensions.y));
}

void NorseWarrior::MoveWith(const Vector2f& delta, const Rectf& winBounds) {
	MoveWith(delta.x, delta.y, winBounds);
}

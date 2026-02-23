#include "pch.h"
#include "PersonalBubble.h"
#include "utils.h"

// public

void PersonalBubble::SetPosition(const Vector2f& pos) {
	SetPosition(pos.x, pos.y);
}

void PersonalBubble::SetPosition(float x, float y) {
	m_SocialBubble.center.x = x;
	m_SocialBubble.center.y = y;
}

void PersonalBubble::RandomizeVelocity() {
	m_Velocity = Vector2f{
		static_cast<float>(rand() % 61 + 20),
		static_cast<float>(rand() % 61 + 20)
	};
}

void PersonalBubble::RandomizePosition(const Rectf& viewPort) {
	m_SocialBubble.center = Vector2f{
		rand() % static_cast<int>(viewPort.width - m_SocialBubble.radius * 2) + m_SocialBubble.radius,
		rand() % static_cast<int>(viewPort.height - m_SocialBubble.radius * 2) + m_SocialBubble.radius
	};
}

void PersonalBubble::Update(const Rectf& viewPort, float elapsedSec) {
	m_SocialBubble.center.x += m_Velocity.x * elapsedSec;
	m_SocialBubble.center.y += m_Velocity.y * elapsedSec;

	if (m_SocialBubble.center.x - m_SocialBubble.radius < 0.f) {
		m_Velocity.x *= -1;
		m_SocialBubble.center.x = m_SocialBubble.radius;
	}

	else if (m_SocialBubble.center.x + m_SocialBubble.radius > viewPort.width) {
		m_Velocity.x *= -1;
		m_SocialBubble.center.x = viewPort.width - m_SocialBubble.radius;
	}
	if (m_SocialBubble.center.y - m_SocialBubble.radius < 0.f) {
		m_Velocity.y *= -1;
		m_SocialBubble.center.y = m_SocialBubble.radius;
	}
	else if (m_SocialBubble.center.y + m_SocialBubble.radius > viewPort.height) {
		m_Velocity.y *= -1;
		m_SocialBubble.center.y = viewPort.height - m_SocialBubble.radius;
	}
	GrowConfidence(elapsedSec);
}

void PersonalBubble::InteractWith(const PersonalBubble* other) {
	if (utils::IsOverlapping(other->m_SocialBubble, m_SocialBubble)) {
		m_BodyRadius /= 2;
		if (m_BodyRadius < m_MinBodyRadius)
			m_BodyRadius = m_MinBodyRadius;
		m_Velocity.x *= (IsSameDirection(m_Velocity.x, other->m_Velocity.x) ? -1 : 1);
		m_Velocity.y *= (IsSameDirection(m_Velocity.y, other->m_Velocity.y) ? -1 : 1);
		m_SocialBubble.center.x += m_Velocity.x * 0.1f;
		m_SocialBubble.center.y += m_Velocity.y * 0.1f;
	}
}

void PersonalBubble::Draw() const {
	DrawSocialBubble();
	DrawBody();
}

// private

void PersonalBubble::DrawSocialBubble() const {
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, 1.f });
	utils::DrawEllipse(m_SocialBubble.center, m_SocialBubble.radius, m_SocialBubble.radius, 3.f);
	utils::DrawEllipse(m_SocialBubble.center, 2.f, 2.f, 3.f);
	utils::SetColor(Color4f{ 1.f, 1.f, 1.f, .5f });
	utils::DrawEllipse(m_SocialBubble.center, m_SocialBubble.radius, m_SocialBubble.radius);
}

void PersonalBubble::DrawBody() const {
	utils::SetColor(Color4f{ 0.f, 1.f, 0.f, 1.f });
	utils::FillEllipse(m_SocialBubble.center, m_BodyRadius, m_BodyRadius);
}

void PersonalBubble::GrowConfidence(float elapsedSec) {
	if (m_BodyRadius < m_SocialBubble.radius / 1.5f)
		m_BodyRadius += 8 * elapsedSec;
}

bool PersonalBubble::IsSameDirection(float dir1, float dir2) const {
	return (dir1 < 0 && dir2 < 0) || (dir1 > 0 && dir2 > 0);
}
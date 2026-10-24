#include "pch.h"
#include "Camera.h"

const Vector2f Camera::VIEWPORT = Vector2f{ 1080.f, 720.f };
Vector2f Camera::m_Pos{};
float Camera::m_Scale{ 1.f };

Vector2f Camera::GetPos() {
	return m_Pos;
}

void Camera::SetPos(const Vector2f& pos) {
	m_Pos = pos * m_Scale;
}

float Camera::GetScale() {
	return m_Scale;
}

void Camera::Add(float x, float y) {
	m_Pos.x += x;
	m_Pos.y += y;
}

void Camera::ZoomIn() const
{
	m_Scale = m_Scale >= m_MaxZoom ? m_Scale : m_Scale + m_ScaleChange;
}

void Camera::ZoomOut() const
{
	
	m_Scale = m_Scale <= m_MinZoom ? m_Scale : m_Scale - m_ScaleChange;
}

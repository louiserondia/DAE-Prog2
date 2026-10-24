#include "pch.h"
#include "Renderable.h"
#include <iostream>
#include "Map.h"

Renderable::Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, int dist, int priority, const std::string& str) :
	m_Pos(pos),
	m_pTexture(texturePair.first),
	m_TextureBounds(texturePair.second),
	m_Dist(dist),
	m_Priority(priority),
	m_Name(str)
{
}

void Renderable::Draw() const {
	const Rectf dst{ m_Pos.x , m_Pos.y, m_Dimensions.x, m_Dimensions.y };
	m_pTexture->Draw(dst, m_TextureBounds);
}

void Renderable::DrawAt(const Vector2f &pos) const {
	const Rectf dst{ pos.x, pos.y, m_Dimensions.x, m_Dimensions.y };
	m_pTexture->Draw(dst, m_TextureBounds);
}

Vector2f Renderable::GetPos() const {
	return m_Pos;
}

int Renderable::GetDist() const {
	return m_Dist;
}

int	Renderable::GetPriority() const {
	return m_Priority;
}

Vector2f Renderable::GetDimensions() const {
	return m_Dimensions;
}

bool Renderable::operator<(const Renderable& rhs) {
	const int width{ Map::GetWidth() };

	const int lhsPrio{ GetPriority() },
			rhsPrio{ rhs.GetPriority() };

	//floors will always be drawn before
	if (!lhsPrio) return false;
	else if (!rhsPrio) return true;

	// bigger priority goes to the front if equality
	if (m_Dist == rhs.GetDist())
		return lhsPrio > rhsPrio;
	return m_Dist < rhs.GetDist();
}

const std::string& Renderable::GetName() const
{
	return m_Name;
}

void Renderable::SetDimensions(float x, float y)
{
	m_Dimensions = Vector2f{ x, y };
}

void Renderable::SetDimensions(const Vector2f &dimensions)
{
	m_Dimensions = dimensions;
}

void Renderable::SetDist(int dist)
{
	m_Dist = dist;
}

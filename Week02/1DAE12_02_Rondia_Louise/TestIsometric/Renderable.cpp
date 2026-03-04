#include "pch.h"
#include "Renderable.h"
#include <iostream>
#include "Map.h"

Renderable::Renderable(const Vector2f& pos, Texture* texture, const Rectf& textureBounds, const Vector2i& coord, int priority) :
	m_Pos(pos),
	m_Texture(texture),
	m_TextureBounds(textureBounds),
	m_Coord(coord),
	m_Priority(priority)
{
}

Renderable::Renderable(float x, float y, Texture* texture, const Rectf& textureBounds, const Vector2i& coord, int priority) :
	m_Pos(Vector2f{ x, y }),
	m_Texture(texture),
	m_TextureBounds(textureBounds),
	m_Coord(coord),
	m_Priority(priority)
{
}

Renderable::Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, int priority) :
	m_Pos(pos),
	m_Texture(texturePair.first),
	m_TextureBounds(texturePair.second),
	m_Coord(coord),
	m_Priority(priority)
{
}

Renderable::Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, int priority, const std::string& str) :
	m_Pos(pos),
	m_Texture(texturePair.first),
	m_TextureBounds(texturePair.second),
	m_Coord(coord),
	m_Priority(priority),
	name(str)
{
}

Renderable::Renderable(float x, float y, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, int priority) :
	m_Pos(Vector2f{ x, y }),
	m_Texture(texturePair.first),
	m_TextureBounds(texturePair.second),
	m_Coord(coord),
	m_Priority(priority)
{
}


int Renderable::GetDistToBottomRight(int width) const {
	//std::cout << m_Coord.x << " coord y " << m_Coord.y << std::endl;
	return (width - m_Coord.x) * (width - m_Coord.x) + m_Coord.y * m_Coord.y; // why check sqrt en vrai
	//return sqrtf(static_cast<float>((width - m_Coord.x) * (width - m_Coord.x) + m_Coord.y * m_Coord.y));
}

void Renderable::Draw() const {
	m_Texture->Draw(Vector2f{ m_Pos.x, m_Pos.y }, m_TextureBounds);
}

Vector2f Renderable::GetPos() const {
	return m_Pos;
}

Vector2i Renderable::GetCoord() const {
	return m_Coord;
}

int	Renderable::GetPriority() const {
	return m_Priority;
}

bool Renderable::operator<(const Renderable& rhs) {

	const int width{ Map::GetWidth() };
	int lhsDist{ GetDistToBottomRight(width) },
		rhsDist{ rhs.GetDistToBottomRight(width) };
	int lhsPrio{ GetPriority() },
		rhsPrio{ rhs.GetPriority() };

	//floors will always be drawn before
	if (!lhsPrio) return false;
	else if (!rhsPrio) return true;

	// bigger priority goes to the front if equality
	if (lhsDist == rhsDist)	return lhsPrio > rhsPrio;
	return lhsDist < rhsDist;
}
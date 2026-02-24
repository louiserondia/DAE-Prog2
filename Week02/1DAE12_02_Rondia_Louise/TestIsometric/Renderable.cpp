#include "pch.h"
#include "Renderable.h"

Renderable::Renderable(const Vector2f& pos, Texture* texture, const Rectf& textureBounds, const Vector2i& coord) : 
	m_Pos(pos), 
	m_Texture(texture), 
	m_TextureBounds(textureBounds), 
	m_Coord(coord)
{}
Renderable::Renderable(float x, float y, Texture* texture, const Rectf& textureBounds, const Vector2i& coord) :
	m_Pos(Vector2f{x, y}), 
	m_Texture(texture), 
	m_TextureBounds(textureBounds), 
	m_Coord(coord)
{}

Renderable::Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord) : 
	m_Pos(pos), 
	m_Texture(texturePair.first), 
	m_TextureBounds(texturePair.second), 
	m_Coord(coord)
{}

Renderable::Renderable(float x, float y, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord) : 
	m_Pos(Vector2f{x, y}), 
	m_Texture(texturePair.first),
	m_TextureBounds(texturePair.second),
	m_Coord(coord)
{}


float Renderable::GetDistToMaxWidth(int width) const {
	return sqrtf(static_cast<float>((width - m_Coord.x) * (width - m_Coord.x) + m_Coord.y * m_Coord.y));
}

bool Renderable::operator<(const Renderable& rhs) const {
	return GetDistToMaxWidth(4) < rhs.GetDistToMaxWidth(4);
}

void Renderable::Draw(const Rectf& bounds) const {
	m_Texture->Draw(Vector2f{ m_Pos.x, m_Pos.y + bounds.height / 2 }, m_TextureBounds);
}

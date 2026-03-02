#include "pch.h"
#include "Renderable.h"
#include <iostream>

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

Renderable::Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, const std::string& str) : 
	m_Pos(pos), 
	m_Texture(texturePair.first), 
	m_TextureBounds(texturePair.second), 
	m_Coord(coord),
	name(str)
{}

Renderable::Renderable(float x, float y, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord) : 
	m_Pos(Vector2f{x, y}), 
	m_Texture(texturePair.first),
	m_TextureBounds(texturePair.second),
	m_Coord(coord)
{}


float Renderable::GetDistToBottomRight(int width) const {
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
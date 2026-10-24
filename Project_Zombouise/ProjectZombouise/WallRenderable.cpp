#include "pch.h"
#include "WallRenderable.h"
#include "Tile.h"
#include <iostream>

WallRenderable::WallRenderable(
	const Vector2f& pos,
	std::pair<Texture*, Rectf> texturePair, 
	int dist,
	int priority,
	const std::string& str
) : Renderable(pos, texturePair, dist, priority, str)
{}

void WallRenderable::Draw() const {
	Vector2f pos{ GetPos() };
	pos.y -= Tile::HEIGHT / 2;
	m_pTexture->Draw(pos, Rectf{ m_TextureBounds.left, m_TextureBounds.height, m_TextureBounds.width, m_TextureBounds.height });
	pos.y += GetDimensions().y / 2;
	m_pTexture->Draw(pos, Rectf{ m_TextureBounds.left, m_TextureBounds.height, m_TextureBounds.width, m_TextureBounds.height });
	pos.y += GetDimensions().y / 2;
	m_pTexture->Draw(pos, m_TextureBounds);
}
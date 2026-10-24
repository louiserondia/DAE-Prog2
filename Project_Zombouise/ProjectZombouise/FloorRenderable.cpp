#include "pch.h"
#include "FloorRenderable.h"
#include <iostream>
#include "Tile.h"

FloorRenderable::FloorRenderable(
	const Vector2f& pos,
	std::pair<Texture*, Rectf> texturePair,
	int dist,
	int priority,
	const std::string& str
) : Renderable(pos, texturePair, dist, priority, str) 
{}

void FloorRenderable::Draw() const {
	Vector2f pos{ GetPos() };
	pos.y -= Tile::HEIGHT / 2;
	m_pTexture->Draw(pos, m_TextureBounds);
}
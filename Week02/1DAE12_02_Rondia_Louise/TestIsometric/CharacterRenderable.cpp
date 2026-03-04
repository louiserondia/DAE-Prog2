#include "pch.h"
#include "CharacterRenderable.h"

void CharacterRenderable::SetPos(const Vector2f pos)  {
	m_Pos = pos;
}

void CharacterRenderable::SetCoord(const Vector2i coord)  {
	m_Coord = coord;
}

void CharacterRenderable::Draw() const {
	m_Texture->Draw(Vector2f{ m_Pos.x - m_Texture->GetWidth() / 2, m_Pos.y}, m_TextureBounds);
}
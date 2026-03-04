#pragma once
#include "Renderable.h"

class CharacterRenderable : public Renderable
{
	// Player * m_Player -> ref to the gameplay, see later

public:
	using Renderable::Renderable; // non

	//CharacterRenderable() : Renderable() {
		
	//}

	// using to inherit of the constructor instead of ->
	// CharacterRenderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord):
	// Renderable(pos, texturePair, coord) { }

	void SetPos(const Vector2f pos);
	void SetCoord(const Vector2i coord);
	void Draw() const override;
};


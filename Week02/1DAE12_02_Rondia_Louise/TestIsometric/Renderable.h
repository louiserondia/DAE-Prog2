#pragma once
#include "Vector2i.h"
#include "Texture.h"

class Renderable
{
public:

	Renderable(const Vector2f& pos, Texture* texture, const Rectf& textureBounds, const Vector2i& coord);

	float GetDistToMaxWidth(int width) const;
	void Draw(const Rectf& bounds) const;

private:

	Vector2f m_Pos{};
	Texture* m_Texture{};
	Rectf	 m_TextureBounds{};
	Vector2i m_Coord{};

	// plus besoin
	bool operator<(const Renderable& rhs) const;
};


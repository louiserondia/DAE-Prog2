#pragma once
#include "Vector2i.h"
#include "Texture.h"

class Renderable
{
public:
	Renderable() = default;
	Renderable(const Vector2f& pos, Texture* texture, const Rectf& textureBounds, const Vector2i& coord);
	Renderable(float x, float y, Texture* texture, const Rectf& textureBounds, const Vector2i& coord);
	Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord);
	Renderable(float x, float y, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord);
	
	// with name for debug
	Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, const std::string& str);

	float GetDistToBottomRight(int width) const;
	virtual void Draw() const;
	Vector2f GetPos() const;
	Vector2i GetCoord() const;

	std::string name{}; // for debug
protected:
	Vector2f m_Pos{};
	Vector2i m_Coord{};

private:
	Texture* m_Texture{};
	Rectf	 m_TextureBounds{};
	// add a type for comparaison

};

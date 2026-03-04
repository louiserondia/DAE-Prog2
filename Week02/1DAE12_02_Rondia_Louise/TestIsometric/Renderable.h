#pragma once
#include "Vector2i.h"
#include "Texture.h"

class Renderable
{
public:
	Renderable() = default;
	Renderable(const Vector2f& pos, Texture* texture, const Rectf& textureBounds, const Vector2i& coord, int priority);
	Renderable(float x, float y, Texture* texture, const Rectf& textureBounds, const Vector2i& coord, int priority);
	Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, int priority);
	Renderable(float x, float y, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, int priority);
	
	// with name for debug
	Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, const Vector2i& coord, int priority, const std::string& str);

	virtual void	Draw() const;
	int				GetDistToBottomRight(int width) const;
	Vector2f		GetPos() const;
	Vector2i		GetCoord() const;
	int				GetPriority() const;
	bool			operator<(const Renderable& rhs);

	std::string name{}; // for debug
protected:
	Vector2f m_Pos{};
	Vector2i m_Coord{};
	Texture* m_Texture{};
	Rectf	 m_TextureBounds{};
	int		 m_Priority{};

private:
	// add a type for comparaison

};

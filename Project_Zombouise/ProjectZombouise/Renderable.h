#pragma once
#include "Vector2i.h"
#include "Texture.h"
#include <ole.h>

class Renderable
{
public:
	explicit Renderable() = default;
	explicit Renderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, int dist, int priority, const std::string& str);
	virtual ~Renderable() = default;
	Renderable(const Renderable& other) = default;
	Renderable& operator=(const Renderable& other) = default;
	Renderable(Renderable&& other) noexcept = default;
	Renderable& operator=(Renderable&& other) noexcept = default;
	
	Vector2f		GetPos() const;
	int				GetDist() const;
	int				GetPriority() const;
	Vector2f		GetDimensions() const;

	void			SetDimensions(float x, float y);
	void			SetDimensions(const Vector2f& dimensions);
	void			SetDist(int dist);

	virtual void	Draw() const;
	void			DrawAt(const Vector2f& pos) const;

	bool			operator<(const Renderable& rhs);
	static bool		CompareRenderables(Renderable* a, Renderable* b) { return (*a < *b); }
	const std::string& GetName() const;

protected:

	Vector2f	m_Pos{};
	int			m_Dist{}; // dist from bottom right
	Rectf		m_TextureBounds{};
	Texture*	m_pTexture{};

private:

	std::string m_Name{};
	int			m_Priority{};
	Vector2f	m_Dimensions{};
};

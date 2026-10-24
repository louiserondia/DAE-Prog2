#pragma once
#include "Renderable.h"
#include <map>
#include "WalkingRenderable.h"

class Texture;

class ZombieRenderable final : public WalkingRenderable
{

public:

	explicit ZombieRenderable(
		const Vector2f& pos,
		std::pair<Texture*, Rectf> texturePair,
		int dist,
		int priority,
		const std::string& str);

	virtual void	Draw() const override;
	Rectf			GetDstBounds() const;
	void			SetArmsUp(bool hasArmsUp);
	void			SetCanBeDisplayed(bool canBeDisplayed);
	virtual void	UpdateFrameBounds() override;
	void			Fall();
	void			GetUp();
	void			LieDown();

private:

	virtual void InitAnimFrames() override;
	
	bool		m_HasArmsUp{};
	bool		m_CanBeDisplayed{ true };
	Texture*	m_pBloodTex{};
};


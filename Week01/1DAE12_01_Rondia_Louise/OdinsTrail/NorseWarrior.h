#pragma once
#include <vector>
#include "Texture.h"

struct NorseWarrior
{
	std::string m_Name{};
	std::string m_BattleCry{};
	Vector2f	m_Pos{};
	Texture*	m_pWarriorTexture{};
	Texture*	m_pLabelTexture{};
	float		m_HealthPercent{ 100 };
	bool		m_IsSelected{};	

	void	DrawWarrior() const;
	void	DrawLabel() const;
	Rectf	GetBounds() const;
	Rectf	GetHitbox() const;
	void	Select();
	void	Unselect();
	void	MoveWith(float dX, float dY, const Rectf& winBounds);
	void	MoveWith(float dX, float dY, const Vector2f& winDimensions);
	void	MoveWith(const Vector2f& delta, const Rectf& winBounds);
	void	MoveWith(const Vector2f& delta, const Vector2f& winDimensions);

};


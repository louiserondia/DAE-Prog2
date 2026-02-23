#pragma once
#include "Texture.h"

struct Arrow
{
	enum class State
	{
		flying,
		stuck,
		destroyed
	};

	State	 state{};
	Vector2f pos{};
	Vector2f velocity{};
	Texture* texture{};

	void Draw() const;
	void Update(float elapsedSec);
};


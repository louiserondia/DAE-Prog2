#pragma once
#include "Renderable.h"

class WallRenderable final: public Renderable
{
public:

	explicit WallRenderable(
		const Vector2f& pos,
		std::pair<Texture*, Rectf> texturePair,
		int dist,
		int priority,
		const std::string& str
	);

	void Draw() const override;

private:

};


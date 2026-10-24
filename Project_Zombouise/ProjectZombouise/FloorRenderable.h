#pragma once
#include "Renderable.h"

class FloorRenderable final : public Renderable
{
public:
	explicit FloorRenderable(
		const Vector2f& pos,
		std::pair<Texture*, Rectf> texturePair,
		int dist,
		int priority,
		const std::string& str
	);

	void Draw() const override;

private:

};


#include "pch.h"
#include "Arrow.h"
#include <iostream>
#include "utils.h"

#define M_PI 3.1415

void Arrow::Draw() const {
	const float angle{ atan2f(velocity.y, velocity.x) };

	glPushMatrix();
	{
		glTranslatef(pos.x, pos.y, 0.f);
		glRotatef(angle * 180 / M_PI, 0.f, 0.f, 1.f);
		texture->Draw();
	}
	glPopMatrix();
	utils::SetColor(Color4f{ 1.f, 0.f, 0, 1.f });
	utils::DrawRect(pos.x - 3, pos.y - 3, 6, 6, 5.f);
}

void Arrow::Update(float elapsedSec) {
	switch (state)
	{
	case Arrow::State::flying:
		pos.x += velocity.x * elapsedSec;
		pos.y += velocity.y * elapsedSec;
		velocity.y += elapsedSec * 300;
		break;
	case Arrow::State::stuck:
		break;
	case Arrow::State::destroyed:
		break;
	default:
		break;
	}
}

// how can i set the pos to the center properly ? 
#pragma once
#include <iostream>

class PersonalBubble
{
public:

	void SetPosition(const Vector2f& pos);
	void SetPosition(float x, float y);
	void RandomizeVelocity();
	void RandomizePosition(const Rectf& viewPort);
	void Update(const Rectf& viewPort, float elapsedSec);
	void InteractWith(const PersonalBubble* other);
	void Draw() const;


private:

	void DrawSocialBubble() const;
	void DrawBody() const;
	void GrowConfidence(float elapsedSec);
	bool IsSameDirection(float dir1, float dir2) const;

	Circlef		m_SocialBubble{ 0.f, 0.f, 60.f };
	float		m_BodyRadius{ 10.f };
	const float m_MinBodyRadius{ 8.f };
	Vector2f	m_Velocity{ 10.f, 20.f };

};


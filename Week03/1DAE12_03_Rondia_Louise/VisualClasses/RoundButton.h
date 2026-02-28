#pragma once
#include "Texture.h"

class RoundButton
{

public:
	RoundButton(float x, float y, const std::string &path);
	RoundButton(const Vector2f& pos, const std::string& path);
	~RoundButton();

	bool IsPressed() const;
	bool CheckHit(float x, float y);
	bool CheckHit(const Vector2f& pos);
	void Draw() const;
	void Press();
	void Release();

private:
	Rectf GetBounds() const;

	bool m_IsDown{};
	Vector2f m_Pos{};
	Texture* m_Texture{};

};


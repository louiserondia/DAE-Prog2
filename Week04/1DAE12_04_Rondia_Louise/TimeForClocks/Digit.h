#pragma once
class Texture;

class Digit
{
public:

	enum class Mode {
		dark,
		bright
	};

	Digit(int number, Mode mode, const Vector2f& pos);
	Digit(int number, Mode mode, const Vector2f& pos, const Color4f& color);
	~Digit();
	
	void Increase();
	void Decrease();
	void Reset();
	void SetValue(int num);
	void Draw() const;
	void DrawMeridium(const Vector2f& pos, bool isAM) const;
	Color4f GetColor() const;
	Color4f GetBgColor() const;
	float GetWidth() const;
	float GetHeight() const;
	int	 GetValue() const;


private:

	Rectf GetFrameRect(int num) const;

	int			m_Num{};
	Vector2f	m_Pos{};
	Color4f		m_Color{ 0.f, 1.f, 0.f, 1.f };
	Mode		m_Mode{};
	static Texture*	m_pSpriteSheet;
	static int		m_NumInstances;
};

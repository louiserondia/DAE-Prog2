#pragma once
class Camera final
{
public:

	static Vector2f	GetPos();
	void			SetPos(const Vector2f& pos);
	static float	GetScale();
	void			Add(float x, float y);
	void			ZoomIn() const;
	void			ZoomOut() const;

	static const Vector2f VIEWPORT;

private:
	static Vector2f	m_Pos;
	static float	m_Scale;
	const float		m_ScaleChange{ 0.1f };
	const float		m_MinZoom{ 0.4f };
	const float		m_MaxZoom{ 1.8f };
};


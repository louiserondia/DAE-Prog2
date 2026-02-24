#pragma once
#include "Texture.h"
#include "Vector2i.h"

class TextureManager
{
public:
	enum class Type {
		floor,
		wall
	};

	static TextureManager* GetInstance();
	~TextureManager();

	std::pair<Texture *, Rectf> GetTexture(Type type, int index);

private:

	TextureManager();

	static TextureManager* instance;

	Texture* m_FloorsTexture{};
	Texture* m_WallsTexture{};

	Vector2i m_FloorsDim{ 3, 6 };
	Vector2i m_WallsDim{ 6, 4 };
};
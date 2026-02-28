#pragma once
#include "Texture.h"
#include "Vector2i.h"

#define NO_TEXTURE 0

#define WALL_W_BRICK 1
#define WALL_N_BRICK 2
#define WALL_W_TILE 3
#define WALL_N_TILE 4

#define FLOOR_WOOD_0 1
#define FLOOR_WOOD_1 2


class TextureManager
{
public:

	// i could make a macro for each type of wall like BRICK_NORTH 0 BRICK_WEST 1

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
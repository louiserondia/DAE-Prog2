#pragma once
#include <iostream>
#include <vector>
#include "Texture.h"
#include "Vector2i.h"
#include <Vector2i.h>
#include "TextureManager.h"
#include "Floor.h"
#include "Wall.h"

class Renderable;
class WallRenderable;

class Tile final
{
public:

	Vector2i GetCoord() const;
	const Wall& GetNorthWall() const;
	const Wall& GetWestWall() const;
	bool HasNorthWall() const;
	bool HasWestWall() const;

	void SetCoord(int x, int y);
	void SetCoord(const Vector2i& c);

	int GetDistToBottomRight(int width) const;

	void CreateWall(bool isNorth, int token, WallRenderable* renderable);
	void CreateFloor(int token, FloorRenderable* renderable);

	static const float WIDTH;
	static const float HEIGHT;

private:

	Vector2i	m_Coord{};
	Floor		m_Floor{};
	Wall		m_WestWall{};
	Wall		m_NorthWall{};

};


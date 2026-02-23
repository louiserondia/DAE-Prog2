#pragma once
#include <iostream>
#include <vector>
#include "Texture.h"
#include "Vector2i.h"
#include <Vector2i.h>

class Tile
{
public:
	void SetTile(const Rectf& floorSrc, bool isWestWall, bool isNorthWall);
	void SetCoord(int x, int y);
	void SetCoord(const Vector2i& c);
	void SetIsWestWall(bool isWestWall, const Rectf& westWallSrc);
	void SetIsNorthWall(bool isNorthWall, const Rectf& NorthWallSrc);
	
	// temporary
private:	

	int	m_Type{}; // 0 is grass, 1 is wood
	bool m_IsWestWall{};
	bool m_IsNorthWall{};
	Vector2i m_Coord{};
	

};

// maybe tile needs to know it's type for sounds etc but not the texture
// also needs to know if it has a wall but not the rectf


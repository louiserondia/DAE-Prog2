#pragma once
#include <iostream>
#include <vector>
#include "Texture.h"
#include "Vector2i.h"
#include <Vector2i.h>
#include "TextureManager.h"

class Tile
{
public:
	void SetCoord(int x, int y);
	void SetCoord(const Vector2i& c);
	void SetWestWallType(int token);
	void SetNorthWallType(int token);
	
private:	

	int	 m_FloorType{};
	bool m_WestWallType{};
	bool m_NorthWallType{};
	Vector2i m_Coord{};
	
	// pointer to renderable 

};

// tile needs to know its type for sounds etc but not the texture
// also needs to know if it has a wall but not the rectf


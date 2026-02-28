#include "pch.h"
#include "Tile.h"

void Tile::SetCoord(int x, int y) {
	m_Coord = Vector2i{ x, y };
}

void Tile::SetCoord(const Vector2i& coord) {
	m_Coord = coord;
}

void Tile::SetWestWallType(int token) {
	m_WestWallType = token;
}

void Tile::SetNorthWallType(int token) {
	m_NorthWallType = token;
}

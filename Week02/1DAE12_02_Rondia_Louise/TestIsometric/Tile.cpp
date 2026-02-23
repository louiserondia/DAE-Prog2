#include "pch.h"
#include "Tile.h"

void Tile::SetTile(const Rectf& floorSrc, bool isWestWall, bool isNorthWall) {
	m_IsWestWall = isWestWall;
	m_IsNorthWall = isNorthWall;
}

void Tile::SetCoord(int x, int y) {
	m_Coord = Vector2i{x, y};
}

void Tile::SetCoord(const Vector2i& coord) {
	m_Coord = coord;
}

void Tile::SetIsWestWall(bool isWestWall, const Rectf& westWallSrc) {
	m_IsWestWall = isWestWall;
}

void Tile::SetIsNorthWall(bool isNorthWall, const Rectf& northWallSrc) {
	m_IsNorthWall = isNorthWall;
}

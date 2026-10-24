#include "pch.h"
#include "Tile.h"

const float Tile::WIDTH{ 128.f };
const float Tile::HEIGHT{ 64.f };

Vector2i Tile::GetCoord() const {
	return m_Coord;
}

const Wall& Tile::GetNorthWall() const
{
	return m_NorthWall;
}

const Wall& Tile::GetWestWall() const
{
	return m_WestWall;
}

bool Tile::HasNorthWall() const
{
	return m_NorthWall.Exists();
}

bool Tile::HasWestWall() const
{
	return m_WestWall.Exists();
}

void Tile::SetCoord(int x, int y) {
	m_Coord = Vector2i{ x, y };
}

void Tile::SetCoord(const Vector2i& coord) {
	m_Coord = coord;
}

int Tile::GetDistToBottomRight(int width) const {
	return (width - m_Coord.x) * (width - m_Coord.x) + m_Coord.y * m_Coord.y;
}

void Tile::CreateWall(bool isNorth, int token, WallRenderable* renderable)
{
	if (isNorth) { // north wall 
		m_NorthWall = Wall{ token, renderable };
	}
	else {
		m_WestWall = Wall{token, renderable};
	}
}

void Tile::CreateFloor(int token, FloorRenderable* renderable) {
	m_Floor = Floor{ token, renderable };
}

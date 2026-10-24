#include "pch.h"
#include "Wall.h"
#include <iostream>
#include "Player.h"
#include "Map.h"

Wall::Wall(int token, WallRenderable* renderable) : m_Exists(true), m_Type(token), m_pRenderable(renderable)
{
}

// -----
// Finds which tile we're trying to move onto
// Depending on the direction, will check if 
//  - the tile itself (North and West)
//  - the next tile (South and East)
// has walls
// -----
bool Wall::IsThereWall(const Vector2f& pos, const Vector2f& mvt, const tile_map_t& map)
{
	const Tile* nextTile = Map::GetTile(map, pos, mvt);
	const Vector2i curCoord{ Map::CoordFromScreenPos(pos) };

	// no wall if current or next tile is out of map and if no change of tile
	if (Map::IsCoordOutOfMap(curCoord) || !nextTile || nextTile->GetCoord() == curCoord) return 0;
	// length ???
	if (abs(nextTile->GetCoord().x - curCoord.x) + abs(nextTile->GetCoord().y - curCoord.y) > 1) {
		if (Player::GetIsSliding())
			return true;
	}

	const Vector2i diff{ nextTile->GetCoord() - curCoord };

	if (diff.x == -1) { // N
		return map.at(curCoord).HasNorthWall();
	}
	if (diff.y == 1) { // W
		return map.at(curCoord).HasWestWall();
	}
	if (diff.x == 1) { // S
		return nextTile->HasNorthWall();
	}
	//if (diff.y == -1) // E
	return nextTile->HasWestWall();
}

Vector2f Wall::WallNormal(const Vector2f& pos, const Vector2f& mvt, const tile_map_t& map)
{
	const Vector2i curCoord{ Map::CoordFromScreenPos(pos) };
	const Vector2f none{};
	const Tile* nextTile = Map::GetTile(map, pos + mvt);

	if (Map::IsCoordOutOfMap(curCoord) || !nextTile || nextTile->GetCoord() == curCoord) return none;

	const Vector2i diff{ nextTile->GetCoord() - curCoord };
	if (diff.y == 1) { // north tile so wall normal is pointing south
		return (map.at(curCoord).HasWestWall() ? Vector2f{ -1.f, -0.5f } : none);
	}
	if (diff.x == -1) { // W tile (points E)
		return (map.at(curCoord).HasNorthWall() ? Vector2f{ 1.f, -0.5f } : none);
	}
	if (diff.y == -1) { // S tile (points N)
		return (nextTile->HasWestWall() ? Vector2f{ 1.f, 0.5f } : none);
	}
	// (diff.x == 1) E tile (points W)
	return (nextTile->HasNorthWall() ? Vector2f{ -1.f, 0.5f } : none);
}

WallRenderable* Wall::GetRenderable() const
{
	return m_pRenderable;
}

bool Wall::Exists() const
{
	return m_Exists;
}

#pragma once
#include <unordered_map>
#include "Vector2i.h"

class WallRenderable;
class Tile;

using tile_map_t = std::unordered_map <Vector2i, Tile>;

class Wall final
{
public:

	explicit Wall() = default;
	explicit Wall(int token, WallRenderable* renderable);

	static Vector2f WallNormal(const Vector2f& pos, const Vector2f& mvt, const tile_map_t& map);
	static bool		IsThereWall(const Vector2f& pos, const Vector2f& mvt, const tile_map_t& map);

	WallRenderable *GetRenderable() const;
	bool			Exists() const;

private:

	bool			m_Exists{};
	int				m_Type{};
	WallRenderable* m_pRenderable{};

};

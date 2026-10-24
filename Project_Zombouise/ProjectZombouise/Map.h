#pragma once
#include "Vector2i.h"
#include <iostream>
#include <vector>
#include <map>
#include <unordered_set>
#include <unordered_map>
#include "Tile.h"
#include <algorithm>
#include "Renderable.h"
#include "TextureManager.h"

using render_queue_t = std::vector<Renderable*>;

class Map final
{
public:
	
	explicit Map() = default;
	~Map();
	Map(const Map& rhs) = delete;
	Map& operator=(const Map& rhs) = delete;
	Map(Map&& rhs) noexcept = default;
	Map& operator=(Map&& rhs) noexcept = default;

	const std::unordered_map<Vector2i, Tile>& GetTilesMap() const;
	
	static const std::unordered_set<Vector2i>& GetBlockedTiles();
	static Vector2i		GetRandFreeTile();
	static const Tile*	GetTile(const std::unordered_map<Vector2i, Tile>& map, const Vector2f& pos);
	static const Tile*	GetTile(const std::unordered_map<Vector2i, Tile>& map, const Vector2f& pos, const Vector2f& mvt);
	static Vector2i		CoordFromScreenPos(const Vector2f& pos);
	static Vector2f		ScreenPosFromCoord(const Vector2i& coord);
	static int			DistFromBottomRight(const Vector2i& coord);
	static bool			IsCoordOutOfMap(const Vector2i& coord);

	void MapParser(render_queue_t& stack, render_queue_t& floorStack);
	void Initialize();
	static int GetWidth();
	static int GetHeight();

private:

	void InterpretToken(render_queue_t& stack, render_queue_t& floorStack, Vector2i coord, const std::string& token);
	void AddWallToStack(bool isNorth, int token, render_queue_t& stack, Tile& tile);

	std::unordered_map<Vector2i, Tile> m_Tiles{};

	TextureManager* m_TextureManager{};

	static int m_Width;
	static int m_Height;

	Texture* floors{};
	Texture* walls{};
	Rectf	floorSrc{ 0.f, 0.f, 62.f, 31.f };
	Rectf	wallSrc{ 0.f, 0.f, 64.f, 96.f };

	static std::unordered_set<Vector2i> m_BlockedTiles;

};


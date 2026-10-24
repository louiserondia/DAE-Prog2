#include "pch.h"
#include "Map.h"
#include <sstream>
#include <fstream>
#include "WallRenderable.h"
#include "FloorRenderable.h"
#include "Renderable.h"

int Map::m_Width = 0;
int Map::m_Height = 0;
std::unordered_set<Vector2i> Map::m_BlockedTiles{};

using tex_bound_pair_t = std::pair<Texture*, Rectf>;
using render_queue_t = std::vector<Renderable*>;

Map::~Map()
{
	m_BlockedTiles.clear();
}

void Map::Initialize()
{
	m_TextureManager = TextureManager::GetInstance();
}

int Map::GetWidth()
{
	return m_Width;
}

int Map::GetHeight()
{
	return m_Height;
}

const std::unordered_map<Vector2i, Tile>& Map::GetTilesMap() const
{
	return m_Tiles;
}

const std::unordered_set<Vector2i>& Map::GetBlockedTiles()
{
	return m_BlockedTiles;
}

// UTILS

Vector2i Map::GetRandFreeTile()
{
	Vector2i tile{ rand() % m_Width, rand() % m_Height };
	while (m_BlockedTiles.count(tile) || IsCoordOutOfMap(tile))
	{
		tile = Vector2i{ rand() % m_Width, rand() % m_Height };
	}
	return tile;
}

const Tile* Map::GetTile(const std::unordered_map<Vector2i, Tile>& map, const Vector2f& pos) {
	const Vector2i coord{ Map::CoordFromScreenPos(pos) };
	if (IsCoordOutOfMap(coord)) return nullptr;

	return &map.at(coord);
}

const Tile* Map::GetTile(const std::unordered_map<Vector2i, Tile>& map, const Vector2f& pos, const Vector2f& mvt) {
	return GetTile(map, pos + mvt);
}

Vector2i Map::CoordFromScreenPos(const Vector2f& pos) {
	float halfW = Tile::WIDTH * 0.5f;
	float halfH = Tile::HEIGHT * 0.5f;

	float gx = (pos.x / halfW - pos.y / halfH) * 0.5f;
	float gy = (pos.x / halfW + pos.y / halfH) * 0.5f;

	return Vector2i{
		static_cast<int>(std::floor(gx)),
		static_cast<int>(std::floor(gy))
	};
}

Vector2f Map::ScreenPosFromCoord(const Vector2i& coord) {
	const Vector2f
		x1{ 0.5f * Tile::WIDTH, -0.5f * Tile::HEIGHT }, // (1, 0) -> (0.5, 0.5)
		y1{ 0.5f * Tile::WIDTH, 0.5f * Tile::HEIGHT }; // (0, 1) -> (0.5, -0.5)

	return Vector2f{ static_cast<float>(coord.x) * x1 + static_cast<float>(coord.y) * y1 };
}

int Map::DistFromBottomRight(const Vector2i& coord) {
	return (m_Width - coord.x) * (m_Width - coord.x) + coord.y * coord.y;
}

bool Map::IsCoordOutOfMap(const Vector2i& coord) {
	return (coord.x < 0 || coord.y < 0 || coord.x >= m_Width || coord.y >= m_Height);
}


// PARSER

void Map::AddWallToStack(bool isNorth, int token, render_queue_t& stack, Tile& tile)
{
	Texture* wallTex{ m_TextureManager->GetTexture(TextureManager::TextureType::wall) };
	Rectf wallBounds{ m_TextureManager->GetBounds(TextureManager::TextureType::wall, token) };
	const tex_bound_pair_t wallPair{ wallTex, wallBounds };

	Vector2f pos{ ScreenPosFromCoord(tile.GetCoord()) };

	std::string name;
	if (!isNorth)
	{ // west wall is offset to the right
		pos.x += Tile::WIDTH / 2;
		name = "west wall";
	}
	else
	{ // for debug
		name = "north wall";
	}
	pos.y += Tile::HEIGHT / 2;

	WallRenderable* wallRend{ new WallRenderable{ pos, wallPair, tile.GetDistToBottomRight(m_Width), 1, name } };
	wallRend->SetDimensions(Tile::WIDTH / 2, Tile::HEIGHT * 2);
	tile.CreateWall(isNorth, token, wallRend);
	stack.push_back(wallRend);
}

void Map::InterpretToken(render_queue_t& stack, render_queue_t& floorStack, Vector2i coord, const std::string& token)
{
	if (token.length() != 3)
		return;

	int floorToken{ token[0] - 48 },
		westWallToken{ token[2] - 48 },
		northWallToken{ token[1] - 48 };
	// ATTENTION j ai inverse 2 et 1 temp

	Tile tile{};
	const Vector2f pos{ ScreenPosFromCoord(coord) };
	Texture* floorTex{ m_TextureManager->GetTexture(TextureManager::TextureType::floor) };
	Rectf floorBounds{ m_TextureManager->GetBounds(TextureManager::TextureType::floor, floorToken) };
	const tex_bound_pair_t floorPair{ floorTex, floorBounds };

	tile.SetCoord(coord);

	FloorRenderable* floorRend{ new FloorRenderable{pos, floorPair, tile.GetDistToBottomRight(m_Width), 0, "floor"} };
	floorRend->SetDimensions(Tile::WIDTH, Tile::HEIGHT);
	tile.CreateFloor(floorToken, floorRend);
	if (floorToken == 2) { // 2 is wood atm
		m_BlockedTiles.insert(coord);
	}
	floorStack.push_back(std::move(floorRend));
	//Renderer::PushFloorRenderable(floorRend);


	if (northWallToken != NO_TEXTURE)
	{
		AddWallToStack(true, northWallToken, stack, tile);
	}
	if (westWallToken != NO_TEXTURE)
	{
		AddWallToStack(false, westWallToken, stack, tile);
	}

	m_Tiles[coord] = tile;
}

static Vector2i GetMapDimensions(std::string mapStr)
{
	std::istringstream iss(mapStr);
	std::string line{};
	Vector2i coord{};

	while (std::getline(iss, line))
	{
		coord.x = 0;
		std::string token{};
		std::stringstream ss{ line };

		while (ss >> token)
		{
			++coord.x;
		}
		++coord.y;
	}
	return coord;
}

void Map::MapParser(render_queue_t& stack, render_queue_t& floorStack)
{
	std::ifstream file("../../images_&_spritesheets/map.txt");
	if (!file)
	{
		std::cerr << "Failed to open map file\n";
	}
	std::stringstream buffer;
	buffer << file.rdbuf();
	const std::string mapStr{ buffer.str() };
	std::istringstream iss{ mapStr };

	std::string line{};
	Vector2i coord{};

	const Vector2i dim{ GetMapDimensions(mapStr) };
	m_Width = dim.x;
	m_Height = dim.y;

	while (std::getline(iss, line)) {
		coord.x = 0;
		std::string token{};
		std::stringstream ss{ line };

		while (ss >> token) {
			InterpretToken(stack, floorStack, coord, token);
			++coord.x;
		}
		++coord.y;
	}
}

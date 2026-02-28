#include "pch.h"
#include "Map.h"
#include <sstream>

typedef std::pair<Texture*, Rectf> pair;

void Map::Initialize() {
	m_TextureManager = TextureManager::GetInstance();
}

void Map::Delete() {
}

Vector2f Map::ScreenPosFromCoord(const Vector2i& coord, float w, float h) {
	return Vector2f{
		(w / 2) * static_cast<float>(coord.x + coord.y),
		(h / 2) * static_cast<float>(coord.y - coord.x) - h / 2
	};
}

void Map::AddWallToStack(int token, std::vector<Renderable>& stack, const Vector2i& coord) {
	const pair wallPair{ m_TextureManager->GetTexture(TextureManager::Type::wall, token) };
	Vector2f pos{ ScreenPosFromCoord(coord, floorSrc.width, floorSrc.height) };

	if (!(token & 1)) { // north wall is offset to the right
		pos.x += floorSrc.width / 2;
	}

	pos.y += floorSrc.height / 2;
	stack.push_back(Renderable{ pos, wallPair, coord });
	pos.y += floorSrc.height / 2;
	stack.push_back(Renderable{ pos, wallPair, coord });
}

void Map::InterpretToken(std::vector<Renderable>& stack, Vector2i coord, const std::string& token) {
	if (token.length() != 3)
		return;

	int floorToken{ token[0] - 48 },
		westWallToken{ token[1] - 48 },
		northWallToken{ token[2] - 48 };

	Tile tile{};
	const Vector2f pos{ ScreenPosFromCoord(coord, floorSrc.width, floorSrc.height) };
	const pair floorPair{ m_TextureManager->GetTexture(TextureManager::Type::floor, floorToken) };

	tile.SetCoord(coord);
	tile.SetWestWallType(westWallToken);
	tile.SetNorthWallType(northWallToken);
	stack.push_back(Renderable{ pos, floorPair, coord });

	if (westWallToken != NO_TEXTURE) {
		AddWallToStack(westWallToken, stack, coord);
	}
	if (northWallToken != NO_TEXTURE) {
		AddWallToStack(northWallToken, stack, coord);
	}

	m_Tiles[coord] = tile;
}

std::vector<Renderable> Map::MapParser() {
	std::vector<Renderable> stack{};
	std::istringstream iss(mapStr);
	std::string line;
	Vector2i coord{};

	while (std::getline(iss, line)) {
		coord.x = 0;
		std::string token{};
		std::stringstream ss{ line };

		while (ss >> token) {
			std::cout << token << " ";
			InterpretToken(stack, coord, token);
			++coord.x;
		}
		++coord.y;
		std::cout << std::endl;
	}

	width = coord.x;
	height = coord.y;
	return stack;
}

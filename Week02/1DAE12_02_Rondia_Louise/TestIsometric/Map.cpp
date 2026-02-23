#include "pch.h"
#include "Map.h"
#include <sstream>

void Map::Initialize() {
	floors = new Texture("floors.png");
	walls = new Texture("walls.png");
}

void Map::Delete() {
	delete floors;
	delete walls;
}

Vector2f Map::ScreenPosFromCoord(const Vector2i& coord, float w, float h) {
	return Vector2f{
		(w / 2) * static_cast<float>(coord.x + coord.y),
		(h / 2) * static_cast<float>(coord.y - coord.x) - h / 2
	};
}

void Map::InterpretToken(std::vector<Renderable>& stack, Vector2i coord, const std::string& token) {
	if (token.length() != 3)
		return;

	Tile tile{};
	Vector2f pos{ ScreenPosFromCoord(coord, floorSrc.width, floorSrc.height) };

	tile.SetCoord(coord);
	
	stack.push_back(Renderable{ pos, floors, floorSrc, coord });
	if (token[1] != '0') {
		tile.SetIsWestWall(true, wallSrc);

		stack.push_back(Renderable{ pos + Vector2f{ 0.f, floorSrc.height / 2 }, walls, wallSrc, coord });
		stack.push_back(Renderable{ pos + Vector2f{ 0.f, floorSrc.height }, walls, wallSrc, coord });
	}
	if (token[2] != '0') {
		Rectf boundsNorth{ wallSrc };
		boundsNorth.left = wallSrc.width;

		stack.push_back(Renderable{ pos + Vector2f{ floorSrc.width / 2, floorSrc.height / 2 }, walls, boundsNorth, coord });
		stack.push_back(Renderable{ pos + Vector2f{ floorSrc.width / 2, floorSrc.height }, walls, boundsNorth, coord });
		tile.SetIsNorthWall(true, wallSrc);
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

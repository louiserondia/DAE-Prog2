#pragma once
#include <iostream>
#include <vector>
#include <map>
#include <unordered_map>
#include "Tile.h"
#include <algorithm>
#include "Renderable.h"
#include "TextureManager.h"

class Map
{
public:
	std::vector<Renderable> MapParser();
	void Initialize();
	void Delete();
	void PrintStack() const;
	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

private:

	int width{};
	int height{};

	Vector2f ScreenPosFromCoord(const Vector2i& coord, float w, float h);
	void InterpretToken(std::vector<Renderable>& stack, Vector2i coord, const std::string& token);

	std::unordered_map<Vector2i, Tile> m_Tiles{};

	TextureManager* m_TextureManager{};

	Texture* floors{};
	Texture* walls{};
	Rectf		floorSrc{ 0.f, 0.f, 128.f, 64.f };
	Rectf		wallSrc{ 0.f, 0.f, 64.f, 96.f };

	const std::string mapStr{ "000 002 000 000\n000 110 022 000\n000 111 101 020\n000 000 000 000\n" };
	//const std::string mapStr{ "000 000\n011 000" }; 
	// 1st digit is floor type (0 grass, 1 wood)
	// 2nd digit is West  wall type (0 none, 1 interior, 2 exterior)
	// 2nd digit is North wall type (0 none, 1 interior, 2 exterior)

};

// map of coordinates as key and value is a Tile class which has texture and Wall infos

//const std::string mapStr{ "00 02 02 00\n04 19 1e 01\n04 17 09 00\n00 08 00 00" };
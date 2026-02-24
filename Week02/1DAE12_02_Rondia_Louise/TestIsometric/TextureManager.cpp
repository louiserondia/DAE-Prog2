#include "pch.h"
#include "TextureManager.h"

TextureManager* TextureManager::instance = nullptr;

TextureManager* TextureManager::GetInstance() {
	if (instance) {
		return instance;
	}
	instance = new TextureManager{};
	return instance;
}

TextureManager::TextureManager() :
	m_FloorsTexture(new Texture{ "floors.png" }),
	m_WallsTexture(new Texture{ "walls.png" })
{}

TextureManager::~TextureManager() {
	delete m_FloorsTexture;
	delete m_WallsTexture;
}


std::pair<Texture *, Rectf> TextureManager::GetTexture(Type type, int index) {
	Texture* texture{};
	Vector2i dim{};
	
	switch (type)
	{
	case TextureManager::Type::floor:
		texture = m_FloorsTexture;
		dim = m_FloorsDim;
		break;
	case TextureManager::Type::wall:
		texture = m_WallsTexture;
		dim = m_WallsDim;
		break;
	default:
		texture = m_FloorsTexture;
		dim = m_WallsDim;
		break;
	}

	const float w{ texture->GetWidth() }, h{ texture->GetHeight() };
	const Rectf bounds{ 
		w / dim.x * (index % dim.x),
		h / dim.y * (index / dim.x),
		w / dim.x,
		h / dim.y
	};
	return std::make_pair(texture, bounds);
}



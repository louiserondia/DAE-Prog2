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
	m_WallsTexture(new Texture{ "walls.png" }),
	m_CharacterTexture(new Texture{ "lion.png" })
{}

TextureManager::~TextureManager() {
	delete m_FloorsTexture;
	delete m_WallsTexture;
	delete m_CharacterTexture;
	delete instance;
	// is it really deleted ?
}


std::pair<Texture *, Rectf> TextureManager::GetTexture(Type type, int index) {
	index -= 1; // 0 is NO_TEXTURE, so it starts at 1
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
	case TextureManager::Type::character:
		texture = m_CharacterTexture;
		return std::make_pair(texture, Rectf{0.f, 0.f, texture->GetWidth() ,texture->GetHeight()});
	default: 
		break;
	}

	// si pas de texture
	const float w{ texture->GetWidth() }, h{ texture->GetHeight() };
	const Rectf bounds{ 
		w / dim.x * (index % dim.x),
		h / dim.y * (index / dim.x),
		w / dim.x,
		h / dim.y
	};
	return std::make_pair(texture, bounds);
}



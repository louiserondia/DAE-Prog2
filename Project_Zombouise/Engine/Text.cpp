#include "Text.h"
#include "Texture.h"
#include <cstring>
#include <SDL_ttf.h>
#include "utils.h"

const Color4f Text::m_White{ 1.f, 1.f, 1.f, 1.f };
const Color4f Text::m_Black{ 0.f, 0.f, 0.f, 1.f };
TTF_Font* Text::m_CorbelFont{};
TTF_Font* Text::m_ConstantiaFont{};

void Text::LoadFonts() 
{
	m_CorbelFont = TTF_OpenFont("../../images_&_spritesheets/CORBEL.ttf", m_DefaultFontSize);
	if (!m_CorbelFont) {
		std::cerr << "Failed to load corbel font! " << TTF_GetError() << std::endl;
	}

	m_ConstantiaFont = TTF_OpenFont("../../images_&_spritesheets/constantia.ttf", m_DefaultFontSize);
	if (!m_ConstantiaFont) {
		std::cerr << "Failed to load constantia font! " << TTF_GetError() << std::endl;
	}
}

Texture* Text::TextureFromString(const std::string& str, FontFamily family, bool isBlack)
{
	TTF_Font* font{};

	switch (family)
	{
	case Text::FontFamily::corbel:
		font = m_CorbelFont;
		break;
	case Text::FontFamily::constantia:
		font = m_ConstantiaFont;
		break;
	default:
		break;
	}

	Texture* texture = new Texture(str, font, isBlack ? m_Black : m_White);
	return texture;
}

void Text::FreeFonts() 
{
	TTF_CloseFont(m_CorbelFont);
	TTF_CloseFont(m_ConstantiaFont);
}
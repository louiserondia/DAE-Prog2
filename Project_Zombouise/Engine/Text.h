#pragma once
#include <iostream>
#include <map>
#include "structs.h"

class Texture;
struct TTF_Font;

class Text
{
public:
    enum class FontFamily {
        corbel,
        constantia
    };

    static void LoadFonts();
    static void FreeFonts();
    static Texture* TextureFromString(const std::string& str, FontFamily family, bool isBlack);

private:

    static TTF_Font* m_CorbelFont;
    static TTF_Font* m_ConstantiaFont;

    static constexpr int m_DefaultFontSize{ 100 };
    static const Color4f m_White;
    static const Color4f m_Black;
};

// Draw a text from a string, the default font size is 100 but can be srinked and set to white or black

// ex :
//	DrawTextFromString("Hello World", Vector2f{ 0.f, 0.f });
//	DrawTextFromString("Hello World", Vector2f{ 0.f, 0.f }, 80);
//	DrawTextFromString("Hello World", Vector2f{ 0.f, 0.f }, 80, 0);
#include "pch.h"
#include "TextureManager.h"
#include "SoundEffect.h"
#include "SoundStream.h"
#include <iostream>
#include "Game.h"
#include "Text.h"

// INIT & CLEAN

TextureManager* TextureManager::GetInstance()
{
	static TextureManager* instance = nullptr;

	if (instance == nullptr)
	{
		instance = new TextureManager{};
	}
	return instance;
}

TextureManager::TextureManager()
{
	m_pTextures[static_cast<int>(TextureType::floor)] = new Texture{ "../../images_&_spritesheets/floors1.png" };
	m_pTextures[static_cast<int>(TextureType::wall)] = new Texture{ "../../images_&_spritesheets/walls2.png" };
	m_pTextures[static_cast<int>(TextureType::player)] = new Texture{ "../../images_&_spritesheets/kate.png" };
	m_pTextures[static_cast<int>(TextureType::zombie)] = new Texture{ "../../images_&_spritesheets/zombie.png" };
	m_pTextures[static_cast<int>(TextureType::zombie2)] = new Texture{ "../../images_&_spritesheets/zombie2.png" };
	m_pTextures[static_cast<int>(TextureType::right_click)] = new Texture{ "../../images_&_spritesheets/right_click.png" };
	m_pBloodTex = new Texture{ "blood_stain.png" };

	m_pSfx[static_cast<int>(SfxType::chase_0)] = new SoundEffect{ "../../images_&_spritesheets/chase_0.mp3" };
	m_pSfx[static_cast<int>(SfxType::chase_1)] = new SoundEffect{ "../../images_&_spritesheets/chase_1.mp3" };
	m_pSfx[static_cast<int>(SfxType::wander_0)] = new SoundEffect{ "../../images_&_spritesheets/wander_0.mp3" };
	m_pSfx[static_cast<int>(SfxType::wander_1)] = new SoundEffect{ "../../images_&_spritesheets/wander_1.mp3" };
	m_pSfx[static_cast<int>(SfxType::wander_2)] = new SoundEffect{ "../../images_&_spritesheets/wander_2.mp3" };

	m_pSfx[static_cast<int>(SfxType::intro)] = new SoundEffect{ "../../images_&_spritesheets/clic.mp3" };
	m_pSfx[static_cast<int>(SfxType::head_smash)] = new SoundEffect{ "../../images_&_spritesheets/HeadSmash.wav" };
	m_pSfx[static_cast<int>(SfxType::push)] = new SoundEffect{ "../../images_&_spritesheets/push.mp3" };
	m_pSfx[static_cast<int>(SfxType::bite)] = new SoundEffect{ "../../images_&_spritesheets/bite.mp3" };

	Text::LoadFonts();
	m_pTextTextures[static_cast<int>(TextType::you_lived_for)] = nullptr;
	m_pTextTextures[static_cast<int>(TextType::you_killed)] = nullptr;
	m_pTextTextures[static_cast<int>(TextType::this_is_how_you_die)] =
		Text::TextureFromString("THIS IS HOW YOU DIED", Text::FontFamily::constantia, false);
	m_pTextTextures[static_cast<int>(TextType::press_to_start)] =
		Text::TextureFromString("Press any key to start...", Text::FontFamily::corbel, false);

	m_pMusics[static_cast<int>(MusicType::start)] = new SoundStream{ "../../images_&_spritesheets/preface.ogg" };
	m_pMusics[static_cast<int>(MusicType::death)] = new SoundStream{ "../../images_&_spritesheets/tuneDeath.ogg" };

	m_pSfx[static_cast<int>(SfxType::wander_0)]->SetVolume(50);
	m_pSfx[static_cast<int>(SfxType::wander_1)]->SetVolume(50);
	m_pSfx[static_cast<int>(SfxType::wander_2)]->SetVolume(50);
}

TextureManager::~TextureManager()
{
	delete m_pBloodTex;
	for (int index = 0; index < m_TextureCount; ++index)
	{
		if (m_pTextures[index] != nullptr)
			delete m_pTextures[index];
	}
	for (int index = 0; index < m_TextCount; ++index)
	{
		if (m_pTextTextures[index] != nullptr)
			delete m_pTextTextures[index];
	}
	for (int index = 0; index < m_SfxCount; ++index)
	{
		if (m_pSfx[index] != nullptr)
			delete m_pSfx[index];
	}
	for (int index = 0; index < m_MusicCount; ++index)
	{
		if (m_pMusics[index] != nullptr)
			delete m_pMusics[index];
	}

	Text::FreeFonts();
}

// GETTERS AND SETTERS

Texture* TextureManager::GetTexture(TextureType type) const
{
	if (type == TextureType::none || type == TextureType::count)
		return nullptr;

	return m_pTextures[static_cast<int>(type)];
}

Texture* TextureManager::GetTextTexture(TextType type) const
{
	if (type == TextType::none || type == TextType::count)
		return nullptr;

	return m_pTextTextures[static_cast<int>(type)];
}

SoundEffect* TextureManager::GetSfx(SfxType type) const
{
	if (type == SfxType::none || type == SfxType::count)
		return nullptr;

	return m_pSfx[static_cast<int>(type)];
}

SoundStream* TextureManager::GetMusic(MusicType type) const
{
	if (type == MusicType::none || type == MusicType::count)
		return nullptr;

	return m_pMusics[static_cast<int>(type)];
}

Texture* TextureManager::GetBloodTex() const
{
	return m_pBloodTex;
}

Rectf TextureManager::GetBounds(TextureType type, int token) const
{
	token -= 1; // 0 is NO_TEXTURE, so it starts at 1
	Texture* texture{ m_pTextures[static_cast<int>(type)] };
	const float width{ texture->GetWidth() }, height{ texture->GetHeight() };

	Vector2i dim{};

	switch (type)
	{
	case TextureType::player:
	case TextureType::zombie:
		return Rectf{ 0.f, 0.f, width, height };
	case TextureType::floor:
		dim = m_FloorsDim;
		break;
	case TextureType::wall:
		dim = m_WallsDim;
		break;
	}

	const Rectf bounds{
		width / dim.x * (token % dim.x),
		height / dim.y * (token / dim.x),
		width / dim.x,
		height / dim.y
	};
	return bounds;
}

void TextureManager::SetGameOverTex()
{
	const int time{ static_cast<int>(Game::GetTimer()) };
	const int minutes{ time % 60 };
	const int hours{ time / 60 };
	const int zombies{ Zombie::GetDeathCounter() };

	std::string h{ " hour" };
	std::string m{ " minute" };
	std::string z{ " zombie" };

	std::string str{ "You survived for " };
	if (hours > 0)
	{
		if (hours > 1)
		{
			h += "s";
		}
		str += std::to_string(hours) + h + " and ";
	}
	if (minutes > 1 || !minutes)
	{
		m += "s";
	}
	str += std::to_string(minutes) + m + ".";

	if (m_pTextTextures[static_cast<int>(TextType::you_lived_for)] != nullptr)
	{
		delete m_pTextTextures[static_cast<int>(TextType::you_lived_for)];
	}
	m_pTextTextures[static_cast<int>(TextType::you_lived_for)] =
		Text::TextureFromString(str, Text::FontFamily::corbel, false);

	if (zombies > 1 || !zombies)
	{
		z += "s";
	}
	str = "You killed " + std::to_string(zombies) + z + ".";

	if (m_pTextTextures[static_cast<int>(TextType::you_killed)] != nullptr)
	{
		delete m_pTextTextures[static_cast<int>(TextType::you_killed)];
	}
	m_pTextTextures[static_cast<int>(TextType::you_killed)] =
		Text::TextureFromString(str, Text::FontFamily::corbel, false);
}

#pragma once
#include "Vector2i.h"

class Texture;
class SoundEffect;
class SoundStream;

#define NO_TEXTURE 0

#define WALL_W_BRICK 1
#define WALL_N_BRICK 2
#define WALL_W_TILE 3
#define WALL_N_TILE 4

#define FLOOR_WOOD_0 1
#define FLOOR_WOOD_1 2


class TextureManager final
{
public:

	enum class TextureType {
		none = -1,
		floor,
		wall,
		player,
		zombie,
		zombie2,
		right_click,
		count
	};

	enum class TextType {
		none = -1,
		you_lived_for,
		you_killed,
		this_is_how_you_die,
		press_to_start,
		count
	};

	enum class SfxType {
		none = -1,
		chase_0,
		chase_1,
		wander_0,
		wander_1,
		wander_2,
		intro,
		head_smash,
		push,
		bite,
		count
	};

	enum class MusicType {
		none = -1,
		start,
		death,
		count
	};

	static TextureManager* GetInstance();
	~TextureManager();

	Texture*		GetTexture(TextureType type) const;
	Texture*		GetTextTexture(TextType type) const;
	SoundEffect*	GetSfx(SfxType type) const;
	SoundStream*	GetMusic(MusicType type) const;
	Texture*		GetBloodTex() const;
	
	void			SetGameOverTex();

	Rectf			GetBounds(TextureType type, int index = 0) const;

private:

	explicit TextureManager();

	TextureManager(const TextureManager& rhs) = delete;
	TextureManager& operator=(const TextureManager& rhs) = delete;
	TextureManager(TextureManager&& rhs) = delete;
	TextureManager& operator=(TextureManager&& rhs) = delete;

	static constexpr int m_TextureCount{ static_cast<int>(TextureType::count) };
	Texture* m_pTextures[m_TextureCount]{};

	static constexpr int m_TextCount{ static_cast<int>(TextType::count) };
	Texture* m_pTextTextures[m_TextCount]{};

	static constexpr int m_SfxCount{ static_cast<int>(SfxType::count) };
	SoundEffect* m_pSfx[m_SfxCount]{};

	static constexpr int m_MusicCount{ static_cast<int>(SfxType::count) };
	SoundStream* m_pMusics[m_MusicCount]{};

	Texture* m_pBloodTex{};

	Vector2i m_FloorsDim{ 3, 2 };
	Vector2i m_WallsDim{ 6, 4 };

};
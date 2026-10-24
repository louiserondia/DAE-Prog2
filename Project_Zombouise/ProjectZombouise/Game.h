#pragma once
#include "BaseGame.h"
#include "Texture.h"
#include "Map.h"
#include "Renderer.h"
#include "Camera.h"
#include "PlayerRenderable.h"
#include "ZombieRenderable.h"
#include "Renderable.h"
#include "FloorRenderable.h"
#include "Player.h"
#include "Zombie.h"
#include "ZombieManager.h"

class Game final : public BaseGame
{
public:
	explicit Game(const Window& window);
	Game(const Game& other) = delete;
	Game& operator=(const Game& other) = delete;
	Game(Game&& other) = delete;
	Game& operator=(Game&& other) = delete;
	// http://isocpp.github.io/CppCoreGuidelines/CppCoreGuidelines#Rh-override
	~Game();

	void Update(float elapsedSec) override;
	void Draw() const override;

	static float	GetTimer();
	static bool*	GetDirKeysState();
	static int		GetDirKeysCount(const bool* keys);
	void			UpdateDirKeysState();
	static Vector2f GetRightClickPos() { return m_RightClickPos; }

	// Event handling
	void ProcessKeyDownEventIntro(const SDL_KeyboardEvent& e);
	void ProcessKeyDownEventPlay(const SDL_KeyboardEvent& e);
	void ProcessKeyDownEventGameOver(const SDL_KeyboardEvent& e);
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;

	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;
	static bool	isDebug;


private:

	enum class State
	{
		intro,
		play,
		gameover
	};

	// FUNCTIONS
	void	Initialize();
	void	Cleanup();
	void	ClearBackground() const;

	void	Restart();
	void	GameOver();

	void	UpdateIntro(float elapsedSec);
	void	UpdatePlay(float elapsedSec);
	void	UpdateGameOver(float elapsedSec);

	Map				m_Map{};
	Camera			m_Camera{};
	Renderer		*m_pRenderer;
	Player			*m_pPlayer;
	ZombieManager	*m_pZombieManager{};

	Vector2i		m_Dir{};
	static float	m_Timer;
	static bool		m_DirKeysState[4];

	State			m_State{};
	static Vector2f	m_RightClickPos;

};

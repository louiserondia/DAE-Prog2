#pragma once
#include "BaseGame.h"
#include "Digit.h";
#include "Clock.h";

class Game : public BaseGame
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

	// Event handling
	void ProcessKeyDownEvent(const SDL_KeyboardEvent& e) override;
	void ProcessKeyUpEvent(const SDL_KeyboardEvent& e) override;
	void ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e) override;
	void ProcessMouseDownEvent(const SDL_MouseButtonEvent& e) override;
	void ProcessMouseUpEvent(const SDL_MouseButtonEvent& e) override;

private:

	// FUNCTIONS
	void Initialize();
	void Cleanup();
	void ClearBackground() const;

	//Digit m_Digit{ 4, Digit::Mode::dark, Vector2f{100.f, 100.f} };
	Clock m_Clocks[4]{ 
		Clock{ Vector2f{0.f, 0.f}, true, 23, 59, Digit::Mode::dark, Color4f{1.f, 0.f, 0.f, 1.f} },
		Clock{ Vector2f{0.f, 200.f}, true, 11, 36, Digit::Mode::bright, Color4f{0.f, 1.f, 0.f, 1.f} },
		Clock{ Vector2f{0.f, 400.f}, false, 11, 36, Digit::Mode::dark, Color4f{1.f, 0.f, 1.f, 1.f} },
		Clock{ Vector2f{0.f, 600.f}, false, 0, 0, Digit::Mode::bright, Color4f{0.f, 0.f, 1.f, 1.f} }
	};
	float m_AccSec{};
	const float m_CoolDown{ .1f };
};
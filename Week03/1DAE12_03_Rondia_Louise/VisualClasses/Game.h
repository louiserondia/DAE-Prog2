#pragma once
#include "BaseGame.h"
#include "RoundButton.h"
#include "Card.h"
#include <vector>

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

	// VARIABLES

	bool		m_IsAcceptBtnDown{};
	RoundButton m_ButtonAccept{ 200.f, 200.f, "button_accept.png" };
	RoundButton m_ButtonPause{ 400.f, 200.f, "button_pauze.png" };
	int m_Index{};

	Card* m_Card{};
	std::vector<Card*> m_Cards{};

};
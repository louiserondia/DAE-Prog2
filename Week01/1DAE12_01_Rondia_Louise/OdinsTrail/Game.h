#pragma once
#include "BaseGame.h"
#include "Texture.h"
#include <vector>
#include "NorseWarrior.h"
#include "Arrow.h"

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
	void PrintPresentation() const;

	void DrawWarriors() const;
	void SelectWarrior(float eventX, float eventY);
	void SelectWarrior(const Vector2f& eventPos);
	void UpdateSelectedWarrior(NorseWarrior& warrior);
	void UpdateWarriorPosition(float elapsedSec);
	void CreateArrow();
	void UpdateIfCollision(Arrow& arrow);

	Texture* m_pBgTexture{};

	std::vector<NorseWarrior> m_Warriors =
	{
		NorseWarrior{ "Torsten", "Shield wall strong!" , Vector2f{377, 48} },
		NorseWarrior{ "Gunnar", "No foe shall stand!", Vector2f{752, 47} },
		NorseWarrior{ "Skalli", "Blood and steel await!", Vector2f{150, 31} },
		NorseWarrior{ "Thorvald", "Let the earth tremble!", Vector2f{567, 31} },
		NorseWarrior{ "Leif", "Honor and peace guide me!", Vector2f{939, 31} },
	};

	NorseWarrior* m_SelectedWarrior = nullptr;

	std::vector<Arrow> m_Arrows{};
	Texture* m_pArrowTexture{};

	Rectf m_ArrHitZones[3]
	{
		Rectf{0, 0, 110, GetViewPort().height / 2},
		Rectf{0, GetViewPort().height / 2, 80.f, GetViewPort().height / 2},
		Rectf{0, 0, GetViewPort().width, 60.f },
	};
};
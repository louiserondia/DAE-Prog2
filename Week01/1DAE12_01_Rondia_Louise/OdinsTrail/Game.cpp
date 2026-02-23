#include "pch.h"
#include "Game.h"
#include <iostream>
#include "utils.h"

Game::Game(const Window& window)
	:BaseGame{ window }
{
	Initialize();
}

Game::~Game()
{
	Cleanup();
}

void Game::Initialize()
{
	m_pBgTexture = new Texture("background.png");
	if (!m_pBgTexture->IsCreationOk())
		std::cerr << "Error when creating background texture\n";

	m_pArrowTexture = new Texture("arrow.png");

	for (NorseWarrior& warrior : m_Warriors) {
		warrior.m_pWarriorTexture = new Texture("NorseWarriors/" + warrior.m_Name + ".png");
		if (!warrior.m_pWarriorTexture->IsCreationOk())
			std::cerr << "Error when creating " + warrior.m_Name + "'s image texture\n";

		warrior.m_pLabelTexture = new Texture(warrior.m_Name, "Norse.otf", 21, Color4f{ 0.35f, 0.25f, 0.18f, 1.f });
		if (!warrior.m_pLabelTexture->IsCreationOk())
			std::cerr << "Error when creating " + warrior.m_Name + "'s label texture\n";

		if (warrior.m_Name == "Torsten") {
			warrior.m_IsSelected = true;
			UpdateSelectedWarrior(warrior);
		}
	}
	PrintPresentation();
}

void Game::Cleanup()
{
	delete m_pBgTexture;
	delete m_pArrowTexture;
	for (NorseWarrior& warrior : m_Warriors) {
		delete warrior.m_pWarriorTexture;
		delete warrior.m_pLabelTexture;
	}
}

void Game::Update(float elapsedSec)
{
	const float mult{ 100.f };

	const Uint8* pStates = SDL_GetKeyboardState(nullptr);
	if (pStates[SDL_SCANCODE_RIGHT])
	{
		UpdateWarriorPosition(elapsedSec * mult);
	}
	if (pStates[SDL_SCANCODE_LEFT])
	{
		UpdateWarriorPosition(elapsedSec * -mult);
	}
	for (Arrow& arrow : m_Arrows) {
		if (arrow.state == Arrow::State::stuck)
			continue;

		arrow.Update(elapsedSec);
		UpdateIfCollision(arrow);
	}

}

void Game::Draw() const
{
	ClearBackground();
	m_pBgTexture->Draw();
	DrawWarriors();
	for (const Arrow& arrow : m_Arrows) {
		arrow.Draw();
	}
	for (int idx{}; idx < 3; ++idx) {
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(m_ArrHitZones[idx], 3);
	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	//std::cout << "KEYDOWN event: " << e.keysym.sym << std::endl;
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	//std::cout << "KEYUP event: " << e.keysym.sym << std::endl;
	//switch ( e.keysym.sym )
	//{
	//case SDLK_LEFT:
	//	//std::cout << "Left arrow key released\n";
	//	break;
	//case SDLK_RIGHT:
	//	//std::cout << "`Right arrow key released\n";
	//	break;
	//case SDLK_1:
	//case SDLK_KP_1:
	//	//std::cout << "Key 1 released\n";
	//	break;
	//}
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	//switch ( e.button )
	//{
	//case SDL_BUTTON_LEFT:
	//	std::cout << " left button " << std::endl;
	//	break;
	//case SDL_BUTTON_RIGHT:
	//	std::cout << " right button " << std::endl;
	//	break;
	//case SDL_BUTTON_MIDDLE:
	//	std::cout << " middle button " << std::endl;
	//	break;
	//}

}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		SelectWarrior(e.x, e.y);
		CreateArrow();
		break;
	case SDL_BUTTON_RIGHT:
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	}
}

void Game::ClearBackground() const
{
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::PrintPresentation() const {
	std::cout << "NORSE WARRIORS REPORTING FOR DUTY !\n";
	for (const NorseWarrior& warrior : m_Warriors) {
		std::cout << warrior.m_Name << " stands prepared ! " << warrior.m_BattleCry << std::endl;
	}
}

void Game::DrawWarriors() const {
	for (const NorseWarrior& warrior : m_Warriors) {
		warrior.DrawWarrior();
		warrior.DrawLabel();
		utils::SetColor(Color4f{ 1, 0, 0, 1 });
		utils::DrawRect(warrior.GetHitbox(), 2.f);
	}
}

void Game::SelectWarrior(float eventX, float eventY) {
	SelectWarrior(Vector2f{ eventX, eventY });
}

void Game::SelectWarrior(const Vector2f& eventPos) {
	bool wasSelected{};

	for (NorseWarrior& warrior : m_Warriors) {
		const Rectf bounds = warrior.GetBounds();
		if (utils::IsPointInRect(eventPos, bounds)) {
			wasSelected = true;
			m_SelectedWarrior = &warrior;
		}
	}
	if (!wasSelected)
		m_SelectedWarrior = nullptr;
}

void Game::UpdateSelectedWarrior(NorseWarrior& warrior) {
	m_SelectedWarrior = &warrior;
}

void Game::UpdateWarriorPosition(float elapsedSec) {
	if (m_SelectedWarrior != nullptr)
		m_SelectedWarrior->MoveWith(elapsedSec, 0.f, GetViewPort());
}

void Game::CreateArrow() {
	m_Arrows.push_back(Arrow{});
	m_Arrows.back().texture = m_pArrowTexture;
	m_Arrows.back().pos.x = static_cast<float>(rand() % static_cast<int>(GetViewPort().width));
	m_Arrows.back().pos.y = GetViewPort().height;
	m_Arrows.back().velocity.x = static_cast<float>(rand() % 1000 - 500);
	m_Arrows.back().velocity.y = -50.f;
}

void Game::UpdateIfCollision(Arrow &arrow) {
	for (NorseWarrior& warrior : m_Warriors) {
		if (utils::IsPointInRect(arrow.pos, warrior.GetHitbox())) {
			arrow.state = Arrow::State::stuck;
			warrior.m_HealthPercent -= 12.8f;
		}
	}
	for (const Rectf& hitZone : m_ArrHitZones) {
		if (utils::IsPointInRect(arrow.pos, hitZone)) {
			arrow.state = Arrow::State::stuck;
		}
	}
}
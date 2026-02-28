#include "pch.h"
#include "Game.h"

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
	for (int suitIdx{ 1 }; suitIdx <= 4; ++suitIdx) {
		for (int rankIdx{ 2 }; rankIdx <= 14; ++rankIdx) {
			m_Cards.push_back(new Card{ static_cast<Card::Suit>(suitIdx), rankIdx });
		}
	}
	m_Index = rand() % 52;
}

void Game::Cleanup()
{
	delete m_Card;
	for (int idx{}; idx < m_Cards.size(); ++idx) {
		delete m_Cards[idx];
	}
}

void Game::Update(float elapsedSec)
{
	// Check keyboard state
	//const Uint8 *pStates = SDL_GetKeyboardState( nullptr );
	//if ( pStates[SDL_SCANCODE_RIGHT] )
	//{
	//	std::cout << "Right arrow key is down\n";
	//}
	//if ( pStates[SDL_SCANCODE_LEFT] && pStates[SDL_SCANCODE_UP])
	//{
	//	std::cout << "Left and up arrow keys are down\n";
	//}
}

static int GetIndex(int row, int col, int nCol) {
	return row * nCol + col;
}

static int GetRow(int index, int nCol) {
	return index / nCol;
}

static int GetCol(int index, int nCol) {
	return index % nCol;
}

void Game::Draw() const
{
	ClearBackground();
	m_ButtonAccept.Draw();
	m_ButtonPause.Draw();

	const int nRow{ 4 }, nCol{ 13 };
	const float width{ GetViewPort().width / nCol },
		height{ GetViewPort().height / nRow };
	/*Rectf bounds{ 0, 0, width, height };

	for (int row{}; row < nRow; ++row) {
		bounds.left = 0;
		for (int col{}; col < nCol; ++col) {
			m_Cards[GetIndex(row, col, nCol)]->Display(bounds);
			bounds.left += width;
		}
		bounds.bottom += height;
	}*/

	Rectf bounds{ 0.f, 0.f, width, height};
	m_Cards[m_Index]->Display(bounds);
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
	const Vector2f pos{ static_cast<float>(e.x), static_cast<float>(e.y) };
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		if (m_ButtonAccept.CheckHit(pos)) {
			m_IsAcceptBtnDown = true;
			m_ButtonAccept.Press();
		}
		if (m_ButtonPause.CheckHit(pos))
			m_ButtonPause.IsPressed() ? m_ButtonPause.Release() : m_ButtonPause.Press();
		break;
	case SDL_BUTTON_RIGHT:
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	}

}

void Game::ProcessMouseUpEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		if (m_IsAcceptBtnDown) {
			m_IsAcceptBtnDown = false;
			m_ButtonAccept.Release();
		}
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

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
	m_Map.Initialize();
	m_Renderer = Renderer(m_Map.MapParser());
	m_Renderer.SetDimensions(Map::GetWidth(), m_Map.GetHeight());
	m_Character = m_Renderer.CreateCharacter();
	m_Renderer.SortStack();
}

void Game::Cleanup()
{
	m_Map.Delete();
}

Vector2i ScreenCoordFromPo(const Vector2f& pos, float w, float h) {
	float halfW = w * 0.5f;
	float halfH = h * 0.5f;

	float gx = (pos.x / halfW - pos.y / halfH) * 0.5f;
	float gy = (pos.x / halfW + pos.y / halfH) * 0.5f;

	return Vector2i{
		static_cast<int>(std::floor(gx)),
		static_cast<int>(std::floor(gy))
	};
}

void Game::Update(float elapsedSec)
{
	const float speed{ 100.f };
	if (m_Dir.x || m_Dir.y) {
		m_Camera.Add(elapsedSec * m_Dir.x * speed, elapsedSec * m_Dir.y * speed);
		m_Character->SetPos(m_Camera.Get());
		//std::cout << m_Camera.Get().x << " y " << m_Camera.Get().y << std::endl;
		//std::cout << ScreenCoordFromPo(m_Camera.Get(), 128.f, 64.f).x << " coord y " << ScreenCoordFromPo(m_Camera.Get(), 128.f, 64.f).y << std::endl;
		m_Character->SetCoord(ScreenCoordFromPo(m_Camera.Get(), 128.f, 64.f));
		m_Renderer.SortStack();
	}

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

void Game::Draw() const
{
	ClearBackground();
	m_Renderer.DrawMap(GetViewPort(), m_Camera, m_Dir);
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	switch (e.keysym.sym)
	{
	case SDLK_LEFT:
		m_Dir.x = -1;
		break;
	case SDLK_RIGHT:
		m_Dir.x = 1;
		break;
	case SDLK_DOWN:
		m_Dir.y = -1;
		break;
	case SDLK_UP:
		m_Dir.y = 1;
		break;
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	switch ( e.keysym.sym )
	{
	case SDLK_LEFT:
		m_Dir.x = 0;
		break;
	case SDLK_RIGHT:
		m_Dir.x = 0;
		break;
	case SDLK_DOWN:
		m_Dir.y = 0;
		break;
	case SDLK_UP:
		m_Dir.y = 0;
		break;
	//case SDLK_KP_1:
	//	break;
	}
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	//std::cout << "MOUSEMOTION event: " << e.x << ", " << e.y << std::endl;
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	//std::cout << "MOUSEBUTTONDOWN event: ";
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
	//std::cout << "MOUSEBUTTONUP event: ";
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

void Game::ClearBackground() const
{
	glClearColor(1.0f, 1.0f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	//glEnable(GL_DEPTH_TEST);
	//glDepthFunc(GL_LEQUAL);
	//glMatrixMode(GL_PROJECTION);
	//glLoadIdentity();
	//gluPerspective(45.0, (GLfloat)width / (GLfloat)height, 0.1, 100.0); // ou glFrustum, ou glm::perspective si vous utilisez GLM

	//glMatrixMode(GL_MODELVIEW);
	//glLoadIdentity();
}

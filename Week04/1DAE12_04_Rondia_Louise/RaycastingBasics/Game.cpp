#include "pch.h"
#include "Game.h"
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

}

void Game::Cleanup()
{
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

void Game::Draw() const
{
	ClearBackground();

	const Color4f 
		lineColor{ 1.f, 1.f, 1.f, 1.f },
		intersectColor{ 0.f, 0.f, 1.f, 1.f },
		inBoundsColor{ 0.f, 1.f, 0.f, 1.f },
		outBoundsColor{ 1.f, 0.f, 0.f, 1.f },
		normalColor{ 0.f, 1.f, 1.f, 1.f },
		reflectionColor{ 1.f, 1.f, 0.f, 1.f };

	const Vector2f startPosition{ 100.0f, 100.0f };

	std::vector<Vector2f> pointVector{};
	utils::HitInfo hitInfo{};

	pointVector.push_back(Vector2f{ 200.0f, 300.0f });
	pointVector.push_back(Vector2f{ 600.0f, 300.0f });

	utils::SetColor(lineColor);
	utils::DrawLine(startPosition, m_MousePosition);
	utils::DrawLine(pointVector[0], pointVector[1]);

	if (utils::Raycast(pointVector, startPosition, m_MousePosition, hitInfo))
	{
		const float intersectPointRadius{ 10.0f };
		const Vector2f deltaVector{ m_MousePosition - startPosition };

		utils::SetColor(intersectColor);
		utils::DrawEllipse(hitInfo.intersectPoint, intersectPointRadius, intersectPointRadius);

		utils::SetColor(inBoundsColor);
		utils::DrawLine(startPosition, startPosition + deltaVector * hitInfo.lambda);

		utils::SetColor(outBoundsColor);
		utils::DrawLine(hitInfo.intersectPoint, hitInfo.intersectPoint + deltaVector * (1.f - hitInfo.lambda));

		utils::SetColor(normalColor);
		utils::DrawLine(hitInfo.intersectPoint, hitInfo.intersectPoint + hitInfo.normal * 20.f);

		const Vector2f reflection{ deltaVector.Reflect(hitInfo.normal) };
		utils::SetColor(reflectionColor);
		utils::DrawLine(hitInfo.intersectPoint, hitInfo.intersectPoint + reflection * (1.f - hitInfo.lambda));

	}
}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{

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
	m_MousePosition = Vector2f{ static_cast<float>(e.x), static_cast<float>(e.y) };
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
	glClearColor(0.0f, 0.0f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}
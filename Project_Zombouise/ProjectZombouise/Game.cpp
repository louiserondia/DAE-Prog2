#include "pch.h"
#include "Game.h"
#include "SoundEffect.h"
#include "SoundStream.h"

float Game::m_Timer{};
bool Game::m_DirKeysState[4]{};
bool Game::isDebug{};
Vector2f Game::m_RightClickPos{};

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
	std::cout << "\
	 ____            _           _                    \n\
	|  _ \\ _ __ ___ (_) ___  ___| |_\n\
	| |_) | '__/ _ \\| |/ _ \\/ __| __|                 \n\
	|  __/| | | (_) | | __ / (__| |_\n\
	|_|___|_|  \\___// |\\___|\\___|\\__|      _          \n\
	|__  /___  _ _|__/_ | |__   ___  _   _(_)___  ___\n\
	  / // _ \\| '_ ` _ \\| '_ \\ / _ \\| | | | / __|/ _ \\\n\
	 / /| (_) | | | | | | |_) | (_) | |_| | \\__ \\  __/\n\
	/____\\___/|_| |_| |_|_.__/ \\___/ \\__,_|_|___/\\___|\n\
		";
	std::cout << "\n\n\
------------------------------\n\
Press ASWD to move around\n\
Shift to run\n\
Ctrl to slow walk\n\
Right click to look around\n\
Space to push - if zombie falls,\n\
press space again to stomp on their head\n\
Press space to restart\n\
------------------------------\n\
";
	TextureManager::GetInstance()->GetMusic(TextureManager::MusicType::start)->Play(0);

	m_pRenderer = Renderer::GetInstance();

	m_Map.Initialize();
	m_Map.MapParser(m_pRenderer->GetRenderQueue(), m_pRenderer->GetFloorRenderQueue());

	m_pPlayer = new Player{};

	m_pZombieManager = new ZombieManager{};
	m_pZombieManager->AddZombiesToRenderQueue(*m_pRenderer);

	m_pRenderer->SetDimensions(Map::GetWidth(), Map::GetHeight());
	m_pRenderer->GetRenderQueue().push_back(m_pPlayer->GetRenderable());
	m_pRenderer->SortRenderQueue();
}

void Game::Cleanup()
{
	delete TextureManager::GetInstance();
	delete Renderer::GetInstance();
	delete m_pZombieManager;
	delete m_pPlayer;
}

void Game::UpdateIntro(float elapsedSec)
{
	m_pRenderer->UpdateIntro(elapsedSec);
}

void Game::UpdatePlay(float elapsedSec)
{
	const int w{ m_Map.GetWidth() }, h{ m_Map.GetHeight() };
	const Vector2i lastCoord{ m_pPlayer->GetCoord() };

	UpdateDirKeysState();
	m_pPlayer->Update(elapsedSec, m_Map);
	m_Camera.SetPos(m_pPlayer->GetRenderable()->GetPos());
	m_pZombieManager->Update(elapsedSec, m_Map, *m_pPlayer, *m_pRenderer);

	m_pRenderer->SortRenderQueue();
	if (lastCoord.x != m_pPlayer->GetCoord().x || lastCoord.y != m_pPlayer->GetCoord().y)
	{
		m_pZombieManager->OnPlayerTileChange();
	}

	if (m_pPlayer->GetState(Player::State::dead))
	{
		GameOver();
	}

	m_Timer += elapsedSec;
}

void Game::UpdateGameOver(float elapsedSec)
{
	m_pRenderer->UpdateGameOver(elapsedSec);
}

void Game::Update(float elapsedSec)
{
	switch (m_State)
	{
	case State::intro:
		UpdateIntro(elapsedSec);
		break;
	case State::play:
		UpdatePlay(elapsedSec);
		break;
	case State::gameover:
		UpdateGameOver(elapsedSec);
		break;
	default:
		break;
	}
}

void Game::Draw() const
{
	ClearBackground();

	switch (m_State)
	{
	case State::intro:
		m_pRenderer->DrawIntroScreen(GetViewPort());
		break;
	case State::play:
		m_pRenderer->Draw(GetViewPort(), *m_pPlayer);
		break;
	case State::gameover:
		m_pRenderer->DrawGameOverScreen(GetViewPort());
		break;
	default:
		break;
	}
}

void Game::UpdateDirKeysState()
{
	if (m_State != State::play)
		return;

	const Uint8* state = SDL_GetKeyboardState(nullptr);

	m_DirKeysState[0] = static_cast<int>(state[SDL_SCANCODE_W]);
	m_DirKeysState[1] = static_cast<int>(state[SDL_SCANCODE_S]);
	m_DirKeysState[2] = static_cast<int>(state[SDL_SCANCODE_A]);
	m_DirKeysState[3] = static_cast<int>(state[SDL_SCANCODE_D]);

}

void Game::ProcessKeyDownEventIntro(const SDL_KeyboardEvent& e)
{
	m_State = State::play;
	TextureManager::GetInstance()->GetMusic(TextureManager::MusicType::start)->Stop();
	TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::intro)->Play(0);
}

void Game::ProcessKeyDownEventPlay(const SDL_KeyboardEvent& e)
{
	if (e.keysym.scancode == SDL_SCANCODE_LSHIFT)
	{
		m_pPlayer->SetState(Player::State::run, 1);
	}
	else if (e.keysym.scancode == SDL_SCANCODE_LCTRL)
	{
		m_pPlayer->SetState(Player::State::crouch, 1);
	}
	else if (e.keysym.scancode == SDL_SCANCODE_SPACE)
	{
		Zombie* target{ m_pZombieManager->GetClosestToPlayer(*m_pPlayer) };
		m_pPlayer->TryAttack(target);
	}
	else if (e.keysym.scancode == SDL_SCANCODE_G)
	{
		GameOver();
	}
	else if (e.keysym.scancode == SDL_SCANCODE_I)
	{
		m_State = State::intro;
	}
	else if (e.keysym.scancode == SDL_SCANCODE_T)
	{
		isDebug = !isDebug;
	}
}

void Game::ProcessKeyDownEventGameOver(const SDL_KeyboardEvent& e)
{
	BaseGame::ProcessKeyDownEvent(e);

	if (e.keysym.scancode == SDL_SCANCODE_SPACE)
	{
		Restart();
	}

}

void Game::ProcessKeyDownEvent(const SDL_KeyboardEvent& e)
{
	switch (m_State)
	{
	case State::intro:
		ProcessKeyDownEventIntro(e);
		break;
	case State::play:
		ProcessKeyDownEventPlay(e);
		break;
	case State::gameover:
		ProcessKeyDownEventGameOver(e);
		break;
	default:
		break;
	}
}

void Game::ProcessKeyUpEvent(const SDL_KeyboardEvent& e)
{
	if (m_State != State::play)
		return;

	if (e.keysym.scancode == SDL_SCANCODE_LSHIFT)
	{
		m_pPlayer->SetState(Player::State::run, 0);
	}
	else if (e.keysym.scancode == SDL_SCANCODE_LCTRL)
	{
		m_pPlayer->SetState(Player::State::crouch, 0);
	}
}

void Game::ProcessMouseMotionEvent(const SDL_MouseMotionEvent& e)
{
	m_RightClickPos = Vector2f{ static_cast<float>(e.x), static_cast<float>(e.y) };
	if (m_pPlayer->GetState(Player::State::observe))
	{
		m_pPlayer->UpdateFovWhenObserving(m_RightClickPos);
	}
}

void Game::ProcessMouseWheelEvent(const SDL_MouseWheelEvent& e)
{
	if (e.y > 0)
	{
		m_Camera.ZoomIn();
	}
	else if (e.y < 0)
	{
		m_Camera.ZoomOut();
	}
}

void Game::ProcessMouseDownEvent(const SDL_MouseButtonEvent& e)
{
	switch (e.button)
	{
	case SDL_BUTTON_LEFT:
		break;
	case SDL_BUTTON_RIGHT:
		SDL_ShowCursor(SDL_DISABLE);
		m_RightClickPos = Vector2f{ static_cast<float>(e.x), static_cast<float>(e.y) };
		m_pPlayer->UpdateFovWhenObserving(m_RightClickPos);
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
		break;
	case SDL_BUTTON_RIGHT:
		SDL_ShowCursor(SDL_ENABLE);
		m_pPlayer->UpdateFovWhenObserving();
		break;
	case SDL_BUTTON_MIDDLE:
		break;
	}
}

void Game::ClearBackground() const
{
	glClearColor(1.0f, 1.0f, 1.f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
}

void Game::Restart()
{
	m_pPlayer->Reset();
	m_pZombieManager->Reset();
	for (int idx{}; idx < 4; ++idx)
	{
		m_DirKeysState[idx] = false;
	}

	m_Timer = 0.f;
	m_State = State::play;
	Zombie::ResetDeathCounter();
	TextureManager::GetInstance()->GetMusic(TextureManager::MusicType::death)->Stop();
}

void Game::GameOver()
{
	m_State = State::gameover;
	TextureManager::GetInstance()->SetGameOverTex();
	TextureManager::GetInstance()->GetMusic(TextureManager::MusicType::death)->Play(0);
	TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::wander_0)->StopAll();
	TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::wander_1)->StopAll();
	TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::wander_2)->StopAll();
	TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::chase_0)->StopAll();
	TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::chase_1)->StopAll();
	m_pRenderer->ResetGameOverTimer();
}

float Game::GetTimer()
{
	return m_Timer;
}

bool* Game::GetDirKeysState()
{
	return m_DirKeysState;
}

int Game::GetDirKeysCount(const bool* keys)
{
	int count{};

	for (int idx{}; idx < 4; ++idx) {
		count += keys[idx] ? 1 : 0;
	}
	return count;
}

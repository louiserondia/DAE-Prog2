#pragma once
#include <vector>
#include <functional>
#include "Renderable.h"
#include "Player.h"

using render_queue_t = std::vector<Renderable *>;

class Renderer final
{
public:

	//reserve for map size + zombie
	static Renderer* GetInstance();
	~Renderer();
	Renderer(const Renderer& rhs) = delete;
	Renderer& operator=(const Renderer& rhs) = delete;
	Renderer(Renderer&& rhs) = delete;
	Renderer& operator=(Renderer&& rhs) = delete; 
	// no move or copy because Renderer is a singleton
	
	void	CleanUp();
	void	UpdateIntro(float elapsedSec);
	void	UpdateGameOver(float elapsedSec);

	void	DrawIntroScreen(const Rectf& viewport) const;
	void	Draw(const Rectf& viewPort, const Player& Player) const;
	void	DrawGameOverScreen(const Rectf& viewport) const;
	
	void	ResetGameOverTimer();
	void	SetDimensions(int width, int height);

	render_queue_t& GetRenderQueue();
	render_queue_t& GetFloorRenderQueue();

	void			SortRenderQueue();
	static float	GetMaxRenderDist();

private:
	
	explicit	Renderer();

	void		DrawScreenTextTexture(const Texture* texture, const Vector2f& center, float fontSize) const;
	void		DrawEmptiness(const Rectf& viewPort) const;
	void		DrawFogOfWar(const Player& Player) const;
	void		DrawRightClick(const Player& player) const;
	void		UpdateIntroFadeValues(float elapsedSec);


	int		m_Width{};
	int		m_Height{};

	Rectf	m_DarkRectBottom{};
	Rectf	m_DarkRectTop{};
	float	m_AlphaBottom{ 1.f };
	float	m_AlphaTop{ 1.f };
	float	m_IntroTimer{};
	float	m_GameOverTimer{};

	Vector2f m_GameOverTextPos{};

	render_queue_t m_RenderQueue{};
	render_queue_t m_FloorRenderQueue{};
};


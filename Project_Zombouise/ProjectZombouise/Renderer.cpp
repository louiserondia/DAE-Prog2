#include "pch.h"
#include "Renderer.h"
#include "utils.h"
#include "TextureManager.h"
#include "Texture.h"
#include "Camera.h"
#include <string>
#include "Game.h"
#include "Map.h"
#include "Tile.h"

using tex_bound_pair_t = std::pair<Texture*, Rectf>;

// INIT AND CLEAN

Renderer* Renderer::GetInstance()
{
	static Renderer* instance = nullptr;

	if (instance == nullptr)
	{
		instance = new Renderer{};
	}
	return instance;
}

Renderer::Renderer() :
	m_DarkRectBottom(Rectf{ 0.f, 0.f, Camera::VIEWPORT.x, Camera::VIEWPORT.y / 2 }),
	m_DarkRectTop(Rectf{ 0.f,Camera::VIEWPORT.y / 2, Camera::VIEWPORT.x, Camera::VIEWPORT.y / 2 }),
	m_GameOverTextPos{ Camera::VIEWPORT.x / 2, 0.f }
{
}

Renderer::~Renderer()
{
	CleanUp();
}

void Renderer::CleanUp() {
	for (int idx{}; idx < m_RenderQueue.size(); ++idx) {
		delete m_RenderQueue[idx];
		m_RenderQueue[idx] = nullptr;
	}
	for (int idx{}; idx < m_FloorRenderQueue.size(); ++idx) {
		delete m_FloorRenderQueue[idx];
		m_FloorRenderQueue[idx] = nullptr;
	}
	m_RenderQueue.clear();
	m_FloorRenderQueue.clear();
}

void Renderer::ResetGameOverTimer()
{
	m_GameOverTimer = 0.f;
}

// UPDATE

void Renderer::UpdateIntro(float elapsedSec)
{
	UpdateIntroFadeValues(elapsedSec);
}

void Renderer::UpdateGameOver(float elapsedSec)
{
	if (m_GameOverTimer * 30 > Camera::VIEWPORT.y / 2)
		return;

	m_GameOverTimer += elapsedSec;
	m_GameOverTextPos.y = std::min( Camera::VIEWPORT.y / 2, m_GameOverTimer * 30);
}

void Renderer::UpdateIntroFadeValues(float elapsedSec)
{
	static const float max{ 2.5f };

	m_IntroTimer += elapsedSec;
	m_AlphaTop = std::max(0.f, 1.f - m_IntroTimer / 2);

	if (m_IntroTimer > max && m_IntroTimer < max * 2)
	{
		m_AlphaBottom = std::max(0.f, 1.f - (m_IntroTimer - max) / 2);
	}
	else if (m_IntroTimer > max * 2)
	{
		m_AlphaBottom = sinf((m_IntroTimer - max * 2 - 1) * 2) / 4 + 0.25f;
	}
}

void Renderer::DrawIntroScreen(const Rectf& viewport) const
{
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f }); // lerp l'alpha
	utils::FillRect(viewport);

	Texture* texture{ TextureManager::GetInstance()->GetTextTexture(TextureManager::TextType::this_is_how_you_die) };
	Vector2f pos{ viewport.width / 2, viewport.height / 2 };

	DrawScreenTextTexture(texture, pos, 0.33f);

	texture = TextureManager::GetInstance()->GetTextTexture(TextureManager::TextType::press_to_start);
	pos.y = 30.f;
	DrawScreenTextTexture(texture, pos, 0.18f);

	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, m_AlphaBottom });
	utils::FillRect(m_DarkRectBottom);
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, m_AlphaTop });
	utils::FillRect(m_DarkRectTop);

}

// DRAW

void Renderer::Draw(const Rectf& viewPort, const Player& player) const {
	const Vector2f cam{ Camera::GetPos() };
	const Vector2i camCoord{ Map::CoordFromScreenPos(cam) };

	glPushMatrix();
	{
		const float scale{ Camera::GetScale() };
		glScalef(1.f * scale, 1.f * scale, 0.f);
		glTranslatef((-cam.x + viewPort.width / 2) / scale, (-cam.y + viewPort.height / 2) / scale, 0.f);

		DrawEmptiness(viewPort);

		for (int idx{}; idx < m_FloorRenderQueue.size(); ++idx) {
			m_FloorRenderQueue[idx]->Draw();
		}

		for (int idx{}; idx < m_RenderQueue.size(); ++idx) {
			m_RenderQueue[idx]->Draw();
		}

		DrawFogOfWar(player);
	}
	glPopMatrix();
	DrawRightClick(player);
}

void Renderer::DrawGameOverScreen(const Rectf& viewport) const
{
	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 1.f }); // lerp l'alpha
	utils::FillRect(viewport);

	Vector2f pos{ m_GameOverTextPos};
	DrawScreenTextTexture(TextureManager::GetInstance()->GetTextTexture(TextureManager::TextType::you_lived_for), pos, 0.25f);
	pos.y -= 30.f;
	DrawScreenTextTexture(TextureManager::GetInstance()->GetTextTexture(TextureManager::TextType::you_killed), pos, 0.25f);
}

void Renderer::DrawScreenTextTexture(const Texture* texture, const Vector2f& center, float fontSize) const
{
	if (texture == nullptr) return;

	const float width{ texture->GetWidth() * fontSize };
	const float height{ texture->GetHeight() * fontSize };

	Rectf dst{
		center.x - width / 2,
		center.y,
		width,

		height
	};
	texture->Draw(dst);
}

void Renderer::DrawEmptiness(const Rectf& viewPort) const {
	const int radius{ static_cast<int>(15 / Camera::GetScale()) };
	const Vector2f cam{ Camera::GetPos() / Camera::GetScale() };
	const Vector2i camCoord{ Map::CoordFromScreenPos(cam) };
	Vector2i start{ camCoord - Vector2i{radius, radius} };

	for (int x{}; x < radius * 2; ++x) {
		for (int y{}; y < radius * 2; ++y) {
			const Vector2f pos{ Map::ScreenPosFromCoord(Vector2i{(start.x + x) , (start.y + y)}) };
			m_FloorRenderQueue[0]->DrawAt(pos + Vector2f{ 0.f, Tile::HEIGHT / 2 });
		}
	}
}

void Renderer::DrawFogOfWar(const Player& player) const
{
	const float c{ 0.707f }, s{ 0.707f };
	Vector2f p1{ player.GetRenderable()->GetPos() };
	p1 -= player.GetFovDir().Normalized() * 10;
	const Vector2f d{ player.GetFovDir().Normalized() * 1000 / Camera::GetScale() };
	const Vector2f p2{ p1.x + (d.x * c - d.y * s), p1.y + (d.x * s + d.y * c) };
	const Vector2f p3{ p1.x + (d.x * c + d.y * s), p1.y + (-d.x * s + d.y * c) };
	const Vector2f p4{ p3 - d * 2 };
	const Vector2f p5{ p2 - d * 2 };

	std::vector<Vector2f> poly{};
	poly.push_back(p1);
	poly.push_back(p3);
	poly.push_back(p4);
	poly.push_back(p5);
	poly.push_back(p2);

	utils::SetColor(Color4f{ 0.f, 0.f, 0.f, 0.15f });
	utils::FillPolygon(poly);
}

void Renderer::DrawRightClick(const Player& player) const
{
	if (!player.GetState(Player::State::observe)) return;
	Texture* tex{ TextureManager::GetInstance()->GetTexture(TextureManager::TextureType::right_click) };
	const float w{ tex->GetWidth() / 2.f * Camera::GetScale() };
	const float	h{ tex->GetHeight() / 2.f * Camera::GetScale() };

	Rectf bounds{ Game::GetRightClickPos().x - w / 2, Game::GetRightClickPos().y - 15.f, w, h };
	tex->Draw(bounds);

}

// GETTERS AND SETTERS

float Renderer::GetMaxRenderDist() {
	return Camera::VIEWPORT.x * Camera::GetScale();
}

void Renderer::SetDimensions(int width, int height) {
	m_Width = width;
	m_Height = height;
}

render_queue_t& Renderer::GetRenderQueue()
{
	return m_RenderQueue;
}

render_queue_t& Renderer::GetFloorRenderQueue()
{
	return m_FloorRenderQueue;
}

// UTILS

void Renderer::SortRenderQueue()
{
	std::sort(m_RenderQueue.rbegin(), m_RenderQueue.rend(), Renderable::CompareRenderables);

	// Debug : print queue
	//for (const Renderable * item : m_RenderQueue) {
	//	std::cout << "name : " << item->name
	//		<< ", dist : " << item->GetDist()
	//		<< ", priority : " << item->GetPriority()
	//		<< std::endl;
	//}
	//std::cout << "-------------------------------\n";
}

#include "pch.h"
#include "utils.h"
#include "WalkingRenderable.h"
#include <iostream>
#include "Tile.h"
#include "Game.h"

// INIT & CLEAN

WalkingRenderable::WalkingRenderable(
	const Vector2f& pos,
	std::pair<Texture*, Rectf> texturePair,
	int dist,
	int priority,
	const std::string& str
) : Renderable(pos, texturePair, dist, priority, str)
{
}

WalkingRenderable::~WalkingRenderable()
{
	m_AnimFrames.clear();
	m_CurAnimFrameStr.clear();
}

void WalkingRenderable::Reset(const Vector2i& coord) {
	m_Fps = 2;
	m_FrameTimer = 0.f;
	m_HasArmsUp = false;
	m_Pos = Vector2f{ coord.x * Tile::WIDTH + 128.f / 2,  coord.y * Tile::HEIGHT };
	SetToNewAnimState('I');
}

// UPDATE

void WalkingRenderable::Update(float elapsedSec) {
	UpdateAnimation(elapsedSec);

	if (m_HalfSec > 0.2f)
	{
		m_HalfSec = 0.f;
		UpdateFrameBounds();
	}

	m_HalfSec += elapsedSec;
	m_FrameDst.left = m_Pos.x - m_FrameDst.width / 2;
	m_FrameDst.bottom = m_Pos.y - 20.f; // little offset for the sprite gap
}

void WalkingRenderable::UpdateAnimation(float elapsedSec)
{
	const int frameIndex{ static_cast<int>(m_FrameTimer * m_Fps) };
	if ((m_CurAnimFrameStr[0] == 'P' || m_CurAnimFrameStr[0] == 'S') && frameIndex == m_AnimFrames[m_CurAnimFrameStr].nrFrames)
	{
		SetToNewAnimState('I');
		UpdateFrameBounds();
		return;
	}
	const AnimFrame animFrame{ m_AnimFrames[m_CurAnimFrameStr] };
	m_FrameSrc.left = (animFrame.col + (frameIndex % animFrame.nrFrames)) * m_FrameSrc.width;
	m_FrameTimer += elapsedSec;
}

void WalkingRenderable::UpdateAnimFromKeys(const bool* keys, const Vector2f& dir)
{
	if (dir == Vector2f{})
		SetAnimFrameStrFromKeys(keys);
	else
		SetAnimFrameStrFromFov(keys, dir);
}

void WalkingRenderable::UpdateAnimFromDir(const Vector2f& dir)
{
	std::string str{ m_CurAnimFrameStr };

	// dont change with the direction if not walking, idling or observing
	if (str[0] != 'W' && str[0] != 'I' && str[0] != 'O')
		return;

	if (!dir.x && !dir.y) {
		str[0] = 'I';
	}
	else {
		str.clear();
		str += 'W';
	}

	if (dir.y > 0.f)
		str += "N";
	else if (dir.y < 0.f)
		str += "S";
	if (dir.x < 0.f)
		str += "W";
	else if (dir.x > 0.f)
		str += "E";

	m_CurAnimFrameStr = str;
}

void WalkingRenderable::SetAnimFrameStrFromFov(const bool* keys, const Vector2f& dir)
{
	std::string str{ m_CurAnimFrameStr };
	const int keysCount{ Game::GetDirKeysCount(keys) };

	str.clear();
	if (!keysCount) {
		str += 'I';
	}
	else {
		str += 'W';
	}

	const float margin{ 0.5f };
	if (dir.y > margin)
		str += "N";
	else if (dir.y < -margin)
		str += "S";
	if (dir.x < -margin)
		str += "W";
	else if (dir.x > margin)
		str += "E";

	m_CurAnimFrameStr = str;
}

void WalkingRenderable::SetAnimFrameStrFromKeys(const bool* keys)
{
	std::string str{ m_CurAnimFrameStr };
	const int keysCount{ Game::GetDirKeysCount(keys) };

	if (!keysCount) {
		str[0] = 'I';
		m_CurAnimFrameStr = str;
		return;
	}
	str.clear();
	str += 'W';

	if (keys[0])
		str += "N";
	else if (keys[1])
		str += "S";
	if (keys[2])
		str += "W";
	else if (keys[3])
		str += "E";

	m_CurAnimFrameStr = str;
}

void WalkingRenderable::SetToNewAnimState(char type)
{
	m_FrameTimer = 0.f;
	m_CurAnimFrameStr[0] = type;
	UpdateFrameBounds();
	UpdateAnimation();
}

// DRAW

void WalkingRenderable::Draw() const {
	m_pTexture->Draw(m_FrameDst, m_FrameSrc);
	if (Game::isDebug) 
	{
		DrawDebug(m_Pos, m_FrameDst);
	}
}

// GETTERS & SETTERS

std::string WalkingRenderable::GetAnimFrameStr() const
{
	return m_CurAnimFrameStr;
}

Rectf WalkingRenderable::GetDstBounds() const
{
	return m_FrameDst;
}

void WalkingRenderable::SetPos(const Vector2f& pos) {
	m_Pos = pos;
}

void WalkingRenderable::AddToPos(const Vector2f& pos) {
	m_Pos += pos;
}

void WalkingRenderable::AddToPos(float x, float y) {
	m_Pos.x += x;
	m_Pos.y += y;
}

Rectf WalkingRenderable::GetFrameDst() const
{
	return m_FrameDst;
}

void WalkingRenderable::Die()
{
	m_IsDead = true;
}

// UTILS

bool WalkingRenderable::IsHalfwayThroughAnimation() const
{
	const int frameIndex{ static_cast<int>(m_FrameTimer * m_Fps) };
	const AnimFrame animFrame{ m_AnimFrames.at(m_CurAnimFrameStr) };

	return frameIndex % (animFrame.nrFrames / 2) == 0;
}

void WalkingRenderable::DrawDebug(const Vector2f& pos, const Rectf& frame) {
	Vector2f start{ pos.x - (frame.width / 4), pos.y };
	Vector2f end{ pos.x + (frame.width / 4), pos.y };

	utils::SetColor(Color4f{ 1.f, 0.f, 0.f, 1.f });
	//utils::DrawLine(start, end, 4.f);

	start.y += 10.f;
	end.y += 10.f;
	utils::DrawLine(pos, Vector2f{ pos.x + 10.f, pos.y }, 4.f);
	utils::DrawLine(pos, Vector2f{ pos.x, pos.y  + 10.f}, 4.f);

	utils::SetColor(Color4f{ 0.f, 0.f, 1.f, 1.f });
	utils::DrawRect(frame, 2.f);
}

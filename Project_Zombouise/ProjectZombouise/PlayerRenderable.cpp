#include "pch.h"
#include "utils.h"
#include "PlayerRenderable.h"
#include "Player.h"
#include <iostream>

// INIT

PlayerRenderable::PlayerRenderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, int dist, int priority, const std::string& str)
	: WalkingRenderable(pos, texturePair, dist, priority, str)
{
	m_FrameSrc = Rectf{ 0.f, 0.f, TEX_SRC_IDLE_W, TEX_SRC_IDLE_H };
	m_FrameDst = Rectf{ 0.f, 0.f, TEX_SRC_IDLE_W, TEX_SRC_IDLE_H };

	InitAnimFrames();
	m_CurAnimFrameStr.reserve(3);
	m_CurAnimFrameStr = "IS";
	UpdateFrameBounds();
}

void PlayerRenderable::Reset(const Vector2i& coord) // delete
{
	WalkingRenderable::Reset(coord);
}

// I : idle		W : walk	P : push	O : observe	   S : stomp
// [ Animation type ] + [ N/S direction ] + [ W/E direction ]

// eg. INE	-> Idle North East
// eg. WW	-> Walk West
void PlayerRenderable::InitAnimFrames()
{
	//Idle
	m_AnimFrames["IN"] = AnimFrame{ 0, 8, 2 };
	m_AnimFrames["INE"] = AnimFrame{ 0, 14, 2 };
	m_AnimFrames["IE"] = AnimFrame{ 0, 12, 2 };
	m_AnimFrames["ISE"] = AnimFrame{ 0, 10, 2 };
	m_AnimFrames["IS"] = AnimFrame{ 0, 0, 2 };
	m_AnimFrames["ISW"] = AnimFrame{ 0, 2, 2 };
	m_AnimFrames["IW"] = AnimFrame{ 0, 4, 2 };
	m_AnimFrames["INW"] = AnimFrame{ 0, 6, 2 };

	//Walk
	m_AnimFrames["WN"] = AnimFrame{ 1, 0, 12 };
	m_AnimFrames["WNE"] = AnimFrame{ 2, 12, 12 };
	m_AnimFrames["WE"] = AnimFrame{ 3, 12, 12 };
	m_AnimFrames["WSE"] = AnimFrame{ 4, 12, 12 };
	m_AnimFrames["WS"] = AnimFrame{ 5, 0, 12 };
	m_AnimFrames["WSW"] = AnimFrame{ 4, 0, 12 };
	m_AnimFrames["WW"] = AnimFrame{ 3, 0, 12 };
	m_AnimFrames["WNW"] = AnimFrame{ 2, 0, 12 };

	//Push
	m_AnimFrames["PN"] = AnimFrame{ 9, 1, 6 };
	m_AnimFrames["PNE"] = AnimFrame{ 8, 10, 7 };
	m_AnimFrames["PE"] = AnimFrame{ 7, 1, 7 };
	m_AnimFrames["PSE"] = AnimFrame{ 6, 1, 8 };
	m_AnimFrames["PS"] = AnimFrame{ 5, 1, 8 };
	m_AnimFrames["PSW"] = AnimFrame{ 6, 9, 8 };
	m_AnimFrames["PW"] = AnimFrame{ 7, 10, 7 };
	m_AnimFrames["PNW"] = AnimFrame{ 8, 1, 7 };

	//Observe
	m_AnimFrames["ON"] = AnimFrame{ 9, 0, 1 };
	m_AnimFrames["ONE"] = AnimFrame{ 8, 17, 1 };
	m_AnimFrames["OE"] = AnimFrame{ 7, 0, 1 };
	m_AnimFrames["OSE"] = AnimFrame{ 6, 0, 1 };
	m_AnimFrames["OS"] = AnimFrame{ 5, 0, 1 };
	m_AnimFrames["OSW"] = AnimFrame{ 6, 17, 1 };
	m_AnimFrames["OW"] = AnimFrame{ 7, 17, 1 };
	m_AnimFrames["ONW"] = AnimFrame{ 8, 0, 1 };

	//Stomp
	m_AnimFrames["SN"] = AnimFrame{ 4, 16, 7 };
	m_AnimFrames["SNE"] = AnimFrame{ 3, 16, 8 };
	m_AnimFrames["SE"] = AnimFrame{ 4, 23, 8 };
	m_AnimFrames["SSE"] = AnimFrame{ 5, 16, 9 };
	m_AnimFrames["SS"] = AnimFrame{ 0, 16, 7 };
	m_AnimFrames["SSW"] = AnimFrame{ 1, 16, 9 };
	m_AnimFrames["SW"] = AnimFrame{ 2, 16, 8 };
	m_AnimFrames["SNW"] = AnimFrame{ 6, 16, 8 };
}

// UPDATE

void PlayerRenderable::UpdateFrameBounds()
{
	SetFpsForFrame();

	m_FrameSrc.width = TEX_SRC_IDLE_W;
	m_FrameSrc.height = TEX_SRC_IDLE_H;

	if (m_CurAnimFrameStr[0] == 'O' || m_CurAnimFrameStr[0] == 'P')
	{
		m_FrameSrc.width = TEX_SRC_PUSH_W;
		m_FrameSrc.height = TEX_SRC_PUSH_H;
	}
	else if (m_CurAnimFrameStr[0] == 'S')
	{
		m_FrameSrc.width = TEX_SRC_STOMP_W;
	}

	m_FrameDst.width = m_FrameSrc.width;
	m_FrameDst.height = m_FrameSrc.height;

	m_FrameSrc.bottom = m_AnimFrames[m_CurAnimFrameStr].row * m_FrameSrc.height;
	if (m_CurAnimFrameStr[0] == 'O' || m_CurAnimFrameStr[0] == 'P')
	{
		m_FrameSrc.bottom -= TEX_SRC_PUSH_OFFSET_Y; // TEX_SRC_PUSH_OFFSET_Y
	}
}

void  PlayerRenderable::SetFpsForFrame()
{
	if (m_CurAnimFrameStr[0] == 'I' || m_CurAnimFrameStr[0] == 'O') {
		m_Fps = 2;
	}
	else if (m_CurAnimFrameStr[0] == 'W') {
		if (m_pOwner->GetState(Player::State::run))
			m_Fps = 16;
		else if (m_pOwner->GetState(Player::State::crouch))
			m_Fps = 8;
		else
			m_Fps = 12;
	}
	else if (m_CurAnimFrameStr[0] == 'P' || m_CurAnimFrameStr[0] == 'C') {
		m_Fps = 10;
	}
	else if (m_CurAnimFrameStr[0] == 'S') {
		m_Fps = 12;
	}
}

void PlayerRenderable::UpdateAnimFromKeys(const bool* keys, const Vector2f& dir)
{
	if (m_pOwner->GetState(Player::State::push) || m_pOwner->GetState(Player::State::stomp))
		return;

	WalkingRenderable::UpdateAnimFromKeys(keys, dir);
}

// DRAW 

void PlayerRenderable::Draw() const
{
	const float w{ 60.f };
	const float h{ 150.f };
	const float percent{ static_cast<float>(m_pOwner->GetHp()) / m_pOwner->GetMaxHp() };
	const Vector2f start{ m_Pos.x - w / 2, m_Pos.y + h };
	const Vector2f end{ start.x + (w * percent), m_Pos.y + h };

	Color4f color{ 0.f, 1.f, 0.f, 1.f };
	if (percent < 0.5f && percent > 0.25f)
	{
		color = Color4f{ 1.f, 1.f, 0.f, 1.f };
	}
	else if (percent <= 0.25f)
	{
		color = Color4f{ 1.f, 0.f, 0.f, 1.f };
	}

	utils::SetColor(color);
	utils::DrawLine(start, end, 5.f);
	WalkingRenderable::Draw();
}

// GETTERS AND SETTERS

void PlayerRenderable::SetOwner(Player* owner)
{
	m_pOwner = owner;
}

// ATTACK 

void PlayerRenderable::Push()
{
	WalkingRenderable::SetToNewAnimState('P');
}

void PlayerRenderable::Idle()
{
	WalkingRenderable::SetToNewAnimState('I');
}

void PlayerRenderable::Stomp()
{
	WalkingRenderable::SetToNewAnimState('S');
}

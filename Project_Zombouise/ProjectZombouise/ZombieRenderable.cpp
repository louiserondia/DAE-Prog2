#include "pch.h"
#include "utils.h"
#include "ZombieRenderable.h"
#include "TextureManager.h"
#include <iostream>

// INIT

ZombieRenderable::ZombieRenderable(const Vector2f& pos, std::pair<Texture*, Rectf> texturePair, int dist, int priority, const std::string& str)
	: WalkingRenderable(pos, texturePair, dist, priority, str)
{
	m_FrameSrc = Rectf{ 0.f, 0.f, 32.f, 64.f };
	m_FrameDst = Rectf{ 0.f, 0.f, 64.f, 128.f };

	InitAnimFrames();
	m_CurAnimFrameStr.reserve(3);
	m_CurAnimFrameStr = "IS";
	UpdateFrameBounds();
	m_pBloodTex = TextureManager::GetInstance()->GetBloodTex();
}

// I: idle		W : walk	F : fall	D : die
// [ Animation type ] + [ N/S direction ] + [ W/E direction ]

// eg. INE	-> Idle North East
// eg. WW	-> Walk West
void ZombieRenderable::InitAnimFrames()
{
	m_AnimFrames["IN"] = AnimFrame{ 0, 0, 2 };
	m_AnimFrames["INE"] = AnimFrame{ 1, 0, 2 };
	m_AnimFrames["IE"] = AnimFrame{ 1, 0, 2 };
	m_AnimFrames["ISE"] = AnimFrame{ 1, 0, 2 };
	m_AnimFrames["IS"] = AnimFrame{ 3, 0, 2 };
	m_AnimFrames["ISW"] = AnimFrame{ 2, 0, 2 };
	m_AnimFrames["IW"] = AnimFrame{ 2, 0, 2 };
	m_AnimFrames["INW"] = AnimFrame{ 2, 0, 2 };

	m_AnimFrames["WN"] = AnimFrame{ 0, 0, 5 };
	m_AnimFrames["WNE"] = AnimFrame{ 1, 0, 8 };
	m_AnimFrames["WE"] = AnimFrame{ 1, 0, 8 };
	m_AnimFrames["WSE"] = AnimFrame{ 1, 0, 8 };
	m_AnimFrames["WS"] = AnimFrame{ 3, 0, 5 };
	m_AnimFrames["WSW"] = AnimFrame{ 2, 0, 8 };
	m_AnimFrames["WW"] = AnimFrame{ 2, 0, 8 };
	m_AnimFrames["WNW"] = AnimFrame{ 2, 0, 8 };

	m_AnimFrames["FN"] = AnimFrame{ 9, 6, 2 };
	m_AnimFrames["FNE"] = AnimFrame{ 9, 0, 2 };
	m_AnimFrames["FE"] = AnimFrame{ 8, 0, 2 };
	m_AnimFrames["FSE"] = AnimFrame{ 10, 0, 2 };
	m_AnimFrames["FS"] = AnimFrame{ 8, 6, 2 };
	m_AnimFrames["FSW"] = AnimFrame{ 10, 3, 2 };
	m_AnimFrames["FW"] = AnimFrame{ 8, 3, 2 };
	m_AnimFrames["FNW"] = AnimFrame{ 9, 3, 2 };

	m_AnimFrames["DN"] = AnimFrame{ 10, 7, 1 };
	m_AnimFrames["DNE"] = AnimFrame{ 9, 2, 1 };
	m_AnimFrames["DE"] = AnimFrame{ 8, 2, 1 };
	m_AnimFrames["DSE"] = AnimFrame{ 10, 2, 1 };
	m_AnimFrames["DS"] = AnimFrame{ 10, 6, 1 };
	m_AnimFrames["DSW"] = AnimFrame{ 10, 5, 1 };
	m_AnimFrames["DW"] = AnimFrame{ 8, 5, 1 };
	m_AnimFrames["DNW"] = AnimFrame{ 9, 5, 1 };
}

// UPDATE

void ZombieRenderable::UpdateFrameBounds()
{
	if (m_CurAnimFrameStr[0] == 'I') {
		m_Fps = 2;
	}
	else if (m_CurAnimFrameStr[0] == 'F') {
		m_Fps = 4;
	}
	else if (m_CurAnimFrameStr[0] == 'W') {
		m_Fps = 6;
	}

	m_FrameSrc.bottom = m_AnimFrames[m_CurAnimFrameStr].row * m_FrameSrc.height;
	m_FrameDst.width = 64.f;
	m_FrameSrc.width = 32.f;
	if (m_HasArmsUp)
	{
		if (m_CurAnimFrameStr != "WN" && m_CurAnimFrameStr != "WS"
			&& m_CurAnimFrameStr != "IN" && m_CurAnimFrameStr != "IS")
		{
			m_FrameSrc.width = 54.f;
			m_FrameDst.width = 108.f;
		}
		m_FrameSrc.bottom = (m_AnimFrames[m_CurAnimFrameStr].row + 4) * m_FrameSrc.height;
	}
	if (m_CurAnimFrameStr[0] == 'F' || m_CurAnimFrameStr[0] == 'D') {
		m_FrameSrc.width = 64.f;
		m_FrameDst.width = 128.f;
	}
}

// DRAW

void ZombieRenderable::Draw() const
{
	if (m_CanBeDisplayed) // add if is out of the screen dans utils et set avec ca aussi p-e
	{
		if (m_IsDead)
		{
			m_pBloodTex->Draw(m_Pos - Vector2f{ m_pBloodTex->GetWidth() / 2, m_pBloodTex->GetHeight() / 2 });
		}
		WalkingRenderable::Draw();
	}
}

// ATTACK

void ZombieRenderable::Fall()
{
	m_HasArmsUp = false;
	WalkingRenderable::SetToNewAnimState('F');
}

void ZombieRenderable::GetUp()
{
	WalkingRenderable::SetToNewAnimState('W');
	m_HasArmsUp = true;
}

void ZombieRenderable::LieDown()
{
	WalkingRenderable::SetToNewAnimState('D');
	//m_HasArmsUp = false;
}

// GETTERS & SETTERS

void ZombieRenderable::SetArmsUp(bool hasArmsUp) {
	m_HasArmsUp = hasArmsUp;
}

void ZombieRenderable::SetCanBeDisplayed(bool canBeDisplayed)
{
	m_CanBeDisplayed = canBeDisplayed;
}

#include "pch.h"
#include "Player.h"
#include "PlayerRenderable.h"
#include "Map.h"
#include "Game.h"
#include "SoundEffect.h"
#include <cmath>

using tex_bound_pair_t = std::pair<Texture*, Rectf>;

bool Player::m_IsSliding{};
bool Player::m_States[Player::m_StatesCount]{};

// INIT

Player::Player() : WalkingEntity()
{
	TextureManager* texManager{ TextureManager::GetInstance() };

	m_Coord = Vector2i{ Map::GetRandFreeTile() };
	const Vector2f pos{ Map::ScreenPosFromCoord(m_Coord) };

	Texture* playerTex{ texManager->GetTexture(TextureManager::TextureType::player) };
	Rectf playerBounds{ texManager->GetBounds(TextureManager::TextureType::player) };
	const tex_bound_pair_t pair{ playerTex, playerBounds };

	PlayerRenderable* PlayerRend{ new PlayerRenderable{ pos, pair, Map::DistFromBottomRight(m_Coord), 2, "kate" } };
	PlayerRend->SetDimensions(pair.first->GetWidth(), pair.first->GetHeight());
	SetRenderable(PlayerRend);
	m_pRenderable->SetOwner(this);
	m_HP = m_MaxHP;
}

void Player::Reset() {

	WalkingEntity::Reset();
	m_pRenderable->Reset(GetCoord());
	m_IsSliding = false;

	m_PrevInputTimer = 0.f;
	m_PrevDir = Vector2f{};
	m_NextDir = Vector2f{};
	m_FovDir = Vector2f{ 0.f, -1.f };
	m_HP = m_MaxHP;

	m_NeedsKeysUpdate = false;
	for (int idx{}; idx < m_StatesCount; ++idx)
	{
		SetState(static_cast<State>(idx), 0);
	}
}

// UPDATE

void Player::Update(float elapsedSec, const Map& map)
{
	if (GetState(State::dead))
	{
		return;
	}

	if (!GetState(State::push) && !GetState(State::stomp) && !m_IsFrozen)
	{
		UpdatePos(elapsedSec, map.GetTilesMap());
	}

	WalkingEntity::UpdateAttackTimer(elapsedSec);
	WalkingEntity::UpdateFrozenTimer(elapsedSec);
	CheckHealth();
	m_Coord = Map::CoordFromScreenPos(m_pRenderable->GetPos());
	m_pRenderable->SetDist(Map::DistFromBottomRight(GetCoord()));
	m_pRenderable->Update(elapsedSec);

	const bool* keys{ Game::GetDirKeysState() };

	CheckKeysNeedUpdate(elapsedSec, keys);
	UpdateFromKeyChange(elapsedSec, keys);

	UpdatePush();
	UpdateStomp();
}


void Player::UpdatePos(float elapsedSec, const tile_map_t& map)
{
	Vector2f pos{ m_pRenderable->GetPos() };
	Vector2f mvt{ m_Dir * elapsedSec };
	mvt = mvt.Normalized();
	mvt *= elapsedSec * m_MaxSpeed * m_Speed;
	const Vector2f prevMvt{ mvt };

	const float length{ mvt.Length() };

	if (length > 0.f)
	{
		mvt = CheckCollision(15, pos, mvt, map);
		pos.y += 10.f;
		mvt = CheckCollision(15, pos, mvt, map);
		m_pRenderable->AddToPos(mvt);
		m_IsSliding = !(mvt == prevMvt || mvt == Vector2f{});
	}
}

void Player::UpdateFromKeyChange(float elapsedSec, const bool* keys)
{

	if (m_NeedsKeysUpdate && CanUpdateKeys() && !GetState(State::push) && !GetState(State::stomp) && !m_IsFrozen)
	{
		m_NeedsKeysUpdate = false;
		UpdateKeys(keys);
		UpdateDir();
		if (m_States[static_cast<int>(State::observe)])
		{
			m_pRenderable->UpdateAnimFromKeys(m_PrevKeys, m_FovDir);
		}
		else
		{
			m_pRenderable->UpdateAnimFromKeys(m_PrevKeys);
		}
		m_pRenderable->UpdateFrameBounds();
	}
}

void Player::UpdateKeys(const bool* keys)
{
	const int keysCount{ Game::GetDirKeysCount(keys) };
	m_PrevKeys[0] = keys[0];
	m_PrevKeys[1] = keys[1];
	m_PrevKeys[2] = keys[2];
	m_PrevKeys[3] = keys[3];
}

void Player::UpdateDir() {
	if (!m_PrevKeys[0] && !m_PrevKeys[1])
	{
		m_Dir.y = 0.f;
	}
	else
	{
		m_Dir.y = m_PrevKeys[0] ? 0.5f : -0.5f;
	}

	if (!m_PrevKeys[2] && !m_PrevKeys[3])
	{
		m_Dir.x = 0.f;
	}
	else
	{
		m_Dir.x = m_PrevKeys[3] ? 1.f : -1.f;
	}

	if (!(m_Dir == Vector2f{ 0.f, 0.f }) && !m_States[static_cast<int>(State::observe)])
	{
		m_FovDir = m_Dir;
	}
}

void Player::UpdateFovWhenObserving(const Vector2f& target)
{
	bool oldPushState{ GetState(State::push) };
	if (target == Vector2f{}) {
		SetState(State::observe, 0);
		if (!(m_Dir == Vector2f{}) && !oldPushState)
		{
			m_FovDir = m_Dir;
		}
		return;
	}

	SetState(State::observe, 1);
	m_NeedsKeysUpdate = true;
	const Vector2f dir{ (target - Camera::VIEWPORT / 2).Normalized() };
	if (!oldPushState)
	{
		m_FovDir = (target - Camera::VIEWPORT / 2).Normalized();
	}
}

// ATTACK 

void Player::UpdatePush()
{
	if (!m_States[static_cast<int>(State::push)])
		return;

	if (m_pRenderable->GetCurAnimFrameType() == 'I')
	{
		SetState(State::idle, 1);
		const bool* keys{ Game::GetDirKeysState() };
		m_pRenderable->UpdateAnimFromKeys(keys);
	}
}

void Player::UpdateStomp()
{
	if (!m_States[static_cast<int>(State::stomp)])
		return;

	if (m_pRenderable->GetCurAnimFrameType() == 'I')
	{
		OnEndStompAnim();
		SetState(State::idle, 1);
		const bool* keys{ Game::GetDirKeysState() };
		m_pRenderable->UpdateAnimFromKeys(keys);
	}
}

void Player::GetDamage(int amount, Zombie* attacker) 
{
	m_HP -= amount;
	TryAttack(attacker); // revenge !!
}

bool Player::CanAttack(const Zombie* enemy) const
{
	if (enemy->GetState() == Zombie::State::lie_down || enemy->GetState() == Zombie::State::dead)
	{
		return false;
	}
	return WalkingEntity::CanAttack(enemy);
}

void Player::TryAttack(Zombie* enemy)
{
	if (CanAnimStomp(enemy))
	{
		m_pRenderable->Stomp();
		SetState(State::stomp, 1);
	}
	else
	{
		if (!GetState(State::push))
		{
			m_pRenderable->Push();
			SetState(State::push, 1);
		}

		if (!CanAttack(enemy))
			return;

		m_AttackCooldown = 0.f;
		m_IsAttacking = true;
		Attack(enemy);
	}
}

void Player::Attack(Zombie* enemy)
{
	Push(enemy);
}

void Player::Push(Zombie* enemy) const
{
	TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::push)->Play(0);
	if (rand() % 3 == 0)
	{
		enemy->Fall();
	}
	else 
	{
		enemy->Pushed(m_FovDir);
	}
}

Rectf Player::GetEnemyHeadHitBox(const Zombie* enemy)
{
	const std::string str{ enemy->GetRenderable()->GetAnimFrameStr() };
	const Rectf bounds{ enemy->GetRenderable()->GetDstBounds() };
	Vector2f pos{ enemy->GetRenderable()->GetPos() };
	float w{ bounds.width };
	float h{ bounds.height / 3 };

	pos.y -= 20.f;

	if (str.find('E') != std::string::npos) // east direction is only half of the width to the left
	{
		pos.x -= bounds.width / 2 + 20.f;
		w *= 0.6f;
	}
	else if (str.find('W') != std::string::npos) // west direction is only half of the width to the right
	{
		pos.x += 20.f;
		w *= 0.6f;
	}
	else // north and south
	{
		if (str[1] == 'N')
			pos.y -= 15.f;
		w *= 0.6f;
	}

	return Rectf{ pos.x, pos.y, w, h };
}

bool Player::CanAnimStomp(Zombie* pZombie)
{
	const float max{ 0.8f };
	if (pZombie->GetState() == Zombie::State::dead)
	{
		return false;
	}
	if (m_AttackCooldown < max)
	{
		return false;
	}
	if (pZombie->GetState() != Zombie::State::lie_down)
	{
		return false;
	}
	if ((GetRenderable()->GetPos() - pZombie->GetRenderable()->GetPos()).Length() > 100.f)
	{
		return false;
	}

	m_pStompEnemy = pZombie;
	return true;
}

void Player::OnEndStompAnim()
{
	const Rectf hitbox{ GetEnemyHeadHitBox(m_pStompEnemy) };
	const Vector2f pos{ GetRenderable()->GetPos() };

	if (pos.x > hitbox.left && pos.x < hitbox.left + hitbox.width
		&& pos.y > hitbox.bottom && pos.y < hitbox.bottom + hitbox.height)
	{
		m_pStompEnemy->Die();
		TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::head_smash)->Play(0);
	}
	else
	{
		TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::push)->Play(0);
	}
}

// GETTERS & SETTERS

WalkingRenderable* Player::GetRenderable() const
{
	return m_pRenderable;
}

void Player::SetRenderable(PlayerRenderable* renderable)
{
	m_pRenderable = renderable;
}

bool Player::GetState(State state) const
{
	return m_States[static_cast<int>(state)];
}

void Player::SetState(State state, bool mode)
{
	switch (state)
	{
	case State::idle:
		if (mode == true)
		{
			m_States[static_cast<int>(State::run)] = 0;
			m_States[static_cast<int>(State::crouch)] = 0;
		}
	case State::walk:
	case State::push:
	case State::stomp:
		if (mode == true)
		{
			m_States[static_cast<int>(State::idle)] = 0;
			m_States[static_cast<int>(State::walk)] = 0;
			m_States[static_cast<int>(State::push)] = 0;
			m_States[static_cast<int>(State::stomp)] = 0;
		}
		break;
	case State::observe:
		break;
	case State::crouch:
		if (mode == true)
		{
			m_Speed = P_CROUCH_SPEED;
		}
		else
		{
			m_Speed = P_WALK_SPEED;
		}
		break;
	case State::run:
		if (mode == true)
		{
			m_Speed = P_RUN_SPEED;
		}
		else
		{
			m_Speed = P_WALK_SPEED;
		}
		break;
	default:
		break;
	}
	m_States[static_cast<int>(state)] = mode;
}

int Player::GetHp() const
{
	return m_HP;
}

int Player::GetMaxHp() const
{
	return m_MaxHP;
}

bool Player::GetIsSliding()
{
	return m_IsSliding;
}

Triangle Player::GetFov() const
{
	const float c{ 0.707f }, s{ 0.707f };
	Vector2f p1{ m_pRenderable->GetPos() };
	p1 -= GetFovDir().Normalized() * 100;
	const Vector2f d{ GetFovDir().Normalized() * 1000 / Camera::GetScale() };
	const Vector2f p2{ p1.x + (d.x * c - d.y * s), p1.y + (d.x * s + d.y * c) };
	const Vector2f p3{ p1.x + (d.x * c + d.y * s), p1.y + (-d.x * s + d.y * c) };
	return Triangle{ p1, p2, p3 };
}

Vector2f Player::GetFovDir() const
{
	return m_FovDir;
}

// UTILS

float Player::DistFrom(const Vector2i& coord) const
{
	return (coord - GetCoord()).Length();
}

void Player::CheckHealth()
{
	if (m_HP <= 0) {
		SetState(State::dead, 1);
	}
}

bool Player::HaveDirKeysChanged(const bool* keys) const
{
	return keys[0] != m_PrevKeys[0]
		|| keys[1] != m_PrevKeys[1]
		|| keys[2] != m_PrevKeys[2]
		|| keys[3] != m_PrevKeys[3];
}

bool Player::CanUpdateKeys() const
{
	const int prevKeysCount{ Game::GetDirKeysCount(m_PrevKeys) };

	return true; // debug, en fait c relou quand c "normal"
	if (!prevKeysCount)
		return true;
	if (prevKeysCount && m_pRenderable->IsHalfwayThroughAnimation())
		return true;
	return false;
}

void Player::CheckKeysNeedUpdate(float elapsedSec, const bool* keys)
{
	if (m_KeysCheckCooldown < 0.3f)
	{
		if (!m_NeedsKeysUpdate && HaveDirKeysChanged(keys))
		{
			m_NeedsKeysUpdate = true;
		}
		m_KeysCheckCooldown += elapsedSec;
	}
	else
	{
		m_KeysCheckCooldown = 0.f;
	}
}

WalkingEntity* Player::FindClosestZombie(const std::vector<Zombie*>& zombies)
{
	int closest{};
	Vector2f prev{ 9999.f, 9999.f };
	const Vector2f pos{ m_pRenderable->GetPos() };

	for (int idx{}; idx < zombies.size(); ++idx)
	{
		if (zombies[idx]->GetState() == Zombie::State::dead || !zombies[idx]->IsInFov())
			continue;
		const Vector2f zombiePos{ zombies[idx]->GetRenderable()->GetPos() };
		Vector2f diff{ zombiePos - pos };

		if (diff.Length() < prev.Length())
		{
			prev = diff;
			closest = idx;
		}
	}
	return zombies[closest];
}

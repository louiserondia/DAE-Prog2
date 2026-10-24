#include "pch.h"
#include "WalkingEntity.h"
#include "Renderable.h"
#include "PlayerRenderable.h"
#include "Game.h"

// INIT

void WalkingEntity::Reset()
{
	m_Dir = Vector2f{};
	m_Coord = Vector2i{ rand() % Map::GetHeight(), rand() % Map::GetWidth() }; //  
	m_Speed = P_WALK_SPEED;
	m_HP = 3;
	m_AttackTimer = 0.f;
	m_IsAttacking = false;
	m_IsFrozen = false;
}

// UPDATE

void WalkingEntity::Update(float elapsedSec, const Map& map) {
	if (!m_IsFrozen || m_IsPushed)
	{
		UpdatePos(elapsedSec, map.GetTilesMap());
	}

	UpdateAttackTimer(elapsedSec);
	UpdateFrozenTimer(elapsedSec);
	UpdatePushTimer(elapsedSec);
}

void WalkingEntity::UpdateAttackTimer(float elapsedSec)
{
	const bool* keys{ Game::GetDirKeysState() };

	m_AttackCooldown += elapsedSec;
	if (m_IsAttacking) {
		m_AttackTimer += elapsedSec;
		if (m_AttackTimer >= m_MaxAttackTimer) {
			m_AttackTimer = 0.f;
			m_AttackCooldown = 0.f;
			m_IsAttacking = false;
			GetRenderable()->UpdateAnimFromKeys(keys);
		}
	}
}

void WalkingEntity::UpdateFrozenTimer(float elapsedSec)
{
	if (!m_IsFrozen)
		return;

	const float max{ 1.f };

	m_FrozenTimer += elapsedSec;
	if (m_FrozenTimer > max)
	{
		m_FrozenTimer = 0.f;
		m_IsFrozen = false;
	}
}

void WalkingEntity::UpdatePushTimer(float elapsedSec)
{
	if (!m_IsPushed)
		return;

	const float max{ 0.6f };

	m_PushTimer += elapsedSec;
	if (m_PushTimer > max)
	{
		m_PushTimer = 0.f;
		m_IsPushed = false;
	}
}

// GETTERS & SETTERS

Vector2i WalkingEntity::GetCoord() const
{
	return m_Coord;
}

Vector2f WalkingEntity::GetDir() const
{
	return m_Dir;
}

void WalkingEntity::SetPushed(const Vector2f& dir)
{
	m_IsPushed = true;
	m_PushTimer = 0.f;
	m_PushedDir = dir;
}

// ATTACK

bool WalkingEntity::CanAttack(const WalkingEntity* enemy) const
{
	const float maxAttackCooldown{ 0.8f };
	const float maxDist{ 30.f };

	if (m_AttackCooldown < maxAttackCooldown)
		return false;

	const float dist{ (GetRenderable()->GetPos() - enemy->GetRenderable()->GetPos()).Length() };
	if (dist > maxDist)
		return false;
	return true;
}

void WalkingEntity::TryAttack(WalkingEntity* enemy)
{
	if (!CanAttack(enemy))
		return;

	m_AttackCooldown = 0.f;
	m_IsAttacking = true;
	Attack(enemy);
}

void WalkingEntity::Attack(WalkingEntity* enemy)
{
	enemy->GetDamage(1, this);
	enemy->Pushed(m_Dir);
}


void WalkingEntity::Pushed(const Vector2f& dir)
{
	m_AttackCooldown = 0.f;
	m_IsFrozen = true;
	m_FrozenTimer = 0.f;
	SetPushed(dir);
}

void WalkingEntity::GetDamage(int amount, WalkingEntity* attacker) {
	m_HP -= amount;
	TryAttack(attacker); // revenge !!
}


// UTILS

Vector2f WalkingEntity::GetSlideMvt(const Vector2f& normal, const Vector2f& mvt)
{
	Vector2f slide{ normal };

	if (mvt.x) slide.x *= -1;
	else slide.y *= -1;
	slide /= 2;
	return slide;
}

bool WalkingEntity::IsCollision(const Vector2f& pos, const Vector2f& mvt, const tile_map_t& map)
{
	if (Wall::IsThereWall(pos, mvt, map)) {
		return true;
	}
	return false;
}

// returns the movement updated if needed (slide or none) in case of collision
// is done twice, see DrawDebug in PlayerRenderable for bounds
Vector2f WalkingEntity::CheckCollision(int precision, Vector2f pos, Vector2f mvt, const tile_map_t& map)
{
	const float start{ pos.x - (GetRenderable()->GetFrameDst().width / 4) };
	const float end{ pos.x + (GetRenderable()->GetFrameDst().width / 4) };
	const float incr{ (end - start) / precision };
	Vector2f p{ start, pos.y };

	while (p.x < end) {
		const bool isCollision{ IsCollision(p, mvt, map) };

		if (isCollision) {
			const Vector2f normal = Wall::WallNormal(p, mvt, map).Normalized();
			const float cross{ mvt.CrossProduct(normal) };

			// -----
			// cross product between normal to the wall and movement, 
			// 0 : same axis -> no movement, else : slides along the wall
			// -----
			if (!utils::IsCloseTo(cross, 0.f)) {
				Vector2f slide{ GetSlideMvt(normal, mvt) };

				if (IsCollision(p, slide, map))
					return mvt * 0;

				mvt = slide;
				p.x = start;
				continue;
			}
			else {
				return mvt * 0;
			}
		}
		p.x += incr;
	}
	return mvt;
}
#include "pch.h"
#include "Vector2i.h"
#include "Zombie.h"
#include "ZombieRenderable.h"
#include "TextureManager.h"
#include "Map.h"
#include "Game.h"
#include "SoundEffect.h"
#include <queue>
#include <unordered_map>
#include <algorithm>

using tex_bound_pair_t = std::pair<Texture*, Rectf>;

int	Zombie::m_DeathCounter{};

// INIT

Zombie::Zombie() :
	WalkingEntity(),
	m_MaxSpeed(static_cast<float>(rand() % 100 + 100)),
	m_Radius(static_cast<float>(rand() % 5 + 5)),
	m_pSfxChaseClose(TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::chase_0)),
	m_pSfxChaseFar(TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::chase_1)),
	m_pSfxWanderClose(TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::wander_0)),
	m_pSfxWanderMid(TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::wander_1)),
	m_pSfxWanderFar(TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::wander_2))
{
	TextureManager* texManager{ TextureManager::GetInstance() };

	m_Coord = Vector2i{ Map::GetRandFreeTile() };
	const Vector2f pos{ Map::ScreenPosFromCoord(m_Coord) };
	Texture* texture{};
	if (rand() & 1)
	{
		texture = texManager->GetTexture(TextureManager::TextureType::zombie);
	}
	else
	{
		texture = texManager->GetTexture(TextureManager::TextureType::zombie2);
	}
	Texture* zombieTex{ texture };
	Rectf zombieBounds{ texManager->GetBounds(TextureManager::TextureType::zombie) };
	const tex_bound_pair_t pair{ zombieTex, zombieBounds };

	m_pRenderable = new ZombieRenderable{ pos, pair, Map::DistFromBottomRight(m_Coord), 2, "zombie" };
	m_pRenderable->SetDimensions(pair.first->GetWidth(), pair.first->GetHeight());
	SetMaxAttackCooldown(1.6f);
	ResetWanderStats();
	m_SoundCooldown = (rand() % 30) / 10.f;
	m_MaxSoundCooldown = (rand() % 10) / 1.f;
}

// UPDATE

void Zombie::Update(float elapsedSec, const Map& map, Player& player)
{
	WalkingEntity::Update(elapsedSec, map);

	switch (m_State)
	{
	case Zombie::State::fall:
		Fall(elapsedSec);
		return;
	case Zombie::State::lie_down:
		LieDown(elapsedSec);
		return;
	case Zombie::State::get_up:
		GetUp(elapsedSec);
		return;
	case Zombie::State::dead:
		return;
	default:
		break;
	}

	const int dist{ static_cast<int>((m_Coord - player.GetCoord()).Length()) };

	m_Coord = Map::CoordFromScreenPos(m_pRenderable->GetPos());
	UpdateFovState(player);
	UpdateRunning(player);
	UpdateWanderChaseStates(elapsedSec, player);
	UpdateDir(player);
	PlaySfx(dist);

	if (!m_IsFrozen || m_IsPushed)
	{
		m_pRenderable->Update(elapsedSec);
		m_pRenderable->UpdateAnimFromDir(m_Dir);
		m_pRenderable->SetDist(Map::DistFromBottomRight(m_Coord));
	}

	TryAttack(&player, elapsedSec);
	m_SoundCooldown += elapsedSec;
}

void Zombie::UpdatePos(float elapsedSec, const tile_map_t& map)
{
	const Vector2f pos{ m_pRenderable->GetPos() };
	const Vector2f start{ pos.x - (m_pRenderable->GetFrameDst().width / 4), pos.y };
	const Vector2f end{ pos.x + (m_pRenderable->GetFrameDst().width / 4), pos.y };
	Vector2f dir{ m_Dir };

	if (m_IsPushed)
	{
		dir = m_PushedDir;
	}
	Vector2f mvt{ elapsedSec * dir.x, elapsedSec * dir.y };
	mvt = mvt.Normalized();
	mvt *= elapsedSec * m_MaxSpeed * m_Speed;

	mvt = CheckCollision(2, pos, mvt, map);

	if (m_State != State::wander || m_WanderStats.cooldown < 0.f)
		m_pRenderable->AddToPos(mvt);
}

void Zombie::UpdateDir(const Player& Player)
{
	if (m_State == State::fall || m_State == State::dead)
		return;

	switch (m_State)
	{
	case Zombie::State::wander:
		UpdateDirInWander();
		break;
	case Zombie::State::chase:
		UpdateDirInChase(Player);
		//general function for enter
		break;
	default:
		break;
	}
}

void Zombie::UpdateDirInWander()
{
	const Vector2i coord{ GetCoord() };

	if (m_WanderStats.cooldown < 0.f) {
		if (coord == m_WanderStats.nextTile) {
			ResetWanderStats();
		}

		const Vector2i coordDir{ m_WanderStats.nextTile - coord };
		m_Dir = Map::ScreenPosFromCoord(coordDir);
	}
	else if (m_WanderStats.cooldown > 0.f) // ne pas le faire tout le temps
	{
		m_Dir = Vector2f{ 0.f, 0.f };
	}
	// suivre la case suivant et attendre cooldown
}

void Zombie::UpdateDirInChase(const Player& Player)
{
	const Vector2i coord{ GetCoord() };

	if (m_IsOnPlayerTile) {
		// tend les bras !
		const Vector2f delta{ Player.GetRenderable()->GetPos() - m_pRenderable->GetPos() };
		const Vector2f dir{ float((delta.x > 0) - (delta.x < 0)), float((delta.y > 0) - (delta.y < 0)) };

		m_Dir.x = abs(delta.x) > 5.f ? dir.x : 0.f;
		m_Dir.y = abs(delta.y) > 5.f ? dir.y : 0.f;
	}
	else {
		if (coord == m_Path.at(m_PathIndex)) {
			m_PathIndex++;
			if (m_PathIndex >= m_Path.size()) {
				m_IsOnPlayerTile = true;
				return;
			}

			const Vector2i coordDir{ m_Path.at(m_PathIndex) - coord };
			m_Dir = Map::ScreenPosFromCoord(coordDir);
		}
	}
}

void Zombie::OnPlayerTileChange()
{
	m_NeedsPathUpdate = true;
}

void Zombie::UpdatePath(const Vector2i& PlayerCoord) {
	m_Path.clear();
	Bfs(PlayerCoord);
	m_IsOnPlayerTile = !m_Path.size();
	m_PathIndex = 0;
}

void Zombie::GetDamage(int amount, WalkingEntity* attacker)
{
	m_BrainAttackTimer = 0.f;
}

void Zombie::ResetWanderStats() {
	int x{ rand() & 1 };
	const bool isNeg{ static_cast<bool>(rand() & 1) };
	int y{ x ? 0 : 1 * (isNeg ? -1 : 1) };
	x *= isNeg ? -2 : 2;

	m_WanderStats.nextTile = GetCoord() + Vector2i{ x, y };
	m_WanderStats.cooldown = (rand() % 50) / 10.f;
}

void Zombie::ToggleState(State state, const Player& Player) {
	m_State = state;
	switch (state)
	{
	case Zombie::State::wander:
		ResetWanderStats();
		break;
	case Zombie::State::chase:
		UpdatePath(Player.GetCoord());
		break;
	case Zombie::State::fall:
		break;
	case Zombie::State::dead:
		break;
	default:
		break;
	}
}

void Zombie::UpdateWanderChaseStates(float elapsedSec, const Player& player)
{
	if (m_State == State::fall || m_State == State::dead)
		return;

	if (IsCloseToPlayer(player, m_Radius) && m_State == State::wander) {
		ToggleState(State::chase, player);
	}
	else if (!IsCloseToPlayer(player, m_Radius) && m_State == State::chase) {
		ToggleState(State::wander, player);
	}
	if (m_State == State::chase && m_NeedsPathUpdate) {
		UpdatePath(player.GetCoord());
		m_NeedsPathUpdate = false;
	}
	if (m_State == State::wander) {
		m_WanderStats.cooldown -= elapsedSec;
	}
}

void Zombie::PlaySfx(int dist)
{
	if (Game::isDebug) {
		m_pSfxChaseClose->SetVolume(0);
		m_pSfxChaseFar->SetVolume(0);
		m_pSfxWanderClose->SetVolume(0);
		m_pSfxWanderMid->SetVolume(0);
		m_pSfxWanderFar->SetVolume(0);
	}
	if (dist > 20) return;

	if (m_SoundCooldown > m_MaxSoundCooldown)
	{
		m_SoundCooldown = 0.f;
		m_MaxSoundCooldown = (rand() % 15 + 5) / 1.f;

		switch (m_State)
		{
		case Zombie::State::wander:
			if (dist > 15)
				m_pSfxWanderFar->Play(0);
			if (dist > 10)
				m_pSfxWanderMid->Play(0);
			else
				m_pSfxWanderClose->Play(0);
			break;
		case Zombie::State::chase:
			m_MaxSoundCooldown /= 2;
			if (dist > 5)
				m_pSfxChaseFar->Play(0);
			else
				m_pSfxChaseClose->Play(0);
			break;
			// fall and die sound when falling and dying
		default:
			break;
		}
	}
}

void Zombie::UpdateFovState(const Player& Player) {
	if (!utils::IsPointInTriangle(m_pRenderable->GetPos(), Player.GetFov()) && !m_IsOutOfFov) {
		m_IsOutOfFov = true;
		m_pRenderable->SetCanBeDisplayed(false);
	}
	else if (utils::IsPointInTriangle(m_pRenderable->GetPos(), Player.GetFov()) && m_IsOutOfFov) {
		m_IsOutOfFov = false;
		m_pRenderable->SetCanBeDisplayed(true);
	}
}

void Zombie::UpdateRunning(const Player& player)
{
	if (m_State != State::wander && m_State != State::chase)
		return;

	if (IsCloseToPlayer(player, m_Radius / 2)) {
		m_Speed = 1.f;
		m_pRenderable->SetArmsUp(true);
	}
	else {
		m_Speed = 0.8f;
		m_pRenderable->SetArmsUp(false);
	}
}

// ATTACK

bool Zombie::CanAttack(const WalkingEntity* enemy) const
{
	if (m_State == State::fall)
		return false;
	return WalkingEntity::CanAttack(enemy);
}

void Zombie::TryAttack(Player *player, float elapsedSec)
{
	// zombie attacks after 1s, slow brain
	// CanAttack in WalkingEntity, overload for zombie if canAttack then think for one more second

	if (WalkingEntity::CanAttack(player)) {
		static const float max{ 1.f };

		m_BrainAttackTimer += elapsedSec;
		if (m_BrainAttackTimer >= max) {
			m_BrainAttackTimer = 0.f;
			if (!CanAttack(player))
				return;

			m_AttackCooldown = 0.f;
			m_IsAttacking = true;
			Attack(player);
			TextureManager::GetInstance()->GetSfx(TextureManager::SfxType::bite)->Play(0);
		}
	}
}

void Zombie::Attack(Player* player)
{
	player->GetDamage(1, this);
	player->Pushed(m_Dir);
}

void Zombie::Pushed(const Vector2f& dir)
{
	m_BrainAttackTimer = 0.f;
	WalkingEntity::Pushed(dir);
}

void Zombie::Fall(float elapsedSec) {
	static const float max{ 0.5f };

	if (m_State != State::fall)
	{
		m_FallTimer = 0.f;
		m_State = State::fall;
		m_pRenderable->Fall();
		m_Dir = Vector2f{ 0.f, 0.f };
		m_BrainAttackTimer = 0.f;
		m_AttackCooldown = 0.f;
	}
	if (m_FallTimer < max && m_State == State::fall)
	{
		m_FallTimer += elapsedSec;
	}
	if (m_FallTimer > max)
	{
		LieDown();
	}
}

void Zombie::LieDown(float elapsedSec)
{
	static const float max{ 5.f };

	if (m_State != State::lie_down)
	{
		m_LieDownTimer = 0.f;
		m_State = State::lie_down;
		m_IsAttacking = false;
		m_pRenderable->LieDown(); // devrait changer de nom quand ça fonctionne bien
		m_IsFrozen = true;
	}
	if (m_LieDownTimer < max && m_State == State::lie_down)
	{
		m_LieDownTimer += elapsedSec;
	}
	if (m_LieDownTimer > max)
	{
		GetUp();
		m_IsFrozen = false;
	}
}

void Zombie::GetUp(float elapsedSec)
{
	static const float max{ 0.5f };

	if (m_State != State::get_up)
	{
		m_GetUpTimer = 0.f;
		m_State = State::get_up;
		m_IsAttacking = false;
		m_pRenderable->Fall();
		m_IsFrozen = true;
	}
	if (m_GetUpTimer < max && m_State == State::get_up)
	{
		m_GetUpTimer += elapsedSec;
	}
	if (m_GetUpTimer > max)
	{
		m_State = State::chase;
		m_pRenderable->GetUp();
		m_IsFrozen = false;
	}
}

void Zombie::Die()
{
	m_State = State::dead;
	m_pRenderable->Die();
	m_DeathCounter++;
}

// GETTERS & SETTERS

WalkingRenderable* Zombie::GetRenderable() const
{
	return m_pRenderable;
}

void Zombie::SetRenderable(ZombieRenderable* renderable)
{
	m_pRenderable = renderable;
}

int Zombie::GetDeathCounter()
{
	return m_DeathCounter;
}

void Zombie::ResetDeathCounter()
{
	m_DeathCounter = 0;
}

// UTILS

bool Zombie::IsCloseToPlayer(const Player& player, float radius) const
{
	const Vector2i delta{ player.GetCoord() - GetCoord() };
	const float length{ delta.Length() };

	if (player.GetState(Player::State::run))
	{
		radius *= 1.25f;
	}
	else if (player.GetState(Player::State::crouch))
	{
		radius *= 0.75f;
	}
	return length < radius;
}

void Zombie::Bfs(const Vector2i& PlayerCoord) {
	std::queue<Vector2i> queue{};
	std::unordered_set<Vector2i> visited{};
	std::unordered_map<Vector2i, Vector2i> parents{};
	Vector2i cur{}, coord{ GetCoord() }, target{ PlayerCoord };
	const Vector2i dirs[8]{
		Vector2i{0, 1}, Vector2i{1, 0}, Vector2i{0, -1}, Vector2i{-1, 0},
		Vector2i{1, 1}, Vector2i{-1, 1}, Vector2i{1, -1}, Vector2i{-1, -1}
	};

	queue.push(GetCoord());

	while (!queue.empty()) {

		cur = queue.front();

		// stop research if the bfs goes further than detection radius (no path)
		//if ((cur - target).Length() > m_Radius)
			//return; 

		if (cur == target) {
			break;
		}

		queue.pop();
		visited.insert(cur);

		for (int idx{}; idx < 8; ++idx) {
			const Vector2i neighbour{ cur + dirs[idx] };

			// count in unordered_set behaves like exists since no duplicates
			// if going to a diagonal tile, checkcheck if neighbours have walls
			if (idx > 3 && (Map::GetBlockedTiles().count(cur + Vector2i{ dirs[idx].x, 0 })
				&& Map::GetBlockedTiles().count(cur + Vector2i{ 0, dirs[idx].y }))) {
				continue;
			}
			if (!Map::GetBlockedTiles().count(neighbour) && !visited.count(neighbour)) {
				visited.insert(neighbour);
				queue.push(neighbour);
				parents[neighbour] = cur;
			}
		}
	}

	std::vector<Vector2i> path{};

	while (!(cur == coord)) {
		path.push_back(cur);
		cur = parents.at(cur);
	}
	path.push_back(cur);
	std::reverse(path.begin(), path.end());
	m_Path = path;
	parents.clear();
	visited.clear();
}

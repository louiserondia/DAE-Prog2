#pragma once

#include "Vector2i.h"
#include "Tile.h"
#include "WalkingEntity.h"
#include <vector>
#include <functional>

class ZombieRenderable;
class Map;
class Player;
class SoundEffect;

using tile_map_t = std::unordered_map <Vector2i, Tile>;

class Zombie final : public WalkingEntity
{
public:
	
	enum class State
	{
		wander,
		chase,
		fall,
		lie_down,
		get_up,
		dead
	};

	explicit Zombie();

	void			Update(float elapsedSec, const Map& map, Player& Player); // overload not override
	virtual			WalkingRenderable* GetRenderable() const override;
	void			SetRenderable(ZombieRenderable* renderable);
	void			OnPlayerTileChange();
	bool			IsInFov() const { return !m_IsOutOfFov; }
	virtual void	GetDamage(int amount, WalkingEntity *attacker) override;
	virtual void	Pushed(const Vector2f &dir) override;
	void			Fall(float elapsedSec = 0.f);
	void			LieDown(float elapsedSec = 0.f);
	void			GetUp(float elapsedSec = 0.f);
	virtual void	Die();
	static int		GetDeathCounter();
	static void		ResetDeathCounter();
	State			GetState() const { return m_State; }

private:


	struct WanderStats
	{
		float cooldown{};
		Vector2i nextTile{};
	};

	virtual void	UpdatePos(float elapsedSec, const tile_map_t& map) override;
	void			UpdateDir(const Player& Player);
	void			UpdateDirInWander();
	void			UpdateDirInChase(const Player& Player);
	void			UpdatePath(const Vector2i& PlayerCoord);
	void			ResetWanderStats();
	void			ToggleState(State state, const Player& Player);
	bool			IsCloseToPlayer(const Player & player, float radius) const;
	void			Bfs(const Vector2i& PlayerCoord);
	void			UpdateFovState(const Player& Player);
	void			UpdateRunning(const Player & player);
	void			UpdateWanderChaseStates(float elapsedSec, const Player & player);
	void			PlaySfx(int dist);

	virtual bool	CanAttack(const WalkingEntity* enemy) const override;

	// not override but overload, specifically takes a Player
	void			TryAttack(Player *player, float elapsedSec);
	void			Attack(Player* player);

	ZombieRenderable*	m_pRenderable{};
	State				m_State{ State::wander };
	WanderStats			m_WanderStats{};

	std::vector<Vector2i>	m_Path{};
	int						m_PathIndex{};
	bool					m_NeedsPathUpdate{};

	bool			m_IsOnPlayerTile{};
	float			m_Radius{ 8 };
	bool			m_IsOutOfFov{};
	float			m_MaxSpeed{ 150.f };

	float			m_BrainAttackTimer{};
	float			m_FallTimer{};
	float			m_LieDownTimer{};
	float			m_GetUpTimer{};
	float			m_SoundCooldown{};
	float			m_MaxSoundCooldown{};

	SoundEffect*	m_pSfxChaseClose;
	SoundEffect*	m_pSfxChaseFar;
	SoundEffect*	m_pSfxWanderClose;
	SoundEffect*	m_pSfxWanderMid;
	SoundEffect*	m_pSfxWanderFar;

	static int		m_DeathCounter;
};

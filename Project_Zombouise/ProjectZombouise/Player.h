#pragma once

#include "utils.h"
#include "structs.h"
#include "Vector2i.h"
#include "Tile.h"
#include <unordered_map>
#include "WalkingEntity.h"
#include "Zombie.h"

class PlayerRenderable;
class Map;

using tile_map_t = std::unordered_map <Vector2i, Tile>;

class Player final : public WalkingEntity
{
public:

	enum class State {
		idle,
		crouch,
		walk,
		run,
		observe,
		push,
		stomp,
		dead,
		count
	};

	static constexpr int m_StatesCount{ static_cast<int>(State::count) };
	static bool m_States[m_StatesCount];

	explicit Player();

	virtual void	Reset() override;
	virtual void	Update(float elapsedSec, const Map& map) override;


	void			CheckKeysNeedUpdate(float elapsedSec, const bool* keys);
	void			UpdateFromKeyChange(float elapsedSec, const bool* keys);

	virtual			WalkingRenderable* GetRenderable() const override;
	void			SetRenderable(PlayerRenderable* renderable);
	void			SetState(State state, bool on);
	State			GetState() const;
	bool			GetState(State state) const;

	int				GetHp() const;
	int				GetMaxHp() const;

	static bool		GetIsSliding();

	Triangle		GetFov() const;
	Vector2f		GetFovDir() const;
	void			UpdateFovWhenObserving(const Vector2f& target = Vector2f{});

	float			DistFrom(const Vector2i& coord) const;
	WalkingEntity*	FindClosestZombie(const std::vector<Zombie*>& zombies);
	void			TryAttack(Zombie* enemy);
	void			GetDamage(int amount, Zombie* attacker);

	Rectf			GetEnemyHeadHitBox(const Zombie* enemy);
	bool			CanAnimStomp(Zombie* enemy);
	void			OnEndStompAnim();

private:

	virtual void	UpdatePos(float elapsedSec, const tile_map_t& map) override;
	void			UpdateDir();
	void			UpdateKeys(const bool* keys);
	bool			CanUpdateKeys() const;
	bool			HaveDirKeysChanged(const bool* keys) const;
	void			CheckHealth();
	void			UpdatePush();
	void			UpdateStomp();

	bool			CanAttack(const Zombie* enemy) const;
	void			Attack(Zombie* enemy);
	void			Push(Zombie* enemy) const;

	PlayerRenderable* m_pRenderable;

	float		m_PrevInputTimer{};
	Vector2f	m_PrevDir{};
	Vector2f	m_NextDir{};
	Vector2f	m_FovDir{ 0.f, -1.f };

	const int	m_MaxHP{ 8 };
	static bool	m_IsSliding;
	bool		m_NeedsKeysUpdate{};
	bool		m_PrevKeys[4]{};
	float		m_KeysCheckCooldown{};
	State		m_State{};
	Zombie*		m_pStompEnemy{};

};



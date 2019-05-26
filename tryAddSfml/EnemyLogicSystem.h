#pragma once
#include "ECS.h"
#include "Components.h"
#include "Events.h"
#include <tuple>
#include "Level1Map.h"
using namespace ECS;
class EnemyLogicSystem : public EntitySystem {
	virtual ~EnemyLogicSystem() {}

	virtual void tick(World* world, float dt) {
		world->each<Enemy, Weapon>([&](Entity* ent, ComponentHandle<Enemy> enemy,
			ComponentHandle<Weapon> weapon) -> void {
			weapon->direction = Vector(rotateVector(dt/100, weapon->direction)).toUnitVector().direction;
			weapon->angle = angleBetweenVectors(Coordinate(1, 0), weapon->direction) - 90;
			if (weapon->direction.y < 0) {
				weapon->angle *= -1;
				weapon->angle += 180;
			}
			world->emit<WantedShoot>(ent);
		});
	}
};
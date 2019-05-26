#pragma once
#include "ECS.h"
#include "Components.h"
#include "Events.h"
#include <tuple>
#include "Level1Map.h"
using namespace ECS;
class CollisionSystem : public EntitySystem, public EventSubscriber<CheckBulletCollision> {
	virtual ~CollisionSystem() {}

	virtual void configure(World *world) override {
		world->subscribe<CheckBulletCollision>(this);
	}

	virtual void receive(World* world, const CheckBulletCollision& event) override
	{
		Entity *ent = checkBulletCollision(world, event.bullet);
		if (ent) {
			world->emit<isBulletCollided>(isBulletCollided(event.bullet, ent));
			return;
		}
		ent = checkForCollison(event.bullet->sprite, TYPE_COLLISION::DESTRUCTIBLE, world);
		if(ent){
			world->emit<isBulletCollided>(isBulletCollided(event.bullet, ent));
			return;
		}
	}

public:
	static Entity* checkForCollison(sf::Sprite sprite, TYPE_COLLISION type, World *world) {
		Entity* collidableEntity = nullptr;
		world->each<Collision, Render>([&](Entity* entity, ComponentHandle<Collision>collision, ComponentHandle<Render>render) {
			if (sprite.getGlobalBounds().intersects(render->sprite.getGlobalBounds())) {
				collidableEntity = entity;
			}
		});
		return collidableEntity;
	}

	Entity* checkBulletCollision(World* world, Bullet* bullet) {
		Entity* entity = nullptr;
		if (bullet->isEnemyBullet) {
			world->each<Player, Render>([&](Entity* ent, ComponentHandle<Player>player, ComponentHandle<Render>render){
				if (bullet->sprite.getGlobalBounds().intersects(render->sprite.getGlobalBounds())) {
					entity = ent;
				}
			});
		}
		else {
			world->each<Enemy, Render>([&](Entity* ent, ComponentHandle<Enemy>enemy, ComponentHandle<Render>render) {
				if (bullet->sprite.getGlobalBounds().intersects(render->sprite.getGlobalBounds())) {
					entity = ent;
				}
			});
		}
		return entity;
	}
};
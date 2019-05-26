#pragma once
#include "ECS.h"
#include "Components.h"
#include "Events.h"
#include <iostream>
using namespace ECS;
class HealthSystem : public EntitySystem, public EventSubscriber<isBulletCollided> {
	virtual ~HealthSystem() {}

	virtual void configure(World* world) override {
		world->subscribe<isBulletCollided>(this);
	}

	virtual void receive(World* world, const isBulletCollided& event) override {
		decreaseHealth(world, event.entity, event.bullet->damage);
	}

	void decreaseHealth(World* world, Entity* entity, int damage) {
		entity->with<Health>([&](ComponentHandle<Health>health){
			health->currentHealth -= damage;
			if (health->currentHealth <= 0) {
				world->emit<EntityDestroyed>(EntityDestroyed(entity));
				destroyEntity(world, entity);
			}
		});
	}
	void destroyEntity(World* world, Entity* entity){
		world->destroy(entity);
	}

};
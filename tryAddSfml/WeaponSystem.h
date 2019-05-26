#pragma once
#include "ECS.h"
#include "Components.h"
#include "Events.h"
#include <algorithm>
using namespace ECS;
class WeaponSystem : public EntitySystem,
	public EventSubscriber<isBulletCollided>,
	public EventSubscriber<MousePressed>,
	public EventSubscriber<WantedShoot>,
	public EventSubscriber<PositionChanged>{
	virtual ~WeaponSystem() {}
	virtual void configure(class World* world) override
	{
		world->subscribe<MousePressed>(this);
		world->subscribe<PositionChanged>(this);
		world->subscribe<isBulletCollided>(this);
		world->subscribe<WantedShoot>(this);
		
	}

	virtual void receive(World* world, const PositionChanged& event) override
	{
		event.entity->with<Weapon, Render>([&](ComponentHandle<Weapon>weapon, ComponentHandle<Render>render) {
			weapon->position.coordinate.x = event.position.x + render->sprite.getGlobalBounds().width / 2;
			weapon->position.coordinate.y = event.position.y + render->sprite.getGlobalBounds().height / 2;
		});
	}

	virtual void receive(World* world, const MousePressed& event) override
	{
		shoot(event.entity);
	}

	virtual void receive(World* world, const WantedShoot& event) override
	{
		shoot(event.entity);
	}

	virtual void receive(World* world, const isBulletCollided& event) override
	{
		//i know, that const_cast is bad, but in this case i should to mark that bullet isCollided
		Bullet* bullet = const_cast<Bullet *>(event.bullet);
		bullet->isCollided = true;
	}

	void shoot(Entity* entity) {
		entity->with<Weapon,Bullets>([&](ComponentHandle<Weapon>weapon, ComponentHandle<Bullets>bullets){
			if(bullets->type == weapon->typeBullet){
				if (weapon->timeLastShot > weapon->reloadingTime && bullets->currentSize != 0) {
					weapon->timeLastShot = 0;
					Bullet* bullet = bullets->bullets.back();
					bullets->bullets.pop_back();

					bullet->reset();

					float deltaX = weapon->direction.x*weapon->sprite.getLocalBounds().height - weapon->sprite.getLocalBounds().width/2;
					float deltaY = weapon->direction.y*weapon->sprite.getLocalBounds().height - weapon->sprite.getLocalBounds().width/2;
				
					bullet->position.x = weapon->position.coordinate.x + deltaX;
					bullet->position.y = weapon->position.coordinate.y + deltaY;
					bullet->direction = weapon->direction;
				
					bullets->activeBullets.push_back(bullet);
					bullets->currentSize--;
					if(entity->has<Player>()){
						std::cout << "Player has bullets:" << bullets->currentSize << '\n';
					}
				}
			}
		});
	}
	virtual void tick(World* world, float dt) {
		world->each<Weapon, Bullets>([&](Entity* ent, ComponentHandle<Weapon> weapon, ComponentHandle<Bullets>bullets) -> void {
			if(bullets->type == weapon->typeBullet){
				weapon->timeLastShot += dt;
				if(!bullets->bullets.empty()){

					//--------------------------------
					copy_if(bullets->activeBullets.begin(), bullets->activeBullets.end(),
					std::back_inserter(bullets->bullets),[&](Bullet *b){
						return b->way > b->maxDistance;
					});
					bullets->activeBullets.remove_if([&](const Bullet* bullet){ return bullet->way > bullet->maxDistance;});
				
					for (Bullet* bullet : bullets->activeBullets) {
						bullet->way += dt*bullet->speed;
						bullet->position = bullet->position + dt*bullet->speed*Vector(bullet->direction);
						bullet->sprite.setPosition(bullet->position.x, bullet->position.y);
						world->emit<CheckBulletCollision>(CheckBulletCollision(bullet));
						window.draw(bullet->sprite);
					}

					copy_if(bullets->activeBullets.begin(), bullets->activeBullets.end(),
						std::back_inserter(bullets->bullets), [&](Bullet *b) {
						return b->isCollided;
					});
					bullets->activeBullets.remove_if([&](Bullet* bullet) {
						if (bullet->isCollided) {
							bullet->isCollided = false;
							return true;
						}
						return false;
					});
				}
			}
		});
	}

};
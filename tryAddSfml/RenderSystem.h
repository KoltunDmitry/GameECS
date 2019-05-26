#pragma once
#include "Components.h"
#include "ECS.h"
#include "Events.h"
#include <iostream>
#include "Level1Map.h"
using namespace ECS;

//sf::RenderWindow window(sf::VideoMode(1280, 720), "Game");

class RenderSystem : public EntitySystem,
	public EventSubscriber<EntityDestroyed>{

	virtual void configure(class World* world) override
	{
		world->subscribe<EntityDestroyed>(this);
	}


	virtual void receive(World* world, const EntityDestroyed& event) override
	{
		event.entity->with<Render>([&](ComponentHandle<Render>render) {
			render->isVisible = false;
		});
	}

	virtual ~RenderSystem() {}

	virtual void tick(World* world, float dt) {
		
		//draw each weapon
		world->each<Render, Weapon>([&](Entity* ent, ComponentHandle<Render>render, ComponentHandle<Weapon>weapon){
			float x = weapon->position.coordinate.x;
			float y = weapon->position.coordinate.y;
			weapon->sprite.setPosition(x,y);
			weapon->sprite.setRotation(weapon->angle);
			window.draw(weapon->sprite);
		});

		world->each<Render, Position>([&](Entity* entity, ComponentHandle<Render>render,
			ComponentHandle<Position>position){
			if(render->isVisible){
				render->sprite.setPosition(position->coordinate.x, position->coordinate.y);
				if(entity->has<UserInput>()){
					int x = position->coordinate.x;
					int y = position->coordinate.y;
					sf::Vector2u windowSize = window.getSize();
					if (windowSize.x / 2 > x) {
						x = windowSize.x / 2;
					}
					if (windowSize.y / 2 > y) {
						y = windowSize.y/2;
					}
					if (windowSize.y/2 + y > (WIDTH_MAP+1)*HEIGHT_TILE) {
						y = (WIDTH_MAP+1) * HEIGHT_TILE - windowSize.y / 2;
					}
					if (windowSize.x / 2 + x > HEIGHT_MAP * WIDTH_TILE) {
						x = HEIGHT_MAP * WIDTH_TILE - windowSize.x/2;
					}
					view.setCenter(x, y);
				}
				window.draw(render->sprite);
			}
		});
	}
};

#pragma once
#include "Events.h"
#include <iostream>
#include <vector>
#include "CollisionSystem.h"
#include "Common.h"
using namespace ECS;
class MovementSystem : public EntitySystem,
	public EventSubscriber<KeyboardEvent>{
	
	virtual ~MovementSystem() {}

	virtual void configure(World *world) override {
		world->subscribe<KeyboardEvent>(this);
	}

	virtual void receive(World* world, const KeyboardEvent& event) override
	{
		event.entity->with<Position, Movement, Render>([&](ComponentHandle<Position> position, ComponentHandle<Movement> movement, ComponentHandle<Render> render) -> void {
			bool isChangedPosition = false;
			float cos45 = sqrt(2) / 2.0;
			Coordinate oldPosition = position->coordinate;
			std::vector<bool>keyboarding(event.pressed);
			sf::Vector2i direction = detectDirection(keyboarding);
			if (direction.x == 0 && direction.y == 0) {
				isChangedPosition = false;
			}
			else {

				if (direction.x != 0 && direction.y != 0) {
					position->coordinate.x += cos45 * direction.x*movement->velocity*event.dt;
					position->coordinate.y += cos45 * direction.y*movement->velocity*event.dt;
				}
				else if (direction.x != 0) {
					position->coordinate.x += direction.x*movement->velocity*event.dt;
				}
				else {
					position->coordinate.y += direction.y*movement->velocity*event.dt;
				}
				int width = render->sprite.getTextureRect().width;
				int height = render->sprite.getTextureRect().height;

				render->sprite.setPosition(position->coordinate.x, position->coordinate.y);
				if (!CollisionSystem::checkForCollison(render->sprite, TYPE_COLLISION::NON_DESTRUCTIBLE, world)) {
					isChangedPosition = true;
				}
				else {
					position->coordinate = oldPosition;
				}
				if (isChangedPosition) {
					world->emit<PositionChanged>({ position->coordinate, event.entity });
				}
			}
		});
	}
};
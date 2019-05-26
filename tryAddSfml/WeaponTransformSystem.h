#pragma once
#include "ECS.h"
#include "Events.h"
#include "Components.h"
using namespace ECS;
class WeaponTransformSystem : public EntitySystem {
	virtual ~WeaponTransformSystem() {}

	virtual void tick(World* world, float dt) {
		world->each<UserInput, Weapon>([&](Entity* ent,
			ComponentHandle<UserInput> userInput, ComponentHandle<Weapon> weapon) -> void {
			if (window.hasFocus()) {
				sf::Vector2i relativePositionMouse = userInput->mousePosition;
				
				if(relativePositionMouse.x > 0 && relativePositionMouse.y > 0){
					sf::Vector2f center = view.getCenter();
					sf::Vector2u windowSize = window.getSize();

					Coordinate positionMouse = Coordinate(center.x - windowSize.x/2 + relativePositionMouse.x, center.y - windowSize.y/2 + relativePositionMouse.y);

					Vector direction = Vector::calculateVector(weapon->position.coordinate, positionMouse).toUnitVector();
					weapon->direction = direction.direction;
					weapon->angle = angleBetweenVectors(Coordinate(1, 0), direction.direction) - 90;
					if (direction.direction.y < 0) {
						weapon->angle *= -1;
						weapon->angle += 180;
					}
				}
			}
		});
	}

};
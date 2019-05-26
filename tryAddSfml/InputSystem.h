#pragma once
#include "ECS.h"
#include "Components.h"
#include "Events.h"
#include <iostream>
#include <vector>
#include "CollisionSystem.h"
using namespace ECS;
class InputSystem : public EntitySystem {
	virtual ~InputSystem() {}

	virtual void tick(World* world, float dt) {
		world->each<UserInput>([&](Entity* ent, ComponentHandle<UserInput> userInput) -> void {
			std::vector<bool> keyboarding(4,false);
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left) || sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
				keyboarding[0] = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right) || sf::Keyboard::isKeyPressed(sf::Keyboard::D))
			{
				keyboarding[1] = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up) || sf::Keyboard::isKeyPressed(sf::Keyboard::W)) {
				keyboarding[2] = true;
			}
			if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down) || sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
				keyboarding[3] = true;
			}
			world->emit<KeyboardEvent>(KeyboardEvent(keyboarding, ent, dt));
			userInput->mousePosition = sf::Mouse::getPosition(window);
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
			{
				world->emit<MousePressed>({Coordinate(userInput->mousePosition.x, userInput->mousePosition.y), ent});
			}

		});
	}
};
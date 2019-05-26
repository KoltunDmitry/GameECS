#pragma once
#include <SFML/Graphics.hpp>
#include "ECS.h"

#include <vector>


using namespace ECS;

class GameEngine
{
public:
	//here will be init
	GameEngine();
	
	void init();
	void run();
	void drawMap(bool);

	/*sf::RenderWindow& getWindow() {
		return m_window;
	}*/

	~GameEngine();
private:
	//sf::RenderWindow m_window;
	World* world;
	//std::vector<Entity*>entities;
};


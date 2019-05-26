#include "GameEngine.h"
#include "Components.h" 
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "SFML/Graphics/Texture.hpp"

#include "RenderSystem.h"
#include "InputSystem.h"
#include "MovementSystem.h"
#include "WeaponTransformSystem.h"
#include "WeaponSystem.h"
#include "CollisionSystem.h"
#include "HealthSystem.h"
#include "EventSystem.h"
#include "EnemyLogicSystem.h"

#include "Level1Map.h"


GameEngine::GameEngine()
{
	world = World::createWorld();
	init();
}

void GameEngine::init() {

	drawMap(true);

	Entity* player = world->create();

	sf::Texture playerTexture;
	sf::Sprite playerSprite;
	playerSprite.setTexture(playerTexture);
	playerSprite.setColor(sf::Color::Green);
	playerSprite.setTextureRect(sf::IntRect(0, 0, 20, 20));
	playerSprite.setPosition(50, 25);

	
	player->assign<Position>(Coordinate(50, 50));
	player->assign<Render>(playerSprite, true);
	player->assign<Movement>(2);
	player->assign<UserInput>();
	player->assign<Bullets>(Bullets(200, 250, "B2", false));
	player->assign<Weapon>(createWeapon("W2"));
	player->assign<Health>(Health(100, 100));
	player->assign<Player>();

	world->registerSystem(new InputSystem());
	world->registerSystem(new MovementSystem());
	world->registerSystem(new CollisionSystem());
	world->registerSystem(new WeaponTransformSystem());
	world->registerSystem(new WeaponSystem());
	world->registerSystem(new RenderSystem());
	world->registerSystem(new HealthSystem());
	world->registerSystem(new EventSystem());
	world->registerSystem(new EnemyLogicSystem());
	//emit signal
	world->emit<LevelInitialized>(LevelInitialized());
}

void GameEngine::drawMap(bool isFirst) {

	sf::Texture wall;
	sf::Texture road;
	sf::Texture let;
	sf::Texture box;

	sf::Sprite wallSprite;
	wallSprite.setTexture(wall);
	wallSprite.setColor(sf::Color::Magenta);
	wallSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	wallSprite.setPosition(0, 0);

	sf::Sprite roadSprite;
	roadSprite.setTexture(road);
	roadSprite.setColor(sf::Color::Black);
	roadSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	roadSprite.setPosition(0, 0);

	sf::Sprite letSprite;
	letSprite.setTexture(let);
	letSprite.setColor(sf::Color::Yellow);
	letSprite.setTextureRect(sf::IntRect(0, 0, 32, 32));
	letSprite.setPosition(0, 0);

	int widthTile = 32;
	int heightTile = 32;

	for (int i = 0; i < HEIGHT_MAP; i++) {
		for (int j = 0; j < WIDTH_MAP; j++) {
				if (TileMap[i][j] == '0' && isFirst) {
					Entity* wall = world->create();
					wall->assign<Render>(wallSprite, true);
					wall->assign<Position>(Coordinate(widthTile*i, heightTile*j));
					wall->assign<Collision>((TYPE_COLLISION::NON_DESTRUCTIBLE));
				}
				else if (TileMap[i][j] == ' ') {
					roadSprite.setPosition(widthTile*i, heightTile*j);
					window.draw(roadSprite);
				}
				else if(TileMap[i][j] == 's' && isFirst){
					Entity* box = world->create();
					box->assign<Render>(letSprite, true);
					box->assign<Position>(Coordinate(widthTile*i, heightTile*j));
					box->assign<Collision>((TYPE_COLLISION::DESTRUCTIBLE));
					box->assign<Health>(Health(1, 1));
				}
				else if (TileMap[i][j] == 'E' && isFirst) {
					Entity* enemy = world->create();
					sf::Texture enemyTexture;
					sf::Sprite enemySprite;
					enemySprite.setTexture(enemyTexture);
					enemySprite.setColor(sf::Color::Cyan);
					enemySprite.setTextureRect(sf::IntRect(0, 0, 32, 32));

					enemy->assign<Bullets>(Bullets(10000, 10000, "B4", true));
					enemy->assign<Position>(Coordinate(widthTile*i, heightTile*j));
					enemy->assign<Render>(Render(enemySprite, true));
					enemy->assign<Weapon>(createWeapon("W4"));
					enemy->assign<Health>(Health(100, 100));
					enemy->assign<Enemy>(Enemy());
				}
		}
	}

}

void GameEngine::run() {
	sf::Clock clock; 
	while (window.isOpen())
	{
		float time = clock.getElapsedTime().asMicroseconds();
		clock.restart();
		time = time / 5000.0;
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		window.clear();
		drawMap(false);
		world->tick(time);
		window.setView(view);
		window.display();
		//isFirst = false;
	}
}

GameEngine::~GameEngine()
{
}

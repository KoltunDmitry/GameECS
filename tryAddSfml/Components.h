#pragma once
#include "Common.h"
#include "SFML/Graphics.hpp"
#include "SFML/Graphics/Sprite.hpp"
#include "InitializeWindow.h"
//#include "Events.h"
#include <list>
#include <vector>

typedef int COLLISION_TYPE;

enum TYPE_COLLISION { DESTRUCTIBLE = 0, NON_DESTRUCTIBLE, PLAYER, ENEMY, PLAYER_BULLET, ENEMY_BULLET };


struct Position
{
	Position() :coordinate(0, 0) {}
	Position(Coordinate coordinate) :coordinate(coordinate) {};


	Coordinate coordinate;
};

struct Movement
{
	Movement():velocity(0) {}
	Movement(float velocity) : velocity(velocity) {};

	double velocity;
};

struct Health {
	Health(uint64_t currentHealth, uint64_t maxHealth): currentHealth(currentHealth), maxHealth(maxHealth){};
	int currentHealth;
	int maxHealth;
};

struct Transform {
	Transform():angle(0){}
	Transform(float angle): angle(angle){}
	float angle;
};

struct UserInput {
	UserInput(){}
	sf::Vector2i mousePosition;
};

struct Render {
	//Render() {}
	Render(sf::Sprite sprite, bool isVisible) :sprite(sprite), isVisible(isVisible){};
	sf::Sprite sprite;
	float rotation = -1;
	bool isRotated = false;
	bool isVisible = false;
};

struct Bullet {
	Bullet(sf::Sprite sprite, bool isEnemy, int damage = 5, float speed = 7, int maxDistance = 1000): sprite(sprite),damage(damage), speed(speed), way(0), maxDistance(maxDistance), isCollided(false), isEnemyBullet(isEnemy)  {
		/*sf::Texture bulletTexture;
		sf::Sprite bulletSprite;
		bulletSprite.setTexture(bulletTexture);
		bulletSprite.setColor(sf::Color::White);
		bulletSprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
		bulletSprite.setPosition(0, 0);*/

		//this->bulletSprite = bulletSprite;
	}
	void reset() {
		way = 0;
	}
	float maxDistance;
	sf::Sprite sprite;
	int damage;
	float speed;
	Coordinate position;
	Coordinate direction;
	float way;
	bool isCollided;
	bool isEnemyBullet;
};

Bullet* createBullet(std::string name, bool isEnemyBullet) {
	sf::Texture bulletTexture;
	sf::Sprite bulletSprite;
	bulletSprite.setTexture(bulletTexture);
	bulletSprite.setColor(sf::Color::White);
	bulletSprite.setTextureRect(sf::IntRect(0, 0, 4, 4));
	bulletSprite.setPosition(0, 0);
	if (name == "B1") {
		return new Bullet(bulletSprite, isEnemyBullet, 5, 4);
	}
	else if (name == "B2") {
		bulletSprite.setColor(sf::Color::Blue);
		return new Bullet(bulletSprite, isEnemyBullet, 10, 3, 500);
	}
	else if (name == "B3") {
		bulletSprite.setColor(sf::Color::Magenta);
		return new Bullet(bulletSprite, isEnemyBullet, 7, 3.5, 700);
	}
	else if (name == "B4") {
		bulletSprite.setColor(sf::Color::Red);
		return new Bullet(bulletSprite, isEnemyBullet, 50, 5, 1000);
	}
	return nullptr;
}


struct Bullets {
	Bullets(size_t size, size_t maxSize, std::string typeBullet, bool isEnemyBullet) :maxSize(maxSize), type(typeBullet), currentSize(size) {
		for (size_t i = 0; i < currentSize; i++) {
			bullets.push_back(createBullet(typeBullet,isEnemyBullet));
		}
	}
	size_t currentSize;
	size_t maxSize;
	std::string type;
	std::list<Bullet*>bullets;
	std::list<Bullet*>activeBullets;
};


struct Weapon : public Transform {
	Weapon(sf::Sprite sprite, float reloadingTime = 10, std::string type = "", std::string typeBullet = "") : sprite(sprite), reloadingTime(reloadingTime), timeLastShot(0), position(Coordinate(-1000000, -100000)), type(type), typeBullet(typeBullet) {
		int a = rand()%180 - 90;
		int b = rand()&180 - 90;
		direction = Coordinate(a,b);
	}
	float timeLastShot = 1000;
	float reloadingTime;
	std::string type;
	std::string typeBullet;
	sf::Sprite sprite;
	Position position;
	Coordinate direction;

};


Weapon createWeapon(std::string name) {
	sf::Texture weaponTexture;
	sf::Sprite weaponSprite;
	weaponSprite.setTexture(weaponTexture);
	weaponSprite.setColor(sf::Color::Red);
	weaponSprite.setTextureRect(sf::IntRect(0, 0, 5, 30));
	weaponSprite.setPosition(50, 25);
	weaponSprite.setOrigin(weaponSprite.getTextureRect().width / 2.0, 0);

	sf::Texture enemyWeaponTexture;
	sf::Sprite enemyWeaponSprite;

	enemyWeaponSprite.setTexture(enemyWeaponTexture);
	enemyWeaponSprite.setColor(sf::Color::Red);
	enemyWeaponSprite.setTextureRect(sf::IntRect(0, 0, 8, 50));
	enemyWeaponSprite.setPosition(50, 25);
	enemyWeaponSprite.setOrigin(weaponSprite.getTextureRect().width / 2.0, 0);


	if (name == "W1") {
		return Weapon(weaponSprite,75,name,"B3");
	}
	else if (name == "W2") {
		weaponSprite.setColor(sf::Color::Yellow);
		return Weapon(weaponSprite, 30, name, "B2");
	}
	else if (name == "W3") {
		weaponSprite.setColor(sf::Color::White);
		return Weapon(weaponSprite, 50, name, "B1");
	}
	else if (name == "W4") {
		return Weapon(enemyWeaponSprite, 30, name, "B4");
	}
	return Weapon(enemyWeaponSprite);
}


struct ManagedLogic {
	ManagedLogic(const Vector& vectorDirection) :vectorDirection(vectorDirection) {};
	Vector vectorDirection;
	bool isShot = false;
	Coordinate lastPosition;
	double way = 0;
};

struct Collision {
	Collision(TYPE_COLLISION type) {
		this->type = type;
	}
	TYPE_COLLISION type;
};

struct Enemy{};

struct Player{};

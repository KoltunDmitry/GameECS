#pragma once
#include "ECS.h"
#include "Components.h"

struct MousePressed {
	Coordinate coordinate;
	Entity* entity;
};

struct PositionChanged {
	PositionChanged(Coordinate position, Entity* entity): position(position), entity(entity){};
	Coordinate position;
	Entity* entity;
};

struct Damage {
	int health;
	Entity* entity;
};

struct EntityDestroyed {
	EntityDestroyed(Entity* entity): entity(entity){}
	Entity* entity;
};

struct CheckBulletCollision {
	CheckBulletCollision(Bullet *bullet):bullet(bullet){}
	Bullet* bullet;
};

struct isBulletCollided {
	isBulletCollided(Bullet* bullet, Entity* entity): bullet(bullet), entity(entity){}
	Bullet* bullet;
	Entity* entity;
};

struct KeyboardEvent{
	KeyboardEvent(std::vector<bool>&pressed, Entity* entity, float dt):pressed(pressed), entity(entity), dt(dt){}
	std::vector<bool> pressed;
	Entity* entity;
	float dt;
};

struct LevelInitialized {

};

struct WantedShoot {
	WantedShoot(Entity* entity): entity(entity){}
	Entity* entity;
};
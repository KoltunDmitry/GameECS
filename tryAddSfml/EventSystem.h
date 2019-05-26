#pragma once
#include "ECS.h"
#include "Components.h"
#include "Events.h"
#include <tuple>
#include "Level1Map.h"
using namespace ECS;
class EventSystem : public EntitySystem, public EventSubscriber<LevelInitialized>{
	virtual ~EventSystem() {}

	virtual void configure(World *world) override {
		world->subscribe<LevelInitialized>(this);

	}

	virtual void receive(World* world, const LevelInitialized& event) override
	{
		world->each<Position>([&](Entity* entity, ComponentHandle<Position>position){
			world->emit<PositionChanged>(PositionChanged(Coordinate(position->coordinate), entity));
		});
	}


};
#pragma once

#include "./Racer.h"
#include "../main/Game.h"

class Player : public Racer
{
public:
    Player(class Game* game, int clientID);

    void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;
private:
};
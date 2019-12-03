#pragma once

#include "./Actor.h"

class Racer : public Actor
{
public:
    Racer(class Game* game, int clientID);

    void UpdateActor(float deltaTime) override;
	void ActorInput(const uint8_t* keyState) override;

    // getter
    Vector2 GetPosition() const { return Actor::GetPosition(); }
    float GetRotation() const { return Actor::GetRotation(); }
    // setter
    void SetPosition(Vector2 pos) { Actor::SetPosition(pos); }
    void SetRotation(float rot) { Actor::SetRotation(rot); }

private:
};

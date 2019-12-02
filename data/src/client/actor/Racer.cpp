#include "./Racer.h"
#include "../component/SpriteComponent.h"
#include "../component/InputComponent.h"
#include "../main/Game.h"
#include "../main/Client_window.h"

Racer::Racer(Game *game)
	: Actor(game)
{
	// Create a sprite component
	SpriteComponent *sc = new SpriteComponent(this, 50);
	sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/Player_red.png"));

	mPos.x = 270;
	mPos.y = 270;
	SetPosition(mPos);

	// Create an input component and set keys/speed
	// InputComponent* ic = new InputComponent(this);
	// ic->SetForwardKey(SDL_SCANCODE_W);
	// ic->SetBackKey(SDL_SCANCODE_S);
	// ic->SetClockwiseKey(SDL_SCANCODE_A);
	// ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	// ic->SetMaxForwardSpeed(300.0f);
	// ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Racer::UpdateActor(float deltaTime)
{
}

void Racer::ActorInput(const uint8_t *keyState)
{
}
#include "./Player.h"
#include "./Racer.h"
#include "../component/InputComponent.h"

Player::Player(Game *game, int clientID)
	: Actor(game), mGame(game), mClientID(clientID)
{
	// Create an input component and set keys/speed
	InputComponent *ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Player::UpdateActor(float deltaTime)
{
	//SetPosition(mGame->mRacer[mGame->clientID]->GetPosition());
	//SetRotation(mGame->mRacer[mGame->clientID]->GetRotation());
}

void Player::ActorInput(const uint8_t *keyState)
{
}
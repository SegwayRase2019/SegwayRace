#include "./Player.h"
#include "./Racer.h"
#include "../component/InputComponent.h"

Player::Player(Game *game, int clientID)
	: Actor(game), mGame(game), mClientID(clientID), mPlState(EPreparing)
{
	// Create an input component and set keys/speed
	InputComponent *ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_Q);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	ic->SetMaxForwardSpeed(300.0f);
	ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Player::UpdateActor(float deltaTime)
{
	if(mPlState == ERunning)
		UpdateComponents(deltaTime);
}

void Player::ActorInput(const uint8_t *keyState)
{
}

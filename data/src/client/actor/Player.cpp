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

void Player::Update(float deltaTime)
{
	if (mState == EActive)
	{
		ComputeRocalTransform();

		UpdateComponents(deltaTime);
		UpdateActor(deltaTime);

		ComputeRocalTransform();
	}
}

void Player::UpdateActor(float deltaTime)
{
}

void Player::ActorInput(const uint8_t *keyState)
{
}

void Player::ComputeRocalTransform()
{
	if (mRecomputeTransform)
	{
		mRecomputeTransform = false;
		// Scale, then rotate, then translate
		//mWorldTransform = Matrix3::CreateScale(mScale);
		mRocalTransform = Matrix3::CreateTranslation(Vector2(GetPosition()));
		mRocalTransform *= Matrix3::CreateRotation(GetRotation());

		// Inform components world transform updated
		// for (auto comp : mComponents)
		// {
		// 	comp->OnUpdateWorldTransform();
		// }
	}
}

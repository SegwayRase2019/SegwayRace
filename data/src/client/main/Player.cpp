#include "Player.h"

Player::Player(Game *game) : Actor(game){
    // Create a sprite component
	SpriteComponent* sc = new SpriteComponent(this, 150);
	sc->SetTexture(game->GetTexture("Assets/Ship.png"));

	// Create an input component and set keys/speed
	InputComponent* ic = new InputComponent(this);
	ic->SetForwardKey(SDL_SCANCODE_W);
	ic->SetBackKey(SDL_SCANCODE_S);
	ic->SetClockwiseKey(SDL_SCANCODE_A);
	ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
	// ic->SetMaxForwardSpeed(300.0f);
	// ic->SetMaxAngularSpeed(Math::TwoPi);
}

void Player::UpdateActor(float deltaTime){

}

void Player::Input_Keyboard(float unit8_t *state){
    // if(){

    // }
}
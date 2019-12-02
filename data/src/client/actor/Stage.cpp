#include "./Stage.h"
#include "../main/Client_window.h"
#include "../component/InputComponent.h"

Stage::Stage(class Game *game)
    : Actor(game)
{
    SpriteComponent *sc = new SpriteComponent(this, 1);
    sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/StageSample.png"));

    // Create an input component and set keys/speed
    InputComponent *ic = new InputComponent(this);
    ic->SetForwardKey(SDL_SCANCODE_W);
    ic->SetBackKey(SDL_SCANCODE_S);
    ic->SetClockwiseKey(SDL_SCANCODE_A);
    ic->SetCounterClockwiseKey(SDL_SCANCODE_D);
    ic->SetMaxForwardSpeed(-300.0f);
    ic->SetMaxAngularSpeed(Math::TwoPi);
}
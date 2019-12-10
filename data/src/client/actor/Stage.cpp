#include "./Stage.h"
#include "../main/Client_window.h"

Stage::Stage(class Game *game)
    : Actor(game)
{
    SpriteComponent *sc = new SpriteComponent(this, 1);
    sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/StageSample.png"));
}
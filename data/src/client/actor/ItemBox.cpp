#include "./ItemBox.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../component/ItemComponent.h"
#include "../component/SpriteComponent.h"
#include "./Player.h"

ItemBox::ItemBox(Game *game)
    : Actor(game), mIntervalTime(5), mCountTime(0)
{
    SpriteComponent *sc = new SpriteComponent(this, 50);
    sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/ItemBox.png"));
    ItemComponent *itc = new ItemComponent(this);
}

void ItemBox::UpdateActor(float deltaTime)
{

    if (SDL_SCANCODE_I)
    {
        Actor::SetState(EInactive);
    }
    if (Actor::GetState() == EInactive)
    {
        mCountTime += deltaTime;
        if (mCountTime > mIntervalTime)
        {
            Actor::SetState(EActive);
            mCountTime = 0;
        }
    }
}
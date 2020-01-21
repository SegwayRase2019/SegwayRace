#include "./ItemBox.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../component/ItemComponent.h"
#include "../component/SpriteComponent.h"
#include "./Player.h"
#include "../main/Client_func.h"

ItemBox::ItemBox(Game *game)
    : Actor(game), mIntervalTime(5), mCountTime(0)
{
    SpriteComponent *sc = new SpriteComponent(this, 50);
    sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/ItemBox.png"));
    //ItemComponent *itc = new ItemComponent(this);
    Vector2 startpos(1500, -100);
    SetPosition(startpos);
}

void ItemBox::UpdateActor(float deltaTime)
{

    if (Client_command::item_collision == true)
    {
        printf("ok\n");
        Actor::SetState(EInactive);
        Client_command::item_collision = false;
        Player::addforce *= 2;
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
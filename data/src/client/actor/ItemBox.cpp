#include "./ItemBox.h"
#include "../main/Game.h"
#include "../main/Client_window.h"
#include "../component/ItemComponent.h"
#include "../component/SpriteComponent.h"
#include "./Player.h"
#include "../main/Client_func.h"

ItemBox::ItemBox(Game *game)
    : Actor(game), mIntervalTime(5), mCountTime(0), mEffectTime(5), mCountTime1(0)
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
        mGame->mPlayer->affected = true;

        printf("ok\n");
        mCountTime1 += deltaTime;
        if (mCountTime1 > mEffectTime)
        {
            mGame->mPlayer->affected = false;
            Client_command::item_collision = false;
            mCountTime1 = 0;
        }
    }
    if (Client_command::item_exist == false)
    {
        Actor::SetState(EInactive);
        mCountTime += deltaTime;
        if (mCountTime > mIntervalTime)
        {
            Actor::SetState(EActive);
            Client_command::item_exist = true;
            mCountTime = 0;
        }
    }
    if (Actor::GetState() == EInactive)
    {
    }
}
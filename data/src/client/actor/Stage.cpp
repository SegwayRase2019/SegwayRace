#include "./Stage.h"
#include "./Player.h"
#include "../main/Client_window.h"
#include "../../common/math/Math.h"
#include "../ui/PreparingUI.h"

Stage::Stage(class Game *game)
    : Actor(game)
{
    SpriteComponent *sc = new SpriteComponent(this, 1);
    sc->SetTexture(game->GetClient_window()->GetTexture("assets/images/road.png"));
    SetScale(MAP_SCALE);
    class PreparingUI *pr = new PreparingUI(game);
}

void Stage::SetStatrtPosition()
{
    for (int k = 0; k < MAX_CLIENTS; k++)
    {
        StartPosition[k].x = BASIC_POSITION_X + (POSITON_GAP * k);
        StartPosition[k].y = BASIC_POSITION_Y - (POSITON_GAP * k);
    }

    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        mGame->GetClient_command()->PlayerPos[i] =  StartPosition[i];
        if (i == mGame->clientID)
        {
            Vector2 PS(StartPosition[i].x, StartPosition[i].y);
            mGame->mPlayer->SetPosition(PS);
        }
    }
}
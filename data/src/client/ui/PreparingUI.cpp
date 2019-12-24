#include "./PreparingUI.h"
#include "../main/Client_window.h"
#include "../main/Game.h"
#include "../main/Client_func.h"
#include "../../common/math/Math.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

PreparingUI::PreparingUI(Game* game)
    : Canvas(game)
    , mCount(0)
    , mInterval(1)
    , mCountTime(0)
{
    for (int i = 0; i < 4; i++)
    {
        switch (i)
        {
        case 0:
            mCountdown[i] = mGame->GetClient_window()->GetTexture("assets/images/3.png");
            break;
        case 1:
            mCountdown[i] = mGame->GetClient_window()->GetTexture("assets/images/2.png");
            break;
        case 2:
            mCountdown[i] = mGame->GetClient_window()->GetTexture("assets/images/1.png");
            break;
        case 3:
            mCountdown[i] = mGame->GetClient_window()->GetTexture("assets/images/Start.png");
            break;
        }
    }
}

PreparingUI::~PreparingUI()
{
}

void PreparingUI::Draw(SDL_Renderer* renderer)
{
    Vector2 uiPos;
    int w, h;
    SDL_QueryTexture(mCountdown[mCount], NULL, NULL, &w, &h);
    float uiSize = (float)mWindowHeight / 1000 * mCountTime;
    uiPos.x = (mWindowWidth / 2) - ((float)w / 2)*uiSize;
    uiPos.y = (mWindowHeight / 2) - ((float)h / 2)*uiSize;
    DrawTexture(renderer, mCountdown[mCount], uiPos, uiSize);
}

void PreparingUI::Update(float deltaTime)
{
    Canvas::Update(deltaTime);
    mCountTime += deltaTime;
    if(mCountTime > mInterval)
    {
        ++mCount;
        mCountTime = 0;
    }
    if(mCount > 3)
    {
        mGame->GetClient_command()->SendStartSignal();
        Close();
    }
}
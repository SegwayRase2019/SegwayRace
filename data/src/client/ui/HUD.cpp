#include "./HUD.h"
#include "../main/Client_window.h"
#include "../main/Game.h"
#include "../actor/Racer.h"
#include "../main/Client_func.h"
#include "../../common/math/Math.h"
#include <SDL2/SDL_image.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include <algorithm>

HUD::HUD(Game *game)
    : Canvas(game)
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        switch (i)
        {
        case 0:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankOne.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_red.png");
            mPlayerPosUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Pl1pos.png");
            break;
        case 1:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankTwo.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_blue.png");
            mPlayerPosUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Pl2pos.png");
            break;
        case 2:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankThree.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_green.png");
            mPlayerPosUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Pl3pos.png");
            break;
        case 3:
            mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankFour.png");
            mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_purple.png");
            mPlayerPosUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Pl4pos.png");
            break;
        }
    }
    mNoPlayerUI = mGame->GetClient_window()->GetTexture("assets/images/Player_nothing.png");
    mMapUI = mGame->GetClient_window()->GetTexture("assets/images/StageRoad.png");
    mMapBaseUI = mGame->GetClient_window()->GetTexture("assets/images/StageUIBase.png");
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
    Canvas::Update(deltaTime);

    UpdateRanking();
    PlayerPosUpdate();
}

void HUD::Draw(SDL_Renderer *renderer)
{
    // ランキングの描画
    float uiSize = (float)mWindowHeight / 1000;
    int w, h;
    SDL_QueryTexture(mRankingUI[0], NULL, NULL, &w, &h);
    Vector2 uiPos = Vector2::Zero;
    uiPos.y += mWindowHeight - ((float)h * uiSize);
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        DrawTexture(renderer, mRankingUI[i], uiPos, uiSize);
        uiPos.x += ((float)w * uiSize) * 2;
    }
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        if (i < mClientNum)
        {
            uiPos.x = ((float)w * uiSize) * (mRank[i] * 2 - 1);
            DrawTexture(renderer, mRacerUI[i], uiPos, uiSize);
        }
        else
        {
            uiPos.x = ((float)w * uiSize) * ((i + 1) * 2 - 1);
            DrawTexture(renderer, mNoPlayerUI, uiPos, uiSize);
        }
    }
    // ミニマップの描画
    int w2, h2;
    uiSize = (float)mWindowHeight / 2500;
    SDL_QueryTexture(mMapBaseUI, NULL, NULL, &w, &h);
    SDL_QueryTexture(mMapUI, NULL, NULL, &w2, &h2);
    uiPos = Vector2::Zero;
    DrawTexture(renderer, mMapBaseUI, uiPos, uiSize);
    DrawTexture(renderer, mMapUI, uiPos, uiSize);
    // ミニマップにプレイヤを描画
    SDL_QueryTexture(mPlayerPosUI[0], NULL, NULL, &w, &h);
    uiPos.x += (((float)w2 * uiSize) / 2);
    uiPos.y += (((float)h2 * uiSize) / 2);
    float plUISize = uiSize / 2;
    Vector2 plUIPos = Vector2::Zero;
    for (int i = 0; i < mClientNum; i++)
    {
        plUIPos = uiPos;
        plUIPos.x += ((mPlPos[i].x / (10 / uiSize)) - (w * plUISize / 2)); // 10はステージのミニマップに対する画像倍率
        plUIPos.y += ((mPlPos[i].y / (10 / uiSize)) - (h * plUISize / 2));
        DrawTexture(renderer, mPlayerPosUI[i], plUIPos, plUISize);
    }
}

void HUD::AddItemComponent(ItemComponent *item)
{
    mItems.emplace_back(item);
}

void HUD::RemoveItemComponent(ItemComponent *item)
{
    auto iter = std::find(mItems.begin(), mItems.end(), item);
    mItems.erase(iter);
}

void HUD::UpdateRanking()
{
    for (int i = 0; i < mClientNum; i++)
    {
        mRank[i] = mGame->GetClient_command()->PlayerPos[i].rank;
    }
}

void HUD::PlayerPosUpdate()
{
    for (int i = 0; i < mClientNum; i++)
    {
        mPlPos[i] = mGame->mRacer[i]->GetPosition();
    }
}
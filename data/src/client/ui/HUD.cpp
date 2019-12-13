#include "./HUD.h"
#include "../main/Client_window.h"
#include "../main/Game.h"
#include "../actor/Racer.h"
#include "../main/Client_func.h"
#include "../../common/math/Math.h"
#include <algorithm>

HUD::HUD(Game* game)
    : Canvas(game)
{
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        switch(i){
            case 0 : mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankOne.png");
                     mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_red.png");
                     break;
            case 1 : mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankTwo.png");
                     mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_blue.png");
                     break;
            case 2 : mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankThree.png");
                     mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_green.png");
                     break;
            case 3 : mRankingUI[i] = mGame->GetClient_window()->GetTexture("assets/images/RankFour.png");
                     mRacerUI[i] = mGame->GetClient_window()->GetTexture("assets/images/Player_purple.png");
                     break;
        }
    }
    mNoPlayerUI = mGame->GetClient_window()->GetTexture("assets/images/Player_nothing.png");
    mWindowWidth = mGame->GetClient_window()->GetWidth();
    mWindowHeight = mGame->GetClient_window()->GetHeight();
}

HUD::~HUD()
{
}

void HUD::Update(float deltaTime)
{
	Canvas::Update(deltaTime);
	
	UpdateRanking();
}

void HUD::Draw(SDL_Renderer* renderer)
{
    Vector2 uiPos = Vector2::Zero;
    uiPos.y += mWindowHeight - 50;
    for(int i = 0; i < MAX_CLIENTS; i++)
    {
        DrawTexture(renderer, mRankingUI[i], uiPos, 0.5);
        uiPos.x += 50;
        if (mRank[i]>0)
            DrawTexture(renderer, mRacerUI[mRank[i] - 1], uiPos, 0.5);
        else
            DrawTexture(renderer, mNoPlayerUI, uiPos, 0.5);
        uiPos.x += 50;
    }
}

void HUD::AddItemComponent(ItemComponent* item)
{
    mItems.emplace_back(item);
}

void HUD::RemoveItemComponent(ItemComponent* item)
{
    auto iter = std::find(mItems.begin(), mItems.end(), item);
    mItems.erase(iter);
}

void HUD::UpdateRanking()
{
    for (int i = 0; i < MAX_CLIENTS; i++)
    {
        mRank[i] = mGame->GetClient_command()->PlayerPos[i].rank;
    }
}